/* -*- C++ -*- */
// $Id$ */

#include "Notify_StructuredProxyPushConsumer_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"

ACE_RCSID(Notify, Notify_StructuredProxyPushConsumer_i, "$Id$")

TAO_Notify_StructuredProxyPushConsumer_i::TAO_Notify_StructuredProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmin, TAO_Notify_Resource_Manager* resource_manager)
  :structured_proxy_inherited (supplieradmin, resource_manager)
{
}

TAO_Notify_StructuredProxyPushConsumer_i::~TAO_Notify_StructuredProxyPushConsumer_i (void)
{
}

void
TAO_Notify_StructuredProxyPushConsumer_i::cleanup_i (CORBA::Environment& ACE_TRY_ENV)
{
  structured_proxy_inherited::cleanup_i (ACE_TRY_ENV);

  this->is_destroyed_ = 1;
}

void
TAO_Notify_StructuredProxyPushConsumer_i::connect_structured_push_supplier (CosNotifyComm::StructuredPushSupplier_ptr push_supplier, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected
                   ))
{
  if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    this->push_supplier_ =
      CosNotifyComm::StructuredPushSupplier::_duplicate (push_supplier);

  this->event_manager_->register_for_subscription_updates (this, ACE_TRY_ENV);
  ACE_CHECK;

  this->is_connected_ = 1;
}

void
TAO_Notify_StructuredProxyPushConsumer_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->push_supplier_->subscription_change (added, removed,
                                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_StructuredProxyPushConsumer_i::push_structured_event (const CosNotification::StructuredEvent & notification, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  if (this->is_connected_ == 0)
    ACE_THROW (CosEventComm::Disconnected ());

  TAO_Notify_StructuredEvent notify_event (notification);

  CORBA::Boolean bval = this->check_filters_i (notify_event, ACE_TRY_ENV);
  ACE_CHECK;

  if (bval == 1)
    this->event_manager_->push (notify_event, ACE_TRY_ENV);
}

void
TAO_Notify_StructuredProxyPushConsumer_i::disconnect_structured_push_consumer (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->is_destroyed_ = 1;

  // ask our parent to deaactivate us.
  this->myadmin_->
    deactivate_proxy_pushconsumer (this, ACE_TRY_ENV);

  this->cleanup_i (ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
