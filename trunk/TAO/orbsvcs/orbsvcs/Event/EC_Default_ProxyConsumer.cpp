// $Id$

#include "orbsvcs/Event/EC_Default_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_Supplier_Filter_Builder.h"
#include "orbsvcs/Event/EC_Supplier_Filter.h"
#include "ace/Reverse_Lock_T.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Reverse_Lock<ACE_Lock> TAO_EC_Unlock;

TAO_EC_Default_ProxyPushConsumer::
    TAO_EC_Default_ProxyPushConsumer (TAO_EC_Event_Channel_Base* ec)
  : TAO_EC_ProxyPushConsumer (ec)
{
}

TAO_EC_Default_ProxyPushConsumer::~TAO_EC_Default_ProxyPushConsumer (void)
{
}

void
TAO_EC_Default_ProxyPushConsumer::connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      const RtecEventChannelAdmin::SupplierQOS& qos)
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      {
        if (this->event_channel_->supplier_reconnect () == 0)
          throw RtecEventChannelAdmin::AlreadyConnected ();

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

          this->event_channel_->reconnected (this);
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
    ACE_DEBUG ((LM_DEBUG,
                 "Building filter for supplier <%x>.\n",
                this));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
    this->filter_ =
      this->event_channel_->supplier_filter_builder ()->create (this->qos_);
    this->filter_->bind (this);
  }

  // Notify the event channel...
  this->event_channel_->connected (this);
}

void
TAO_EC_Default_ProxyPushConsumer::push (const RtecEventComm::EventSet& event)
{
  TAO_EC_ProxyPushConsumer_Guard ace_mon (this->lock_,
                                          this->refcount_,
                                          this->event_channel_,
                                          this);
  if (!ace_mon.locked ())
    return;

  ace_mon.filter->push (event, this);
}

void
TAO_EC_Default_ProxyPushConsumer::disconnect_push_consumer ()
{
  RtecEventComm::PushSupplier_var supplier;
  int connected = 0;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    connected = this->is_connected_i ();
    supplier = this->supplier_._retn ();
    this->connected_ = 0;

    if (connected)
      this->cleanup_i ();
  }

  // Notify the event channel...
  this->event_channel_->disconnected (this);

  if (CORBA::is_nil (supplier.in ()))
    {
      return;
    }

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
TAO_EC_Default_ProxyPushConsumer::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_Default_ProxyPushConsumer::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_EC_Default_ProxyPushConsumer::_remove_ref (void)
{
  this->_decr_refcnt ();
}

void
TAO_EC_Default_ProxyPushConsumer::activate (
   RtecEventChannelAdmin::ProxyPushConsumer_ptr &proxy)
{
  proxy = this->_this ();
}

PortableServer::ObjectId
TAO_EC_Default_ProxyPushConsumer::object_id (void)
{
  PortableServer::ObjectId_var result =
    this->default_POA_->servant_to_id (this);
  return result.in ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
