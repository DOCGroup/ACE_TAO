// $Id$

#include "CEC_ProxyPushConsumer.h"
#include "CEC_EventChannel.h"
#include "CEC_ConsumerAdmin.h"
#include "CEC_ProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_ProxyPushConsumer.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_ProxyPushConsumer, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

TAO_CEC_ProxyPushConsumer::
    TAO_CEC_ProxyPushConsumer (TAO_CEC_EventChannel* ec)
  : event_channel_ (ec),
    refcount_ (1)
{
  this->lock_ =
    this->event_channel_->create_consumer_lock ();

  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_CEC_ProxyPushConsumer::~TAO_CEC_ProxyPushConsumer (void)
{
  this->event_channel_->destroy_consumer_lock (this->lock_);
}

CORBA::Boolean
TAO_CEC_ProxyPushConsumer::supplier_non_existent (
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

#if !defined(TAO_HAS_MINIMUM_CORBA)
  return this->supplier_->_non_existent (ACE_TRY_ENV);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_ProxyPushConsumer::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  CosEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return;

    supplier = this->supplier_._retn ();

    this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_TRY
    {
      supplier->disconnect_push_supplier (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions, we must isolate other clients from
      // failures on this one.
    }
  ACE_ENDTRY;
}

void
TAO_CEC_ProxyPushConsumer::cleanup_i (void)
{
  this->supplier_ =
    CosEventComm::PushSupplier::_nil ();
}

void
TAO_CEC_ProxyPushConsumer::deactivate (CORBA::Environment &ACE_TRY_ENV)
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
TAO_CEC_ProxyPushConsumer::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_CEC_ProxyPushConsumer::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->event_channel_->destroy_proxy_push_consumer (this);
  return 0;
}

void
TAO_CEC_ProxyPushConsumer::connect_push_supplier (
      CosEventComm::PushSupplier_ptr push_supplier,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected))
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      {
        if (this->event_channel_->supplier_reconnect () == 0)
          ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());

        // Re-connections are allowed, go ahead and disconnect the
        // consumer...
        this->cleanup_i ();

        // @@ Please read the comments in CEC_ProxyPushSupplier about
        //     possible race conditions in this area...
        TAO_CEC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_CEC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->disconnected (this, ACE_TRY_ENV);
          ACE_CHECK;
        }

        // What if a second thread connected us after this?
        if (this->is_connected_i ())
          return;
      }
    this->supplier_ =
      CosEventComm::PushSupplier::_duplicate (push_supplier);
  }

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_CEC_ProxyPushConsumer::push (const CORBA::Any& event,
                                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ The following code is not exception safe, must fix, but the
  // canonical tricks don't work: the destroy_push_consumer () method
  // must be invoked only once the mutex is released, but after the
  // refcount get to zero, seems hard...

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return; // @@ THROW something???

    this->refcount_++;
  }

  this->event_channel_->consumer_admin ()->push (event,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;
    this->refcount_--;
    if (this->refcount_ != 0)
      return;
  }
  this->event_channel_->destroy_proxy_push_consumer (this);
}

void
TAO_CEC_ProxyPushConsumer::disconnect_push_consumer (
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      ACE_THROW (CORBA::BAD_INV_ORDER ()); // @@ add user exception?

    supplier = this->supplier_._retn ();

    this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  // Notify the event channel...
  this->event_channel_->disconnected (this, ACE_TRY_ENV);
  ACE_CHECK;

  if (this->event_channel_->disconnect_callbacks ())
    {
      ACE_TRY
        {
          supplier->disconnect_push_supplier (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore exceptions, we must isolate other clients from
          // failures on this one.
        }
      ACE_ENDTRY;
    }

  this->_decr_refcnt ();
}

PortableServer::POA_ptr
TAO_CEC_ProxyPushConsumer::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPushConsumer::_add_ref (CORBA::Environment &)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPushConsumer::_remove_ref (CORBA::Environment &)
{
  this->_decr_refcnt ();
}
