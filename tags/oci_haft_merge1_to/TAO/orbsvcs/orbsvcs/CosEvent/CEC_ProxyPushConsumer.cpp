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
    refcount_ (1),
    connected_ (0)
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

void
TAO_CEC_ProxyPushConsumer::activate (
    CosEventChannelAdmin::ProxyPushConsumer_ptr &activated_proxy
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventChannelAdmin::ProxyPushConsumer_var result;
  ACE_TRY
    {
      result = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      result  =  CosEventChannelAdmin::ProxyPushConsumer::_nil ();
    }
  ACE_ENDTRY;
  activated_proxy =  result._retn ();
}

void
TAO_CEC_ProxyPushConsumer::deactivate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      PortableServer::POA_var poa =
        this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
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

CORBA::Boolean
TAO_CEC_ProxyPushConsumer::supplier_non_existent (
      CORBA::Boolean_out disconnected
      ACE_ENV_ARG_DECL)
{
  CORBA::Object_var supplier;
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
    if (CORBA::is_nil (this->supplier_.in ()))
      {
        return 0;
      }
    supplier = CORBA::Object::_duplicate (this->supplier_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return supplier->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_ProxyPushConsumer::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  CosEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    supplier = this->supplier_._retn ();
    this->connected_ = 0;
  }

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (supplier.in ()))
    return;

  ACE_TRY
    {
      supplier->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
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
  this->connected_ = 0;
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
  this->event_channel_->destroy_proxy (this);
  return 0;
}

void
TAO_CEC_ProxyPushConsumer::connect_push_supplier (
      CosEventComm::PushSupplier_ptr push_supplier
      ACE_ENV_ARG_DECL)
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

          this->event_channel_->disconnected (this ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

        // What if a second thread connected us after this?
        if (this->is_connected_i ())
          return;
      }
    this->supplier_ =
      CosEventComm::PushSupplier::_duplicate (push_supplier);
    this->connected_ = 1;
  }

  // Notify the event channel...
  this->event_channel_->connected (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ProxyPushConsumer::push (const CORBA::Any& event
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_CEC_ProxyPushConsumer_Guard ace_mon (this->lock_,
                                           this->refcount_,
                                           this->event_channel_,
                                           this);
  if (!ace_mon.locked ())
    return;

  this->event_channel_->consumer_admin ()->push (event
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_ProxyPushConsumer::disconnect_push_consumer (
      ACE_ENV_SINGLE_ARG_DECL)
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

  // Notify the event channel...
  this->event_channel_->disconnected (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (supplier.in ()))
    return;

  if (this->event_channel_->disconnect_callbacks ())
    {
      ACE_TRY
        {
          supplier->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore exceptions, we must isolate other clients from
          // failures on this one.
        }
      ACE_ENDTRY;
    }
}

PortableServer::POA_ptr
TAO_CEC_ProxyPushConsumer::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPushConsumer::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPushConsumer::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

// ****************************************************************

TAO_CEC_ProxyPushConsumer_Guard::
    TAO_CEC_ProxyPushConsumer_Guard (ACE_Lock *lock,
                                     CORBA::ULong &refcount,
                                     TAO_CEC_EventChannel *ec,
                                     TAO_CEC_ProxyPushConsumer *proxy)
 :   lock_ (lock),
     refcount_ (refcount),
     event_channel_ (ec),
     proxy_ (proxy),
     locked_ (0)
{
  ACE_Guard<ACE_Lock> ace_mon (*this->lock_);
  // If the guard fails there is not much we can do, raising an
  // exception is wrong, the client has *no* way to handle that kind
  // of error.  Even worse, there is no exception to raise in that
  // case.
  // @@ Returning something won't work either, the error should be
  // logged though!

  if (proxy->is_connected_i () == 0)
    return;

  this->locked_ = 1;
  this->refcount_++;
}

TAO_CEC_ProxyPushConsumer_Guard::
    ~TAO_CEC_ProxyPushConsumer_Guard (void)
{
  // This access is safe because guard objects are created on the
  // stack, only one thread has access to them
  if (!this->locked_)
    return;

  {
    ACE_Guard<ACE_Lock> ace_mon (*this->lock_);
    // If the guard fails there is not much we can do, raising an
    // exception is wrong, the client has *no* way to handle that kind
    // of error.  Even worse, there is no exception to raise in that
    // case.
    // @@ Returning something won't work either, the error should be
    // logged though!

    this->refcount_--;
    if (this->refcount_ != 0)
      return;
  }
  this->event_channel_->destroy_proxy (this->proxy_);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
