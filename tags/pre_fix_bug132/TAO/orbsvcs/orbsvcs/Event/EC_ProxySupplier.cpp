// $Id$

#include "EC_ProxySupplier.h"
#include "EC_Dispatching.h"
#include "EC_Filter_Builder.h"
#include "EC_QOS_Info.h"
#include "EC_Event_Channel.h"
#include "EC_Scheduling_Strategy.h"
#include "EC_ConsumerControl.h"
#include "orbsvcs/ESF/ESF_RefCount_Guard.h"
#include "orbsvcs/ESF/ESF_Proxy_RefCount_Guard.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxySupplier, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_EC_Unlock;

TAO_EC_ProxyPushSupplier::TAO_EC_ProxyPushSupplier (TAO_EC_Event_Channel* ec)
  : event_channel_ (ec),
    refcount_ (1),
    suspended_ (0),
    child_ (0)
{
  this->lock_ =
    this->event_channel_->create_supplier_lock ();

  this->default_POA_ =
    this->event_channel_->supplier_poa ();
}

TAO_EC_ProxyPushSupplier::~TAO_EC_ProxyPushSupplier (void)
{
  this->event_channel_->destroy_supplier_lock (this->lock_);
  this->cleanup_i ();
}

void
TAO_EC_ProxyPushSupplier::connected (TAO_EC_ProxyPushConsumer* consumer,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Scheduling_Strategy *s =
    this->event_channel_->scheduling_strategy ();

  s->add_proxy_supplier_dependencies (this,
                                      consumer,
                                      ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::reconnected (TAO_EC_ProxyPushConsumer* consumer,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Scheduling_Strategy *s =
    this->event_channel_->scheduling_strategy ();

  s->add_proxy_supplier_dependencies (this,
                                      consumer,
                                      ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::disconnected (TAO_EC_ProxyPushConsumer*,
                                        CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::connected (TAO_EC_ProxyPushSupplier*,
                                     CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::reconnected (TAO_EC_ProxyPushSupplier*,
                                     CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::disconnected (TAO_EC_ProxyPushSupplier*,
                                        CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Save the consumer we where connected to, we need to send a
  // disconnect message to it.
  RtecEventComm::PushConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    int connected = this->is_connected_i ();

    consumer = this->consumer_._retn ();

    if (connected)
      this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (consumer.in ()))
    return;

  ACE_TRY
    {
      consumer->disconnect_push_consumer (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions, we must isolate other clients from
      // problems on this one.
    }
  ACE_ENDTRY;
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

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EC_ProxyPushSupplier::activate (CORBA::Environment &ACE_TRY_ENV) ACE_THROW_SPEC (())
{
  RtecEventChannelAdmin::ProxyPushSupplier_var result;
  ACE_TRY
    {
      result =
        this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions...
    }
  ACE_ENDTRY;
  return result._retn ();
}

void
TAO_EC_ProxyPushSupplier::deactivate (CORBA::Environment &ACE_TRY_ENV) ACE_THROW_SPEC (())
{
  ACE_TRY
    {
      PortableServer::POA_var poa =
        this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Exceptions here should not be propagated.  They usually
      // indicate that an object is beign disconnected twice, or some
      // race condition, but not a fault that the user needs to know
      // about.
    }
  ACE_ENDTRY;
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->event_channel_->destroy_proxy (this);
  return 0;
}

void
TAO_EC_ProxyPushSupplier::connect_push_consumer (
      RtecEventComm::PushConsumer_ptr push_consumer,
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannelAdmin::AlreadyConnected,
                     RtecEventChannelAdmin::TypeError))
{
  // Nil PushConsumers are illegal
  if (CORBA::is_nil (push_consumer))
    ACE_THROW (CORBA::BAD_PARAM ());

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      {
        if (this->event_channel_->consumer_reconnect () == 0)
          ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

        // Re-connections are allowed....
        this->cleanup_i ();

        this->consumer_ =
          RtecEventComm::PushConsumer::_duplicate (push_consumer);
        this->qos_ = qos;
        this->child_ =
          this->event_channel_->filter_builder ()->build (this,
                                                          this->qos_,
                                                          ACE_TRY_ENV);
        ACE_CHECK;

        this->adopt_child (this->child_);

        TAO_EC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_EC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->reconnected (this, ACE_TRY_ENV);
          ACE_CHECK;
        }

        // A separate thread could have connected simultaneously,
        // this is probably an application error, handle it as
        // gracefully as possible
        if (this->is_connected_i ())
          return; // @@ Should we throw
      }

    this->consumer_ =
      RtecEventComm::PushConsumer::_duplicate (push_consumer);
    this->qos_ = qos;

#if TAO_EC_ENABLE_DEBUG_MESSAGES
    ACE_DEBUG ((LM_DEBUG, "Building filters for consumer <%x>\n",
                this));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
    this->child_ =
      this->event_channel_->filter_builder ()->build (this,
                                                      this->qos_,
                                                      ACE_TRY_ENV);
    ACE_CHECK;

    this->adopt_child (this->child_);
  }

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::disconnect_push_supplier (
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventComm::PushConsumer_var consumer;
  int connected = 0;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    connected = this->is_connected_i ();
    consumer = this->consumer_._retn ();

    if (connected)
      this->cleanup_i ();
  }

  // Notify the event channel....
  this->event_channel_->disconnected (this, ACE_TRY_ENV);
  ACE_CHECK;

  if (!connected)
    {
      return;
    }

  if (this->event_channel_->disconnect_callbacks ())
    {
      ACE_TRY
        {
          consumer->disconnect_push_consumer (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore exceptions, we must isolate other clients from
          // problems on this one.
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "ProxySupplier::disconnect_push_supplier");
        }
      ACE_ENDTRY;
    }
}

void
TAO_EC_ProxyPushSupplier::suspend_connection (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
  // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  this->suspended_ = 1;
}

void
TAO_EC_ProxyPushSupplier::resume_connection (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
  // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  this->suspended_ = 0;
}

typedef TAO_ESF_Proxy_RefCount_Guard<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier> Destroy_Guard;

int
TAO_EC_ProxyPushSupplier::filter (const RtecEventComm::EventSet& event,
                                  TAO_EC_QOS_Info& qos_info,
                                  CORBA::Environment& ACE_TRY_ENV)
{
  Destroy_Guard auto_destroy (this->refcount_,
                              this->event_channel_,
                              this);

  int result = 0;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK_RETURN (0);

    if (this->is_connected_i () == 0)
      return 0;

    result =
      this->child_->filter (event, qos_info, ACE_TRY_ENV);
    ACE_CHECK_RETURN (0);
  }
  return result;
}

int
TAO_EC_ProxyPushSupplier::filter_nocopy (RtecEventComm::EventSet& event,
                                         TAO_EC_QOS_Info& qos_info,
                                         CORBA::Environment& ACE_TRY_ENV)
{
  Destroy_Guard auto_destroy (this->refcount_,
                              this->event_channel_,
                              this);

  int result = 0;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK_RETURN (0);

    if (this->is_connected_i () == 0)
      return 0;

    result =
      this->child_->filter_nocopy (event, qos_info, ACE_TRY_ENV);
    ACE_CHECK_RETURN (0);
  }
  return result;
}

void
TAO_EC_ProxyPushSupplier::push (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info,
                                CORBA::Environment& ACE_TRY_ENV)
{
  // The mutex is already held by the caller (usually the filter()
  // method)
  if (this->is_connected_i () == 0)
    return; // TAO_THROW (RtecEventComm::Disconnected ());????

  if (this->suspended_ != 0)
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

  {
    // We have to release the lock to avoid dead-locks.
    TAO_EC_Unlock reverse_lock (*this->lock_);

    ACE_GUARD_THROW_EX (TAO_EC_Unlock, ace_mon, reverse_lock,
                        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    this->event_channel_->dispatching ()->push (this,
                                                consumer.in (),
                                                event,
                                                qos_info,
                                                ACE_TRY_ENV);
    ACE_CHECK;
  }

  if (this->child_ != 0)
    this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::push_nocopy (RtecEventComm::EventSet& event,
                                       TAO_EC_QOS_Info& qos_info,
                                       CORBA::Environment& ACE_TRY_ENV)
{
  // The mutex is already held by the caller (usually the filter()
  // method)
  if (this->is_connected_i () == 0)
    return; // TAO_THROW (RtecEventComm::Disconnected ());????

  if (this->suspended_ != 0)
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

  {
    TAO_EC_Unlock reverse_lock (*this->lock_);

    ACE_GUARD_THROW_EX (TAO_EC_Unlock, ace_mon, reverse_lock,
                        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    this->event_channel_->dispatching ()->push_nocopy (this,
                                                       consumer.in (),
                                                       event,
                                                       qos_info,
                                                       ACE_TRY_ENV);
    ACE_CHECK;
  }

  if (this->child_ != 0)
    this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::push_to_consumer (
    RtecEventComm::PushConsumer_ptr consumer,
    const RtecEventComm::EventSet& event,
    CORBA::Environment& ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return; // ACE_THROW (RtecEventComm::Disconnected ());????

    if (this->suspended_ != 0)
      return;
  }

  ACE_TRY
    {
      consumer->push (event, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
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

          control->consumer_not_exist (this, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
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
                                     sysex,
                                     ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // Shouldn't happen, but does not hurt
    }
  ACE_ENDTRY;
}

void
TAO_EC_ProxyPushSupplier::reactive_push_to_consumer (
    RtecEventComm::PushConsumer_ptr consumer,
    const RtecEventComm::EventSet& event,
    CORBA::Environment& ACE_TRY_ENV)
{
  ACE_TRY
    {
      consumer->push (event, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
    {
      TAO_EC_ConsumerControl *control =
        this->event_channel_->consumer_control ();

      control->consumer_not_exist (this, ACE_TRY_ENV);
      ACE_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      TAO_EC_ConsumerControl *control =
        this->event_channel_->consumer_control ();

      control->system_exception (this,
                                 sysex,
                                 ACE_TRY_ENV);
      ACE_CHECK;
    }
  ACE_CATCHANY
    {
      // Shouldn't happen
    }
  ACE_ENDTRY;
}

CORBA::Boolean
TAO_EC_ProxyPushSupplier::consumer_non_existent (
      CORBA::Boolean_out disconnected,
      CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var consumer;
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    ACE_CHECK_RETURN (0);

    disconnected = 0;
    if (this->is_connected_i () == 0)
      {
        disconnected = 1;
        return 0;
      }

    consumer = CORBA::Object::_duplicate (this->consumer_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return consumer->_non_existent (ACE_TRY_ENV);
#else
  return 0;
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

  if (this->is_connected_i () == 0)
    return 0;

  return this->child_->can_match (header);
}

int
TAO_EC_ProxyPushSupplier::add_dependencies (
      const RtecEventComm::EventHeader &header,
      const TAO_EC_QOS_Info &qos_info,
      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
          ACE_Lock, ace_mon, *this->lock_,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK_RETURN (0);

  return this->child_->add_dependencies (header,
                                         qos_info,
                                         ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_EC_ProxyPushSupplier::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_ProxyPushSupplier::_add_ref (CORBA::Environment &)
{
  this->_incr_refcnt ();
}

void
TAO_EC_ProxyPushSupplier::_remove_ref (CORBA::Environment &)
{
  this->_decr_refcnt ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_RefCount_Guard<CORBA::ULong>;
template class TAO_ESF_Proxy_RefCount_Guard<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_RefCount_Guard<CORBA::ULong>
#pragma instantiate TAO_ESF_Proxy_RefCount_Guard<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
