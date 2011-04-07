// $Id$

#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_Dispatching.h"
#include "orbsvcs/Event/EC_Filter_Builder.h"
#include "orbsvcs/Event/EC_QOS_Info.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_Scheduling_Strategy.h"
#include "orbsvcs/Event/EC_ConsumerControl.h"
#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/ESF/ESF_RefCount_Guard.h"
#include "orbsvcs/ESF/ESF_Proxy_RefCount_Guard.h"

#include "ace/Reverse_Lock_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Reverse_Lock<ACE_Lock> TAO_EC_Unlock;

TAO_EC_ProxyPushSupplier::TAO_EC_ProxyPushSupplier (TAO_EC_Event_Channel_Base* ec, int validate_connection)
  : event_channel_ (ec),
    refcount_ (1),
    suspended_ (false),
    child_ (0),
    consumer_validate_connection_(validate_connection)
{
  this->lock_ =
    this->event_channel_->create_supplier_lock ();

  this->default_POA_ =
    this->event_channel_->supplier_poa ();

  this->qos_.is_gateway = false;
}

TAO_EC_ProxyPushSupplier::~TAO_EC_ProxyPushSupplier (void)
{
  this->event_channel_->destroy_supplier_lock (this->lock_);
  this->cleanup_i ();
}

void
TAO_EC_ProxyPushSupplier::connected (TAO_EC_ProxyPushConsumer* consumer)
{
  TAO_EC_Scheduling_Strategy *s =
    this->event_channel_->scheduling_strategy ();

  s->add_proxy_supplier_dependencies (this, consumer);
}

void
TAO_EC_ProxyPushSupplier::reconnected (TAO_EC_ProxyPushConsumer* consumer)
{
  TAO_EC_Scheduling_Strategy *s =
    this->event_channel_->scheduling_strategy ();

  s->add_proxy_supplier_dependencies (this, consumer);
}

void
TAO_EC_ProxyPushSupplier::disconnected (TAO_EC_ProxyPushConsumer*)
{
}

void
TAO_EC_ProxyPushSupplier::connected (TAO_EC_ProxyPushSupplier*)
{
}

void
TAO_EC_ProxyPushSupplier::reconnected (TAO_EC_ProxyPushSupplier*)
{
}

void
TAO_EC_ProxyPushSupplier::disconnected (TAO_EC_ProxyPushSupplier*)
{
}

void
TAO_EC_ProxyPushSupplier::shutdown (void)
{
  // Save the consumer we where connected to, we need to send a
  // disconnect message to it.
  RtecEventComm::PushConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    bool const connected = this->is_connected_i ();

    consumer = this->consumer_._retn ();

    if (connected)
      this->cleanup_i ();
  }

  this->deactivate ();

  if (CORBA::is_nil (consumer.in ()))
    return;

  try
    {
      consumer->disconnect_push_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions, we must isolate other clients from
      // problems on this one.
    }
}

void
TAO_EC_ProxyPushSupplier::cleanup_i (void)
{
  this->consumer_ =
    RtecEventComm::PushConsumer::_nil ();

  // @@ Why don't we have a destroy() method in the
  // filter_builder?
  delete this->child_;
  this->child_ = 0;
}

void
TAO_EC_ProxyPushSupplier::deactivate (void) throw ()
{
  try
    {
      PortableServer::ObjectId id = this->object_id ();
      this->default_POA_->deactivate_object (id);
    }
  catch (const CORBA::Exception&)
    {
      // Exceptions here should not be propagated.  They usually
      // indicate that an object is being disconnected twice, or some
      // race condition, but not a fault that the user needs to know
      // about.
    }
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

void
TAO_EC_ProxyPushSupplier::refcount_zero_hook (void)
{
  // Use the event channel
  this->event_channel_->destroy_proxy (this);
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    --this->refcount_;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  this->refcount_zero_hook ();
  return 0;
}

typedef TAO_ESF_Proxy_RefCount_Guard<TAO_EC_Event_Channel_Base,TAO_EC_ProxyPushSupplier> Destroy_Guard;

int
TAO_EC_ProxyPushSupplier::filter (const RtecEventComm::EventSet& event,
                                  TAO_EC_QOS_Info& qos_info)
{
  Destroy_Guard auto_destroy (this->refcount_,
                              this->event_channel_,
                              this);

  int result = 0;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      {
        result = this->child_->filter (event, qos_info);
      }
  }
  return result;
}

int
TAO_EC_ProxyPushSupplier::filter_nocopy (RtecEventComm::EventSet& event,
                                         TAO_EC_QOS_Info& qos_info)
{
  Destroy_Guard auto_destroy (this->refcount_,
                              this->event_channel_,
                              this);

  int result = 0;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      {
        result = this->child_->filter_nocopy (event, qos_info);
      }
  }
  return result;
}

void
TAO_EC_ProxyPushSupplier::push (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info)
{
  // The mutex is already held by the caller (usually the filter()
  // method)
  if (!this->is_connected_i ())
    return; // TAO_THROW (RtecEventComm::Disconnected ());????

  if (this->suspended_)
    return;

  TAO_ESF_RefCount_Guard<CORBA::ULong> ace_mon (this->refcount_);
  // The guard will decrement the reference count, notice that the
  // reference count can become 0, but this is not the right spot to
  // check for that and destroy the object.
  // If we did so then we would destroy the object, and consequently
  // the mutex, but the mutex is used later when the stack unwinds and
  // the filter() method tries to destroy the mutex (that originally
  // acquired the mutex in the first place).
  // So the correct thing to do is to just decrement the reference
  // count and let the filter() method do the destruction.

  RtecEventComm::PushConsumer_var consumer =
    RtecEventComm::PushConsumer::_duplicate (this->consumer_.in ());

  this->pre_dispatch_hook (const_cast<RtecEventComm::EventSet&> (event));

  {
    // We have to release the lock to avoid dead-locks.
    TAO_EC_Unlock reverse_lock (*this->lock_);

    ACE_GUARD_THROW_EX (TAO_EC_Unlock, ace_mon, reverse_lock,
                        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    this->event_channel_->dispatching ()->push (this,
                                                consumer.in (),
                                                event,
                                                qos_info);
  }

  if (this->child_ != 0)
    this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::pre_dispatch_hook (RtecEventComm::EventSet&)
{
}

void
TAO_EC_ProxyPushSupplier::push_nocopy (RtecEventComm::EventSet& event,
                                       TAO_EC_QOS_Info& qos_info)
{
  // The mutex is already held by the caller (usually the filter()
  // method)
  if (!this->is_connected_i ())
    return; // TAO_THROW (RtecEventComm::Disconnected ());????

  if (this->suspended_)
    return;

  TAO_ESF_RefCount_Guard<CORBA::ULong> ace_mon (this->refcount_);
  // The guard will decrement the reference count, notice that the
  // reference count can become 0, but this is not the right spot to
  // check for that and destroy the object.
  // If we did so then we would destroy the object, and consequently
  // the mutex, but the mutex is used later when the stack unwinds and
  // the filter() method tries to destroy the mutex (that originally
  // acquired the mutex in the first place).
  // So the correct thing to do is to just decrement the reference
  // count and let the filter() method do the destruction.

  RtecEventComm::PushConsumer_var consumer =
    RtecEventComm::PushConsumer::_duplicate (this->consumer_.in ());

  this->pre_dispatch_hook (event);

  {
    TAO_EC_Unlock reverse_lock (*this->lock_);

    ACE_GUARD_THROW_EX (TAO_EC_Unlock, ace_mon, reverse_lock,
                        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    this->event_channel_->dispatching ()->push_nocopy (this,
                                                       consumer.in (),
                                                       event,
                                                       qos_info);
  }

  if (this->child_ != 0)
    this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::push_to_consumer (
    RtecEventComm::PushConsumer_ptr consumer,
    const RtecEventComm::EventSet& event)
{
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (!this->is_connected_i ())
      return; // ACE_THROW (RtecEventComm::Disconnected ());????

    if (this->suspended_)
      return;
  }

  try
    {
      consumer->push (event);
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      // Do not report errors for old consumers
      // NOTE: The comparison below is not completely correct, it
      // could be that the remote consumer and the local consumer are
      // in fact the same object, but represented by different objects
      // references.  Unfortunately this is not a good spot to invoke
      // _is_equivalent(), and that may not give us the desired answer
      // anyway.
      if (consumer == this->consumer_.in ())
        {
          TAO_EC_ConsumerControl *control =
            this->event_channel_->consumer_control ();

          control->consumer_not_exist (this);
        }
    }
  catch (CORBA::SystemException& sysex)
    {
      // Do not report errors for old consumers
      // NOTE: The comparison below is not completely correct, it
      // could be that the remote consumer and the local consumer are
      // in fact the same object, but represented by different objects
      // references.  Unfortunately this is not a good spot to invoke
      // _is_equivalent(), and that may not give us the desired answer
      // anyway.
      if (consumer == this->consumer_.in ())
        {
          TAO_EC_ConsumerControl *control =
            this->event_channel_->consumer_control ();

          control->system_exception (this,
                                     sysex);
        }
    }
  catch (const CORBA::Exception&)
    {
      // Shouldn't happen, but does not hurt
    }
}

void
TAO_EC_ProxyPushSupplier::reactive_push_to_consumer (
    RtecEventComm::PushConsumer_ptr consumer,
    const RtecEventComm::EventSet& event)
{
  try
    {
      consumer->push (event);
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      TAO_EC_ConsumerControl *control =
        this->event_channel_->consumer_control ();

      control->consumer_not_exist (this);
    }
  catch (CORBA::SystemException& sysex)
    {
      TAO_EC_ConsumerControl *control =
        this->event_channel_->consumer_control ();

      control->system_exception (this,
                                 sysex);
    }
  catch (const CORBA::Exception&)
    {
      // Shouldn't happen
    }
}

CORBA::Boolean
TAO_EC_ProxyPushSupplier::consumer_non_existent (
      CORBA::Boolean_out disconnected)
{
  CORBA::Object_var consumer;
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());

    disconnected = false;
    if (!this->is_connected_i ())
      {
        disconnected = true;
        return false;
      }

    consumer = CORBA::Object::_duplicate (this->consumer_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return consumer->_non_existent ();
#else
  return false;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_EC_ProxyPushSupplier::clear (void)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->child_->clear ();
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::max_event_size (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->child_->max_event_size ();
}

int
TAO_EC_ProxyPushSupplier::can_match (
      const RtecEventComm::EventHeader &header) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  if (!this->is_connected_i ())
    return 0;

  return this->child_->can_match (header);
}

int
TAO_EC_ProxyPushSupplier::add_dependencies (
      const RtecEventComm::EventHeader &header,
      const TAO_EC_QOS_Info &qos_info)
{
  ACE_GUARD_THROW_EX (
          ACE_Lock, ace_mon, *this->lock_,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  return this->child_->add_dependencies (header, qos_info);
}

TAO_END_VERSIONED_NAMESPACE_DECL
