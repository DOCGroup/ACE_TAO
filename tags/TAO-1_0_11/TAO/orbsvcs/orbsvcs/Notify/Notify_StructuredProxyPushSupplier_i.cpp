/* -*- C++ -*- $Id$ */

#include "Notify_StructuredProxyPushSupplier_i.h"

TAO_Notify_StructuredProxyPushSupplier_i::TAO_Notify_StructuredProxyPushSupplier_i
(TAO_Notify_ConsumerAdmin_i &consumeradmin)
  :TAO_Notify_ProxySupplier_i (consumeradmin)
{
}

// Implementation skeleton destructor
TAO_Notify_StructuredProxyPushSupplier_i::~TAO_Notify_StructuredProxyPushSupplier_i (void)
{
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_StructuredProxyPushSupplier_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

void
TAO_Notify_StructuredProxyPushSupplier_i::dispatch_event
(const CORBA::Any & data,
 CORBA::Environment &ACE_TRY_ENV)
{
  CosNotification::StructuredEvent event;
  event.remainder_of_body = data;

  push_consumer_->push_structured_event (event, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::dispatch_event
(const CosNotification::StructuredEvent& event,
 CORBA::Environment &ACE_TRY_ENV)
{
  //@@ check if it passes filter, disconnected etc.
  CORBA::Boolean truth =
    this->match_structured (event, ACE_TRY_ENV);
  ACE_CHECK;

  if (truth == 0)
    return; // don't propogate the event further out.

  push_consumer_->push_structured_event (event, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::connect_structured_push_consumer (
                                                                            CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
                                                                            CORBA::Environment & //ACE_TRY_ENV
                                                                            )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ))
{
  push_consumer_ =
    CosNotifyComm::StructuredPushConsumer::_duplicate (push_consumer);
}

void
TAO_Notify_StructuredProxyPushSupplier_i::disconnect_structured_push_supplier
(CORBA::Environment & /*ACE_TRY_ENV */)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
}

void
TAO_Notify_StructuredProxyPushSupplier_i::suspend_connection (
                                                              CORBA::Environment & //ACE_TRY_ENV
                                                              )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyInactive,
    CosNotifyChannelAdmin::NotConnected
  ))
{
}

void
TAO_Notify_StructuredProxyPushSupplier_i:: resume_connection (
                                                              CORBA::Environment & //ACE_TRY_ENV
                                                              )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyActive,
    CosNotifyChannelAdmin::NotConnected
  ))
{
}
