// $Id$

#include "EC_ConsumerAdmin.h"
#include "EC_ProxySupplier.h"
#include "EC_ProxyConsumer.h"
#include "EC_Event_Channel.h"
#include "EC_Command.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ConsumerAdmin, "$Id$")

TAO_EC_ConsumerAdmin::TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel *ec,
                                            Collection *collection)
  :  event_channel_ (ec),
     collection_ (collection)
{
  if (this->collection_ == 0)
    {
      this->collection_ =
        this->event_channel_->create_proxy_push_supplier_collection ();

      // @@
      // @@ this->collection_->busy_hwm (this->event_channel_->busy_hwm ());
      // @@ this->collection_->max_write_delay (
      // @@           this->event_channel_->max_write_delay ()
      // @@ );
    }
  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_EC_ConsumerAdmin::~TAO_EC_ConsumerAdmin (void)
{
  this->event_channel_->destroy_proxy_push_supplier_collection (this->collection_);
  this->collection_ = 0;
}

void
TAO_EC_ConsumerAdmin::connected (TAO_EC_ProxyPushConsumer *consumer,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Connect_Consumer worker (consumer);

  this->collection_->for_each (&worker, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::reconnected (TAO_EC_ProxyPushConsumer *consumer,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Reconnect_Consumer worker (consumer);

  this->collection_->for_each (&worker, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::disconnected (TAO_EC_ProxyPushConsumer *consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Disconnect_Consumer worker (consumer);

  this->collection_->for_each (&worker, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::connected (TAO_EC_ProxyPushSupplier *supplier,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->connected (supplier, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::reconnected (TAO_EC_ProxyPushSupplier *supplier,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->reconnected (supplier, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::disconnected (TAO_EC_ProxyPushSupplier *supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->disconnected (supplier, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Shutdown_Supplier worker;

  this->collection_->for_each (&worker, ACE_TRY_ENV);
  this->collection_->shutdown (ACE_TRY_ENV);
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EC_ConsumerAdmin::obtain_push_supplier (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_EC_ProxyPushSupplier* supplier =
    this->event_channel_->create_proxy_push_supplier ();

  return supplier->_this (ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_EC_ConsumerAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

// ****************************************************************

void
TAO_EC_Connect_Consumer::work (TAO_EC_ProxyPushSupplier *supplier,
                               CORBA::Environment &ACE_TRY_ENV)
{
  supplier->connected (this->consumer_, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

// ****************************************************************

void
TAO_EC_Reconnect_Consumer::work (TAO_EC_ProxyPushSupplier *supplier,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  supplier->reconnected (this->consumer_, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_->reconnected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

// ****************************************************************

void
TAO_EC_Disconnect_Consumer::work (TAO_EC_ProxyPushSupplier *supplier,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  supplier->disconnected (this->consumer_, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_->disconnected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

// ****************************************************************

void
TAO_EC_Shutdown_Supplier::work (TAO_EC_ProxyPushSupplier *supplier,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      supplier->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions
    }
  ACE_ENDTRY;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_EC_Worker<TAO_EC_ProxyPushSupplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_EC_Worker<TAO_EC_ProxyPushSupplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
