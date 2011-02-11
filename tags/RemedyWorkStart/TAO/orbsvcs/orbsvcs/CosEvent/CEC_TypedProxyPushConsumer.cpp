// -*- C++ -*-
//
// $Id$

#include "orbsvcs/CosEvent/CEC_TypedProxyPushConsumer.h"
#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"
#include "orbsvcs/CosEvent/CEC_TypedConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_DynamicImplementation.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_TypedProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Reverse_Lock_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

// Implementation skeleton constructor
TAO_CEC_TypedProxyPushConsumer::TAO_CEC_TypedProxyPushConsumer
(TAO_CEC_TypedEventChannel* ec, const ACE_Time_Value &timeout)
  : typed_event_channel_ (ec),
    timeout_ (timeout),
    refcount_ (1),
    connected_ (0)
{
  this->lock_ =
    this->typed_event_channel_->create_consumer_lock ();

  this->default_POA_ =
    this->typed_event_channel_->typed_consumer_poa ();

  this->typed_event_channel_->get_servant_retry_map ().bind (this, 0);

  // DSI initialization
  if (TAO_debug_level >= 10)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("***** Initializing the DSI for the new TypedProxyPushConsumer *****\n")));
    }

  this->dsi_impl_ = new
    TAO_CEC_DynamicImplementationServer (this->default_POA_.in(),
                                         this,
                                         this->typed_event_channel_);

  try{
    // tempporary fix, should put this into some init function.

    this->oid_ =
      this->default_POA_->activate_object (this->dsi_impl_);
  }
  catch (...){
  }
}

// Implementation skeleton destructor
TAO_CEC_TypedProxyPushConsumer::~TAO_CEC_TypedProxyPushConsumer (void)
{
  try{
    this->default_POA_->deactivate_object (this->oid_.in ());
  }
  catch (...){
  }

  delete dsi_impl_;

  this->typed_event_channel_->get_servant_retry_map ().unbind (this);
  this->typed_event_channel_->destroy_consumer_lock (this->lock_);
}

void
TAO_CEC_TypedProxyPushConsumer::activate (
    CosTypedEventChannelAdmin::TypedProxyPushConsumer_ptr &activated_proxy)
{
  CosTypedEventChannelAdmin::TypedProxyPushConsumer_var result;
  try
    {
      result = this->_this ();
    }
  catch (const CORBA::Exception&)
    {
      result = CosTypedEventChannelAdmin::TypedProxyPushConsumer::_nil ();
    }
  activated_proxy = result._retn ();
}

void
TAO_CEC_TypedProxyPushConsumer::deactivate (void)
{
  try
    {
      PortableServer::POA_var poa =
        this->_default_POA ();
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this);
      poa->deactivate_object (id.in ());
    }
  catch (const CORBA::Exception&)
    {
      // Exceptions here should not be propagated.  They usually
      // indicate that an object is being disconnected twice, or some
      // race condition, but not a fault that the user needs to know
      // about.
    }
}

CORBA::Boolean
TAO_CEC_TypedProxyPushConsumer::supplier_non_existent (
      CORBA::Boolean_out disconnected)
{
  CORBA::Object_var supplier;
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());

    disconnected = 0;
    if (this->is_connected_i () == 0)
      {
        disconnected = 1;
        return 0;
      }
    if (CORBA::is_nil (this->nopolicy_typed_supplier_.in ()))
      {
        return 0;
      }
    supplier = CORBA::Object::_duplicate
      (this->nopolicy_typed_supplier_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return supplier->_non_existent ();
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_TypedProxyPushConsumer::shutdown (void)
{
  CosEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    supplier = this->typed_supplier_._retn ();
    this->connected_ = 0;
  }

  this->deactivate ();

  if (CORBA::is_nil (supplier.in ()))
    return;

  try
    {
      supplier->disconnect_push_supplier ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions, we must isolate other clients from
      // failures on this one.
    }
}

void
TAO_CEC_TypedProxyPushConsumer::cleanup_i (void)
{
  this->typed_supplier_ =
    CosEventComm::PushSupplier::_nil ();
  this->connected_ = 0;
}

CORBA::ULong
TAO_CEC_TypedProxyPushConsumer::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_CEC_TypedProxyPushConsumer::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->typed_event_channel_->destroy_proxy (this);
  return 0;
}

void
TAO_CEC_TypedProxyPushConsumer::connect_push_supplier (
      CosEventComm::PushSupplier_ptr push_supplier)
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      {
        if (this->typed_event_channel_->supplier_reconnect () == 0)
          throw CosEventChannelAdmin::AlreadyConnected ();

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

          this->typed_event_channel_->disconnected (this);
        }

        // What if a second thread connected us after this?
        if (this->is_connected_i ())
          return;
      }
    this->typed_supplier_ = apply_policy (push_supplier);
    this->connected_ = 1;
  }

  // Notify the event channel...
  this->typed_event_channel_->connected (this);
}

CosEventComm::PushSupplier_ptr
TAO_CEC_TypedProxyPushConsumer::apply_policy
  (CosEventComm::PushSupplier_ptr pre)
{
  if (CORBA::is_nil(pre)) return pre;
  this->nopolicy_typed_supplier_ =
    CosEventComm::PushSupplier::_duplicate (pre);
  CosEventComm::PushSupplier_var post =
    CosEventComm::PushSupplier::_duplicate (pre);
  if (this->timeout_ > ACE_Time_Value::zero)
    {
      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] = this->typed_event_channel_->
        create_roundtrip_timeout_policy (this->timeout_);

      CORBA::Object_var post_obj = pre->_set_policy_overrides
        (policy_list, CORBA::ADD_OVERRIDE);
      post = CosEventComm::PushSupplier::_narrow(post_obj.in ());

      policy_list[0]->destroy ();
      policy_list.length (0);
    }
  return post._retn ();
}

void
TAO_CEC_TypedProxyPushConsumer::push (const CORBA::Any& /* event */)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_CEC_TypedProxyPushConsumer::disconnect_push_consumer ()
{
  CosEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i () == 0)
      throw CORBA::BAD_INV_ORDER (); // @@ add user exception?

    supplier = this->typed_supplier_._retn ();

    this->cleanup_i ();
  }

  // Notify the event channel...
  this->typed_event_channel_->disconnected (this);

  if (!CORBA::is_nil (supplier.in ()))
    {
      if (this->typed_event_channel_->disconnect_callbacks ())
        {
          try
            {
              supplier->disconnect_push_supplier ();
            }
          catch (const CORBA::Exception&)
            {
              // Ignore exceptions, we must isolate other clients from
              // failures on this one.
            }
        }
    }
}

PortableServer::POA_ptr
TAO_CEC_TypedProxyPushConsumer::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_TypedProxyPushConsumer::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_TypedProxyPushConsumer::_remove_ref (void)
{
  this->_decr_refcnt ();
}

CORBA::Object_ptr
TAO_CEC_TypedProxyPushConsumer::get_typed_consumer (void)

{
  CORBA::Object_var server =
         default_POA_->id_to_reference (this->oid_.in ());

  return CORBA::Object::_duplicate (server.in());
}

void
TAO_CEC_TypedProxyPushConsumer::invoke (const TAO_CEC_TypedEvent& typed_event)
{
  TAO_CEC_TypedProxyPushConsumer_Guard ace_mon (this->lock_,
                                                this->refcount_,
                                                this->typed_event_channel_,
                                                this);
  if (!ace_mon.locked ())
    return;

  this->typed_event_channel_->typed_consumer_admin ()->invoke (typed_event);
}

// ****************************************************************

TAO_CEC_TypedProxyPushConsumer_Guard::
    TAO_CEC_TypedProxyPushConsumer_Guard (ACE_Lock *lock,
                                          CORBA::ULong &refcount,
                                          TAO_CEC_TypedEventChannel *ec,
                                          TAO_CEC_TypedProxyPushConsumer *proxy)
 :   lock_ (lock),
     refcount_ (refcount),
     typed_event_channel_ (ec),
     proxy_ (proxy),
     locked_ (0)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
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

TAO_CEC_TypedProxyPushConsumer_Guard::
    ~TAO_CEC_TypedProxyPushConsumer_Guard (void)
{
  // This access is safe because guard objects are created on the
  // stack, only one thread has access to them
  if (!this->locked_)
    return;

  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
    // If the guard fails there is not much we can do, raising an
    // exception is wrong, the client has *no* way to handle that kind
    // of error.  Even worse, there is no exception to raise in that
    // case.
    // @@ Returning something won't work either, the error should be
    // logged though!

    --this->refcount_;
    if (this->refcount_ != 0)
      return;
  }
  this->typed_event_channel_->destroy_proxy (this->proxy_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
