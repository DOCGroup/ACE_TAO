// $Id$

#include "orbsvcs/CosEvent/CEC_ProxyPullConsumer.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_SupplierControl.h"
#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.h"

#include "ace/Reverse_Lock_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPullConsumer.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

TAO_CEC_ProxyPullConsumer::
TAO_CEC_ProxyPullConsumer (TAO_CEC_EventChannel* ec,
                           const ACE_Time_Value &timeout)
  : event_channel_ (ec),
    timeout_ (timeout),
    refcount_ (1)
{
  this->lock_ =
    this->event_channel_->create_consumer_lock ();

  this->default_POA_ =
    this->event_channel_->consumer_poa ();

  this->event_channel_->get_servant_retry_map ().bind (this, 0);
}

TAO_CEC_ProxyPullConsumer::~TAO_CEC_ProxyPullConsumer (void)
{
  this->event_channel_->get_servant_retry_map ().unbind (this);
  this->event_channel_->destroy_consumer_lock (this->lock_);
}

void
TAO_CEC_ProxyPullConsumer::activate (
    CosEventChannelAdmin::ProxyPullConsumer_ptr &activated_proxy)
{
  CosEventChannelAdmin::ProxyPullConsumer_var result;
  try
    {
      result = this->_this ();
    }
  catch (const CORBA::Exception&)
    {
      result = CosEventChannelAdmin::ProxyPullConsumer::_nil ();
    }
  activated_proxy = result._retn ();
}

void
TAO_CEC_ProxyPullConsumer::deactivate (void)
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

// NOTE: There is some amount of duplicated code here, but it is
// intentional. Mainly we want to avoid locking overhead when
// possible, thus the code flow is optimized for that case more than
// for small code.

CORBA::Any*
TAO_CEC_ProxyPullConsumer::try_pull_from_supplier (
    CORBA::Boolean_out has_event)
{
  has_event = 0;
  CosEventComm::PullSupplier_var supplier;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i () == 0)
      return 0; // ACE_THROW (CosEventComm::Disconnected ());????

    supplier =
      CosEventComm::PullSupplier::_duplicate (this->supplier_.in ());

    // The refcount cannot be zero, because we have at least two
    // references,
  }

  CORBA::Any_var any;
  TAO_CEC_SupplierControl *control =
                 this->event_channel_->supplier_control ();

  try
    {
      any = supplier->try_pull (has_event);

      // Inform the control that we got something from the supplier
      control->successful_transmission(this);
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      control->supplier_not_exist (this);
    }
  catch (CORBA::SystemException& sysex)
    {
      control->system_exception (this, sysex);
    }
  catch (const CORBA::Exception&)
    {
      // @@ Should not happen
    }
  return any._retn ();
}

CORBA::Any*
TAO_CEC_ProxyPullConsumer::pull_from_supplier ()
{
  CosEventComm::PullSupplier_var supplier;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i () == 0)
      return 0; // ACE_THROW (CosEventComm::Disconnected ());????

    supplier =
      CosEventComm::PullSupplier::_duplicate (this->supplier_.in ());

    // The refcount cannot be zero, because we have at least two
    // references,
  }

  CORBA::Any_var any;
  try
    {
      any = supplier->pull ();
    }
  catch (const CORBA::Exception&)
    {
      // @@ This is where the policies for misbehaving suppliers
      //    should kick in.... for the moment just ignore them.
    }
  return any._retn ();
}

CORBA::Boolean
TAO_CEC_ProxyPullConsumer::supplier_non_existent (
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
    if (CORBA::is_nil (this->nopolicy_supplier_.in ()))
      {
        return 0;
      }
    supplier = CORBA::Object::_duplicate (this->nopolicy_supplier_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return supplier->_non_existent ();
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_ProxyPullConsumer::shutdown (void)
{
  CosEventComm::PullSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    supplier = this->supplier_._retn ();
  }

  this->deactivate ();

  if (CORBA::is_nil (supplier.in ()))
    return;

  try
    {
      supplier->disconnect_pull_supplier ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions, we must isolate other clients from
      // failures on this one.
    }
}

void
TAO_CEC_ProxyPullConsumer::cleanup_i (void)
{
  this->supplier_ =
    CosEventComm::PullSupplier::_nil ();
}

CORBA::ULong
TAO_CEC_ProxyPullConsumer::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_CEC_ProxyPullConsumer::_decr_refcnt (void)
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
TAO_CEC_ProxyPullConsumer::connect_pull_supplier (
      CosEventComm::PullSupplier_ptr pull_supplier)
{
  // Nil PullSuppliers are illegal
  if (CORBA::is_nil (pull_supplier))
    throw CORBA::BAD_PARAM ();

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

        // @@ Please read the comments in CEC_ProxyPullSupplier about
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
    this->supplier_ = apply_policy (pull_supplier);
  }

  // Notify the event channel...
  this->event_channel_->connected (this);
}

CosEventComm::PullSupplier_ptr
TAO_CEC_ProxyPullConsumer::apply_policy (CosEventComm::PullSupplier_ptr pre)
{
  this->nopolicy_supplier_ = CosEventComm::PullSupplier::_duplicate (pre);
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  CosEventComm::PullSupplier_var post =
    CosEventComm::PullSupplier::_duplicate (pre);
  if (this->timeout_ > ACE_Time_Value::zero)
    {
      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] = this->event_channel_->
        create_roundtrip_timeout_policy (this->timeout_);

      CORBA::Object_var post_obj = pre->_set_policy_overrides
        (policy_list, CORBA::ADD_OVERRIDE);
      post = CosEventComm::PullSupplier::_narrow (post_obj.in ());

      policy_list[0]->destroy ();
      policy_list.length (0);
    }
  return post._retn ();
#else
  return CosEventComm::PullSupplier::_duplicate (pre);
#endif /* TAO_HAS_CORBA_MESSAGING */
}

void
TAO_CEC_ProxyPullConsumer::disconnect_pull_consumer ()
{
  CosEventComm::PullSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i () == 0)
      throw CORBA::BAD_INV_ORDER (); // @@ add user exception?

    supplier = this->supplier_._retn ();

    this->cleanup_i ();
  }

  // Notify the event channel...
  this->event_channel_->disconnected (this);

  if (this->event_channel_->disconnect_callbacks ())
    {
      try
        {
          supplier->disconnect_pull_supplier ();
        }
      catch (const CORBA::Exception&)
        {
          // Ignore exceptions, we must isolate other clients from
          // failures on this one.
        }
    }
}

PortableServer::POA_ptr
TAO_CEC_ProxyPullConsumer::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPullConsumer::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPullConsumer::_remove_ref (void)
{
  this->_decr_refcnt ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
