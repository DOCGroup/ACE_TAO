/* -*- C++ -*- */
// $Id$ */

#include "Notify_StructuredProxyPushSupplier_i.h"
#include "Notify_ConsumerAdmin_i.h"

ACE_RCSID(Notify, Notify_StructuredProxyPushSupplier_i, "$Id$")

TAO_Notify_StructuredProxyPushSupplier_i::TAO_Notify_StructuredProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager)
  :structured_proxy_inherited (consumeradmin, resource_manager)
{
}

TAO_Notify_StructuredProxyPushSupplier_i::~TAO_Notify_StructuredProxyPushSupplier_i (void)
{
}

void
TAO_Notify_StructuredProxyPushSupplier_i::cleanup_i (CORBA::Environment &ACE_TRY_ENV)
{
  structured_proxy_inherited::cleanup_i (ACE_TRY_ENV);

  this->push_consumer_ = CosNotifyComm::StructuredPushConsumer::_nil ();
}

void
TAO_Notify_StructuredProxyPushSupplier_i::connect_structured_push_consumer (CosNotifyComm::StructuredPushConsumer_ptr push_consumer, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected,
                   CosEventChannelAdmin::TypeError
                   ))
{
  if (CORBA::is_nil (push_consumer))
    ACE_THROW (CosEventChannelAdmin::TypeError ());
  else if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    {
      this->push_consumer_ =
        CosNotifyComm::StructuredPushConsumer::_duplicate (push_consumer);
    }

  ACE_TRY
    {
      this->on_connected (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->is_connected_ = 1;
    }
  ACE_CATCHALL
    {
     this->push_consumer_ =
       CosNotifyComm::StructuredPushConsumer::_nil ();
     ACE_RETHROW;
    }
 ACE_ENDTRY;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::dispatch_event_i (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      event.do_push (this->push_consumer_.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->push_consumer_->offer_change (added, removed, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::disconnect_structured_push_supplier(
                                                                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->is_destroyed_ = 1;

  // ask our parent to deactivate us.
  this->myadmin_->
    deactivate_proxy_pushsupplier (this, ACE_TRY_ENV);

  this->cleanup_i (ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>

#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
