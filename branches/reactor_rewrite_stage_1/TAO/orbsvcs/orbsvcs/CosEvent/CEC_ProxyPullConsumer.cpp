// $Id$

#include "CEC_ProxyPullConsumer.h"
#include "CEC_EventChannel.h"
#include "CEC_ConsumerAdmin.h"
#include "CEC_SupplierControl.h"
#include "CEC_ProxyPullSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_ProxyPullConsumer.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_ProxyPullConsumer, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

TAO_CEC_ProxyPullConsumer::
    TAO_CEC_ProxyPullConsumer (TAO_CEC_EventChannel* ec)
  : event_channel_ (ec),
    refcount_ (1)
{
  this->lock_ =
    this->event_channel_->create_consumer_lock ();

  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_CEC_ProxyPullConsumer::~TAO_CEC_ProxyPullConsumer (void)
{
  this->event_channel_->destroy_consumer_lock (this->lock_);
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_ProxyPullConsumer::activate (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventChannelAdmin::ProxyPullConsumer_var result;
  ACE_TRY
    {
      result = this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return CosEventChannelAdmin::ProxyPullConsumer::_nil ();
    }
  ACE_ENDTRY;
  return result._retn ();
}

void
TAO_CEC_ProxyPullConsumer::deactivate (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      PortableServer::POA_var poa =
        this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
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

// NOTE: There is some amount of duplicated code here, but it is
// intentional. Mainly we want to avoid locking overhead when
// possible, thus the code flow is optimized for that case more than
// for small code.

CORBA::Any*
TAO_CEC_ProxyPullConsumer::try_pull_from_supplier (
    CORBA::Boolean_out has_event
    TAO_ENV_ARG_DECL)
{
  has_event = 0;
  CosEventComm::PullSupplier_var supplier;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK_RETURN (0);

    if (this->is_connected_i () == 0)
      return 0; // ACE_THROW (CosEventComm::Disconnected ());????

    supplier =
      CosEventComm::PullSupplier::_duplicate (this->supplier_.in ());

    // The refcount cannot be zero, because we have at least two
    // references,
  }

  CORBA::Any_var any;
  ACE_TRY
    {
      any = supplier->try_pull (has_event TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      TAO_CEC_SupplierControl *control =
        this->event_channel_->supplier_control ();

      control->supplier_not_exist (this TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      TAO_CEC_SupplierControl *control =
        this->event_channel_->supplier_control ();

      control->system_exception (this,
                                 sysex
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ Should not happen
    }
  ACE_ENDTRY;
  return any._retn ();
}

CORBA::Any*
TAO_CEC_ProxyPullConsumer::pull_from_supplier (
    TAO_ENV_SINGLE_ARG_DECL)
{
  CosEventComm::PullSupplier_var supplier;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK_RETURN (0);

    if (this->is_connected_i () == 0)
      return 0; // ACE_THROW (CosEventComm::Disconnected ());????

    supplier =
      CosEventComm::PullSupplier::_duplicate (this->supplier_.in ());

    // The refcount cannot be zero, because we have at least two
    // references,
  }

  CORBA::Any_var any;
  ACE_TRY
    {
      any = supplier->pull (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ This is where the policies for misbehaving suppliers
      //    should kick in.... for the moment just ignore them.
    }
  ACE_ENDTRY;
  return any._retn ();
}

CORBA::Boolean
TAO_CEC_ProxyPullConsumer::supplier_non_existent (
      CORBA::Boolean_out disconnected
      TAO_ENV_ARG_DECL)
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
  return this->supplier_->_non_existent (TAO_ENV_SINGLE_ARG_PARAMETER);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_ProxyPullConsumer::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  CosEventComm::PullSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    supplier = this->supplier_._retn ();
  }

  this->deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (supplier.in ()))
    return;

  ACE_TRY
    {
      supplier->disconnect_pull_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
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
      CosEventComm::PullSupplier_ptr pull_supplier
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected))
{
  // Nil PullSuppliers are illegal
  if (CORBA::is_nil (pull_supplier))
    ACE_THROW (CORBA::BAD_PARAM ());

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

        // @@ Please read the comments in CEC_ProxyPullSupplier about
        //     possible race conditions in this area...
        TAO_CEC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_CEC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->disconnected (this TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

        // What if a second thread connected us after this?
        if (this->is_connected_i ())
          return;
      }
    this->supplier_ =
      CosEventComm::PullSupplier::_duplicate (pull_supplier);
  }

  // Notify the event channel...
  this->event_channel_->connected (this TAO_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ProxyPullConsumer::disconnect_pull_consumer (
      TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventComm::PullSupplier_var supplier;

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
  this->event_channel_->disconnected (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->event_channel_->disconnect_callbacks ())
    {
      ACE_TRY
        {
          supplier->disconnect_pull_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
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
TAO_CEC_ProxyPullConsumer::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPullConsumer::_add_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPullConsumer::_remove_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}
