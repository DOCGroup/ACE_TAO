// $Id$

#include "CEC_ProxyPushSupplier.h"
#include "CEC_Dispatching.h"
#include "CEC_EventChannel.h"
#include "CEC_ConsumerControl.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_ProxyPushSupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_ProxyPushSupplier, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

TAO_CEC_ProxyPushSupplier::TAO_CEC_ProxyPushSupplier (TAO_CEC_EventChannel* ec)
  : event_channel_ (ec),
    refcount_ (1)
{
  this->lock_ =
    this->event_channel_->create_supplier_lock ();

  this->default_POA_ =
    this->event_channel_->supplier_poa ();
}

TAO_CEC_ProxyPushSupplier::~TAO_CEC_ProxyPushSupplier (void)
{
  this->event_channel_->destroy_supplier_lock (this->lock_);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_ProxyPushSupplier::activate (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  CosEventChannelAdmin::ProxyPushSupplier_var result;
  ACE_TRY
    {
      result = this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return CosEventChannelAdmin::ProxyPushSupplier::_nil ();
    }
  ACE_ENDTRY;
  return result._retn ();
}

void
TAO_CEC_ProxyPushSupplier::deactivate (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
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

void
TAO_CEC_ProxyPushSupplier::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Save the consumer we where connected to, we need to send a
  // disconnect message to it.
  CosEventComm::PushConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return;

    consumer = this->consumer_._retn ();

    this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

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
TAO_CEC_ProxyPushSupplier::push (const CORBA::Any &event,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->refcount_++;
  this->event_channel_->dispatching ()->push (this,
                                              event,
                                              ACE_TRY_ENV);
}

void
TAO_CEC_ProxyPushSupplier::push_nocopy (CORBA::Any &event,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->refcount_++;
  this->event_channel_->dispatching ()->push_nocopy (this,
                                                     event,
                                                     ACE_TRY_ENV);
}

void
TAO_CEC_ProxyPushSupplier::cleanup_i (void)
{
  this->consumer_ =
    CosEventComm::PushConsumer::_nil ();
}

CORBA::ULong
TAO_CEC_ProxyPushSupplier::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_CEC_ProxyPushSupplier::_decr_refcnt (void)
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
TAO_CEC_ProxyPushSupplier::connect_push_consumer (
      CosEventComm::PushConsumer_ptr push_consumer,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected,
                     CosEventChannelAdmin::TypeError))
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      {
        if (this->event_channel_->consumer_reconnect () == 0)
          ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());

        // Re-connections are allowed....
        this->cleanup_i ();

        this->consumer_ =
          CosEventComm::PushConsumer::_duplicate (push_consumer);

        TAO_CEC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_CEC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->reconnected (this, ACE_TRY_ENV);
          ACE_CHECK;
        }
        return;
      }

    this->consumer_ =
      CosEventComm::PushConsumer::_duplicate (push_consumer);
  }

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_CEC_ProxyPushSupplier::disconnect_push_supplier (
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventComm::PushConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
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
}

void
TAO_CEC_ProxyPushSupplier::push_to_consumer (const CORBA::Any& event,
                                             CORBA::Environment& ACE_TRY_ENV)
{
  CosEventComm::PushConsumer_var consumer;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    // The reference count was increased just before pushing to the
    // dispatching module, we must decrease here.  But if we get
    // removed then we abort.  We don't want to call _decr_refcnt()
    // because that will require two locks.
    this->refcount_--;
    if (this->refcount_ == 0)
      {
        ace_mon.release ();
        this->event_channel_->destroy_proxy (this);
        return;
      }

    if (this->is_connected_i () == 0)
      return; // ACE_THROW (CosEventComm::Disconnected ());????

    consumer =
      CosEventComm::PushConsumer::_duplicate (this->consumer_.in ());

    // The refcount cannot be zero, because we have at least two
    // references,
  }

  ACE_TRY
    {
      consumer->push (event, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
    {
      TAO_CEC_ConsumerControl *control =
        this->event_channel_->consumer_control ();

      control->consumer_not_exist (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      TAO_CEC_ConsumerControl *control =
        this->event_channel_->consumer_control ();

      control->system_exception (this,
                                 sysex,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Shouldn't happen, but does not hurt
    }
  ACE_ENDTRY;
}

void
TAO_CEC_ProxyPushSupplier::reactive_push_to_consumer (
    const CORBA::Any& event,
    CORBA::Environment& ACE_TRY_ENV)
{
  if (this->is_connected_i () == 0)
    return; // TAO_THROW (CosEventComm::Disconnected ());????

  CosEventComm::PushConsumer_var consumer =
    CosEventComm::PushConsumer::_duplicate (this->consumer_.in ());

  {
    TAO_CEC_Unlock reverse_lock (*this->lock_);

    ACE_GUARD_THROW_EX (TAO_CEC_Unlock, ace_mon, reverse_lock,
                        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    ACE_TRY
      {
        consumer->push (event, ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }
    ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
      {
        TAO_CEC_ConsumerControl *control =
          this->event_channel_->consumer_control ();

        control->consumer_not_exist (this, ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }
    ACE_CATCH (CORBA::SystemException, sysex)
      {
        TAO_CEC_ConsumerControl *control =
          this->event_channel_->consumer_control ();

        control->system_exception (this,
                                   sysex,
                                   ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        // Shouldn't happen, but does not hurt
      }
    ACE_ENDTRY;
  }

  // The reference count was incremented just before delegating on the
  // dispatching strategy, in this can we need to decrement it *now*.
  this->refcount_--;
  // @@ What if it reaches 0???
}

CORBA::Boolean
TAO_CEC_ProxyPushSupplier::consumer_non_existent (
      CORBA::Boolean_out disconnected,
      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK_RETURN (0);

  disconnected = 0;
  if (this->is_connected_i () == 0)
    {
      disconnected = 1;
      return 0;
    }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return this->consumer_->_non_existent (ACE_TRY_ENV);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

PortableServer::POA_ptr
TAO_CEC_ProxyPushSupplier::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPushSupplier::_add_ref (CORBA::Environment &)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPushSupplier::_remove_ref (CORBA::Environment &)
{
  this->_decr_refcnt ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
