// $Id$

#include "EC_ProxyConsumer.h"
#include "EC_Event_Channel.h"
#include "EC_Supplier_Filter.h"
#include "EC_Supplier_Filter_Builder.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxyConsumer, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_EC_Unlock;

TAO_EC_ProxyPushConsumer::
    TAO_EC_ProxyPushConsumer (TAO_EC_Event_Channel* ec)
  : event_channel_ (ec),
    refcount_ (1),
    connected_ (0),
    filter_ (0)
{
  this->lock_ =
    this->event_channel_->create_consumer_lock ();

  this->default_POA_ =
    this->event_channel_->consumer_poa ();

  this->qos_.is_gateway = 0;
}

TAO_EC_ProxyPushConsumer::~TAO_EC_ProxyPushConsumer (void)
{
  this->event_channel_->destroy_consumer_lock (this->lock_);
  this->cleanup_i ();
}


CORBA::Boolean
TAO_EC_ProxyPushConsumer::supplier_non_existent (
      CORBA::Boolean_out disconnected
      TAO_ENV_ARG_DECL)
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
  return supplier->_non_existent (TAO_ENV_SINGLE_ARG_PARAMETER);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_EC_ProxyPushConsumer::connected (TAO_EC_ProxyPushSupplier* supplier
                                     TAO_ENV_ARG_DECL)
{
  TAO_EC_ProxyPushConsumer_Guard ace_mon (this->lock_,
                                          this->refcount_,
                                          this->event_channel_,
                                          this);
  if (!ace_mon.locked ())
    return;

  ace_mon.filter->connected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_EC_ProxyPushConsumer::reconnected (TAO_EC_ProxyPushSupplier* supplier
                                       TAO_ENV_ARG_DECL)
{
  TAO_EC_ProxyPushConsumer_Guard ace_mon (this->lock_,
                                          this->refcount_,
                                          this->event_channel_,
                                          this);
  if (!ace_mon.locked ())
    return;

  ace_mon.filter->reconnected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_EC_ProxyPushConsumer::disconnected (TAO_EC_ProxyPushSupplier* supplier
                                        TAO_ENV_ARG_DECL)
{
  TAO_EC_ProxyPushConsumer_Guard ace_mon (this->lock_,
                                          this->refcount_,
                                          this->event_channel_,
                                          this);
  if (!ace_mon.locked ())
    return;

  ace_mon.filter->disconnected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_EC_ProxyPushConsumer::connected (TAO_EC_ProxyPushConsumer*
                                     TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_ProxyPushConsumer::reconnected (TAO_EC_ProxyPushConsumer*
                                       TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_ProxyPushConsumer::disconnected (TAO_EC_ProxyPushConsumer*
                                        TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_ProxyPushConsumer::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  RtecEventComm::PushSupplier_var supplier;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    supplier = this->supplier_._retn ();
    this->connected_ = 0;

    if (this->filter_ != 0)
      {
        this->filter_->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        this->cleanup_i ();
      }
  }

  this->deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (supplier.in ()))
    return;

  ACE_TRY
    {
      supplier->disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
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
TAO_EC_ProxyPushConsumer::cleanup_i (void)
{
  this->supplier_ =
    RtecEventComm::PushSupplier::_nil ();
  this->connected_ = 0;

  if (this->filter_ != 0)
    {
      this->filter_->unbind (this);
      this->filter_->_decr_refcnt ();
      this->filter_ = 0;
    }
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
TAO_EC_ProxyPushConsumer::activate (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventChannelAdmin::ProxyPushConsumer_var result;
  ACE_TRY
    {
      result =
        this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
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
TAO_EC_ProxyPushConsumer::deactivate (TAO_ENV_SINGLE_ARG_DECL)
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

CORBA::ULong
TAO_EC_ProxyPushConsumer::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_EC_ProxyPushConsumer::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Use the event channel
  this->event_channel_->destroy_proxy (this);
  return 0;
}

void
TAO_EC_ProxyPushConsumer::connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      const RtecEventChannelAdmin::SupplierQOS& qos
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannelAdmin::AlreadyConnected))
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      {
        if (this->event_channel_->supplier_reconnect () == 0)
          ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

        // Re-connections are allowed, go ahead and disconnect the
        // consumer...
        this->cleanup_i ();

        // @@ Please read the comments in EC_ProxySuppliers about
        //     possible race conditions in this area...
        TAO_EC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_EC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->reconnected (this TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

        // A separate thread could have connected siomultaneously,
        // this is probably an application error, handle it as
        // gracefully as possible
        if (this->is_connected_i ())
          return; // @@ Should we throw
      }

    this->supplier_ =
      RtecEventComm::PushSupplier::_duplicate (push_supplier);
    this->connected_ = 1;
    this->qos_ = qos;

#if TAO_EC_ENABLE_DEBUG_MESSAGES
    ACE_DEBUG ((LM_DEBUG, "Building filter for supplier <%x>\n",
                this));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
    this->filter_ =
      this->event_channel_->supplier_filter_builder ()->create (this->qos_);
    this->filter_->bind (this);
  }

  // Notify the event channel...
  this->event_channel_->connected (this TAO_ENV_ARG_PARAMETER);
}

void
TAO_EC_ProxyPushConsumer::push (const RtecEventComm::EventSet& event
                                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_EC_ProxyPushConsumer_Guard ace_mon (this->lock_,
                                          this->refcount_,
                                          this->event_channel_,
                                          this);
  if (!ace_mon.locked ())
    return;

  // No need to keep the lock, the filter_ class is supposed to be
  // thread safe....
  ace_mon.filter->push (event, this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_ProxyPushConsumer::disconnect_push_consumer (
      TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventComm::PushSupplier_var supplier;
  int connected = 0;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    connected = this->is_connected_i ();
    supplier = this->supplier_._retn ();
    this->connected_ = 0;

    if (connected)
      this->cleanup_i ();
  }

  // Notify the event channel...
  this->event_channel_->disconnected (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (supplier.in ()))
    {
      return;
    }

  if (this->event_channel_->disconnect_callbacks ())
    {
      ACE_TRY
        {
          supplier->disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
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
TAO_EC_ProxyPushConsumer::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_ProxyPushConsumer::_add_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_EC_ProxyPushConsumer::_remove_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

// ****************************************************************

TAO_EC_ProxyPushConsumer_Guard::
    TAO_EC_ProxyPushConsumer_Guard (ACE_Lock *lock,
                                    CORBA::ULong &refcount,
                                    TAO_EC_Event_Channel *ec,
                                    TAO_EC_ProxyPushConsumer *proxy)
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

  this->filter = this->proxy_->filter_i ();
  this->filter->_incr_refcnt ();

  this->locked_ = 1;
  this->refcount_++;
}

TAO_EC_ProxyPushConsumer_Guard::
    ~TAO_EC_ProxyPushConsumer_Guard (void)
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

    this->filter->_decr_refcnt ();

    this->refcount_--;
    if (this->refcount_ != 0)
      return;
  }
  this->event_channel_->destroy_proxy (this->proxy_);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
