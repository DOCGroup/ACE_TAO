// $Id$

#include "orbsvcs/CosEvent/CEC_ProxyPushConsumer.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.h"

#include "ace/Reverse_Lock_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (CosEvent,
           CEC_ProxyPushConsumer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

TAO_CEC_ProxyPushConsumer::
TAO_CEC_ProxyPushConsumer (TAO_CEC_EventChannel* ec,
                           const ACE_Time_Value &timeout)
  : event_channel_ (ec),
    timeout_(timeout),
    refcount_ (1),
    connected_ (false)
{
  this->lock_ =
    this->event_channel_->create_consumer_lock ();

  this->default_POA_ =
    this->event_channel_->consumer_poa ();

  this->event_channel_->get_servant_retry_map ().bind (this, 0);
}

TAO_CEC_ProxyPushConsumer::~TAO_CEC_ProxyPushConsumer (void)
{
  this->event_channel_->get_servant_retry_map ().unbind (this);
  this->event_channel_->destroy_consumer_lock (this->lock_);
}

void
TAO_CEC_ProxyPushConsumer::activate (
    CosEventChannelAdmin::ProxyPushConsumer_ptr &activated_proxy)
{
  CosEventChannelAdmin::ProxyPushConsumer_var result;
  try
    {
      result = this->_this ();
    }
  catch (const CORBA::Exception&)
    {
      result  =  CosEventChannelAdmin::ProxyPushConsumer::_nil ();
    }
  activated_proxy =  result._retn ();
}

void
TAO_CEC_ProxyPushConsumer::deactivate (void)
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
      // indicate that an object is beign disconnected twice, or some
      // race condition, but not a fault that the user needs to know
      // about.
    }
}

CORBA::Boolean
TAO_CEC_ProxyPushConsumer::supplier_non_existent (
      CORBA::Boolean_out disconnected)
{
  CORBA::Object_var supplier;
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
    if (CORBA::is_nil (this->nopolicy_supplier_.in ()))
      {
        return false;
      }
    supplier = CORBA::Object::_duplicate (this->nopolicy_supplier_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return supplier->_non_existent ();
#else
  return false;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_ProxyPushConsumer::shutdown (void)
{
  CosEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    supplier = this->supplier_._retn ();
    this->connected_ = false;
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
TAO_CEC_ProxyPushConsumer::cleanup_i (void)
{
  this->supplier_ =
    CosEventComm::PushSupplier::_nil ();
  this->connected_ = false;
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
    --this->refcount_;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->event_channel_->destroy_proxy (this);
  return 0;
}

void
TAO_CEC_ProxyPushConsumer::connect_push_supplier (
      CosEventComm::PushSupplier_ptr push_supplier)
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      {
        if (this->event_channel_->supplier_reconnect () == 0)
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

          this->event_channel_->disconnected (this);
        }

        // What if a second thread connected us after this?
        if (this->is_connected_i ())
          return;
      }
    this->supplier_ = apply_policy (push_supplier);
    this->connected_ = true;
  }

  // Notify the event channel...
  this->event_channel_->connected (this);
}

CosEventComm::PushSupplier_ptr
TAO_CEC_ProxyPushConsumer::apply_policy (CosEventComm::PushSupplier_ptr pre)
{
  if (CORBA::is_nil (pre)) return pre;
  this->nopolicy_supplier_ = CosEventComm::PushSupplier::_duplicate (pre);
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  CosEventComm::PushSupplier_var post =
    CosEventComm::PushSupplier::_duplicate (pre);
  if (this->timeout_ > ACE_Time_Value::zero)
    {
      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] = this->event_channel_->
        create_roundtrip_timeout_policy (this->timeout_);

      CORBA::Object_var post_obj = pre->_set_policy_overrides
        (policy_list, CORBA::ADD_OVERRIDE);
      post = CosEventComm::PushSupplier::_narrow(post_obj.in ());

      policy_list[0]->destroy ();
      policy_list.length (0);
    }
  return post._retn ();
#else
  return CosEventComm::PushSupplier::_duplicate (pre);
#endif /* TAO_HAS_CORBA_MESSAGING */
}

void
TAO_CEC_ProxyPushConsumer::push (const CORBA::Any& event)
{
  TAO_CEC_ProxyPushConsumer_Guard ace_mon (this->lock_,
                                           this->refcount_,
                                           this->event_channel_,
                                           this);
  if (!ace_mon.locked ())
    return;

  this->event_channel_->consumer_admin ()->push (event);
}

void
TAO_CEC_ProxyPushConsumer::disconnect_push_consumer ()
{
  CosEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (!this->is_connected_i ())
      throw CORBA::BAD_INV_ORDER (); // @@ add user exception?

    supplier = this->supplier_._retn ();

    this->cleanup_i ();
  }

  // Notify the event channel...
  this->event_channel_->disconnected (this);

  if (CORBA::is_nil (supplier.in ()))
    return;

  if (this->event_channel_->disconnect_callbacks ())
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

PortableServer::POA_ptr
TAO_CEC_ProxyPushConsumer::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPushConsumer::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPushConsumer::_remove_ref (void)
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
     locked_ (false)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
  // If the guard fails there is not much we can do, raising an
  // exception is wrong, the client has *no* way to handle that kind
  // of error.  Even worse, there is no exception to raise in that
  // case.
  // @@ Returning something won't work either, the error should be
  // logged though!

  if (!proxy->is_connected_i ())
    return;

  this->locked_ = true;
  ++this->refcount_;
}

TAO_CEC_ProxyPushConsumer_Guard::
    ~TAO_CEC_ProxyPushConsumer_Guard (void)
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
  this->event_channel_->destroy_proxy (this->proxy_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
