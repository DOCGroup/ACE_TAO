// $Id$

#include "EC_ProxySupplier.h"
#include "EC_Dispatching.h"
#include "EC_Filter_Builder.h"
#include "EC_QOS_Info.h"
#include "EC_Event_Channel.h"
#include "EC_Scheduling_Strategy.h"

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

    if (this->is_connected_i () == 0)
      return;

    consumer = this->consumer_._retn ();

    this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  this->_decr_refcnt ();

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

void
TAO_EC_ProxyPushSupplier::deactivate (CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
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
  this->event_channel_->destroy_proxy_push_supplier (this);
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

        // Re-connections are allowed, go ahead and disconnect the
        // consumer...
        this->cleanup_i ();

        // @@ Are there any race conditions here:
        //   + The lock is released, but the object is marked as
        //     disconnected already, so:
        //     - No events will be pushed
        //     - Any disconnects will just return
        //   + But another thread could invoke connect_push_consumer()
        //     again, notice that by the time the lock is acquired
        //     again the connected() call may still be running.
        //     It seems like we need delayed operations again, or
        //     something similar to what the POA does in this
        //     scenario.
        //     Meanwhile we can tell the users: "if it hurts don't do
        //     it".
        //
        TAO_EC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_EC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->disconnected (this, ACE_TRY_ENV);
          ACE_CHECK;
        }

        // What if a second thread connected us after this?
        if (this->is_connected_i ())
          return;
      }

    this->consumer_ =
      RtecEventComm::PushConsumer::_duplicate (push_consumer);
    this->qos_ = qos;

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

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      ACE_THROW (CORBA::BAD_INV_ORDER ());

    consumer = this->consumer_._retn ();

    this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  // Notify the event channel....
  this->event_channel_->disconnected (this, ACE_TRY_ENV);
  ACE_CHECK;

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

  this->_decr_refcnt ();
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

int
TAO_EC_ProxyPushSupplier::filter (const RtecEventComm::EventSet& event,
                                  TAO_EC_QOS_Info& qos_info,
                                  CORBA::Environment& ACE_TRY_ENV)
{
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
    if (this->refcount_ > 0)
      return result;
  }

  this->event_channel_->destroy_proxy_push_supplier (this);
  return result;
}

int
TAO_EC_ProxyPushSupplier::filter_nocopy (RtecEventComm::EventSet& event,
                                         TAO_EC_QOS_Info& qos_info,
                                         CORBA::Environment& ACE_TRY_ENV)
{
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
    if (this->refcount_ > 0)
      return result;
  }

  this->event_channel_->destroy_proxy_push_supplier (this);
  return result;
}

void
TAO_EC_ProxyPushSupplier::push (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info,
                                CORBA::Environment& ACE_TRY_ENV)
{
  // No need to grab the lock, it is beign held already by the
  // filter() method
  this->refcount_++;
  this->event_channel_->dispatching ()->push (this,
                                              event,
                                              qos_info,
                                              ACE_TRY_ENV);
  if (this->child_ != 0)
    this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::push_nocopy (RtecEventComm::EventSet& event,
                                       TAO_EC_QOS_Info& qos_info,
                                       CORBA::Environment& ACE_TRY_ENV)
{
  // No need to grab the lock, it is beign held already by the
  // filter() method
  this->refcount_++;
  this->event_channel_->dispatching ()->push_nocopy (this,
                                                     event,
                                                     qos_info,
                                                     ACE_TRY_ENV);
  if (this->child_ != 0)
    this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::push_to_consumer (const RtecEventComm::EventSet& event,
                                            CORBA::Environment& ACE_TRY_ENV)
{
  RtecEventComm::PushConsumer_var consumer;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    // The reference count was increased just before pushing to the
    // dispatching module, we must decrease here.  But if we get
    // removed then we abort.  We don't want to call _decr_refcnt()
    // because that will require two locks.
    this->refcount_--;
    if (this->refcount_ == 0)
      {
        ace_mon.release ();
        this->event_channel_->destroy_proxy_push_supplier (this);
        return;
      }

    if (this->is_connected_i () == 0)
      return; // ACE_THROW (RtecEventComm::Disconnected ());????

    if (this->suspended_ != 0)
      return;

    consumer =
      RtecEventComm::PushConsumer::_duplicate (this->consumer_.in ());

    // The refcount cannot be zero, because we have at least two
    // references,
  }

  ACE_TRY
    {
      consumer->push (event, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ This is where the policies for misbehaving consumers
      //    should kick in.... for the moment just ignore them.
    }
  ACE_ENDTRY;
}

void
TAO_EC_ProxyPushSupplier::reactive_push_to_consumer (
    const RtecEventComm::EventSet& event,
    CORBA::Environment& ACE_TRY_ENV)
{
  if (this->is_connected_i () == 0)
    return; // TAO_THROW (RtecEventComm::Disconnected ());????

  if (this->suspended_ != 0)
    return;

  RtecEventComm::PushConsumer_var consumer =
    RtecEventComm::PushConsumer::_duplicate (this->consumer_.in ());

  {
    TAO_EC_Unlock reverse_lock (*this->lock_);

    ACE_GUARD_THROW_EX (TAO_EC_Unlock, ace_mon, reverse_lock,
                        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    ACE_TRY
      {
        consumer->push (event, ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        // @@ This is where the policies for misbehaving consumers
        //    should kick in.... for the moment just ignore them.
      }
    ACE_ENDTRY;
  }

  // The reference count was incremented just before delegating on the
  // dispatching strategy, in this can we need to decrement it *now*.
  this->refcount_--;
}

CORBA::Boolean
TAO_EC_ProxyPushSupplier::consumer_non_existent (
      CORBA::Boolean_out disconnected,
      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK_RETURN (0);

  disconnected = 0;
  if (this->is_connected_i () == 0)
    {
      disconnected = 1;
      return 0;
    }

  return this->consumer_->_non_existent (ACE_TRY_ENV);
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

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
