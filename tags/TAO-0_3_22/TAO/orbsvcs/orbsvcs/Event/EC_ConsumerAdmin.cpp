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
                                            TAO_EC_ProxyPushSupplier_Set* ss)
  :  event_channel_ (ec),
     supplier_set_ (ss)
{
  if (this->supplier_set_ == 0)
    {
      this->supplier_set_ =
        this->event_channel_->create_proxy_push_supplier_set ();
    }
  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_EC_ConsumerAdmin::~TAO_EC_ConsumerAdmin (void)
{
  this->event_channel_->destroy_proxy_push_supplier_set (this->supplier_set_);
  this->supplier_set_ = 0;
}

void
TAO_EC_ConsumerAdmin::connected (TAO_EC_ProxyPushConsumer *consumer,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (TAO_EC_ConsumerAdmin::Busy_Lock,
      ace_mon, this->busy_lock (),
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  SupplierSetIterator end = this->end ();
  for (SupplierSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->connected (consumer, ACE_TRY_ENV);
      ACE_CHECK;
      consumer->connected (*i, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_ConsumerAdmin::disconnected (TAO_EC_ProxyPushConsumer *consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (TAO_EC_ConsumerAdmin::Busy_Lock,
      ace_mon, this->busy_lock (),
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  SupplierSetIterator end = this->end ();
  for (SupplierSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->disconnected (consumer, ACE_TRY_ENV);
      ACE_CHECK;
      consumer->disconnected (*i, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_ConsumerAdmin::connected (TAO_EC_ProxyPushSupplier *supplier,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_set_->connected (supplier, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::disconnected (TAO_EC_ProxyPushSupplier *supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_set_->disconnected (supplier, ACE_TRY_ENV);
}

void
TAO_EC_ConsumerAdmin::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW_EX (TAO_EC_ConsumerAdmin::Busy_Lock,
      ace_mon, this->busy_lock (),
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    SupplierSetIterator end = this->end ();
    for (SupplierSetIterator i = this->begin ();
         i != end;
         ++i)
      {
        ACE_TRY
          {
            (*i)->shutdown (ACE_TRY_ENV);
            ACE_TRY_CHECK;
          }
        ACE_CATCHANY
          {
            /* ignore all exceptions */
          }
        ACE_ENDTRY;
      }
  }
  this->supplier_set_->shutdown (ACE_TRY_ENV);
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EC_ConsumerAdmin::obtain_push_supplier (CORBA::Environment &ACE_TRY_ENV)
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
