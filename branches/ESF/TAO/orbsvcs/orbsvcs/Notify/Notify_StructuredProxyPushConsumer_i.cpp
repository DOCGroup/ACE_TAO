/* -*- C++ -*- $Id$ */

#include "Notify_StructuredProxyPushConsumer_i.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_Dispatcher.h"

TAO_Notify_StructuredProxyPushConsumer_i::TAO_Notify_StructuredProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i &supplieradmin)
  :TAO_Notify_ProxyConsumer_i (supplieradmin)
{
}

TAO_Notify_StructuredProxyPushConsumer_i::~TAO_Notify_StructuredProxyPushConsumer_i (void)
{
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_StructuredProxyPushConsumer_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

void
TAO_Notify_StructuredProxyPushConsumer_i::connect_structured_push_supplier (
                                                                            CosNotifyComm::StructuredPushSupplier_ptr push_supplier,
                                                                            CORBA::Environment & //ACE_TRY_ENV
                                                                            )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ))
{
  //@@ incomplete
  push_supplier_ =
    CosNotifyComm::StructuredPushSupplier::_duplicate (push_supplier);
}

void
TAO_Notify_StructuredProxyPushConsumer_i::push_structured_event (
                                                                 const CosNotification::StructuredEvent& event,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ))
{
  //@@ Filtering comes here.
  CORBA::Boolean truth =
    this->match_structured (event, ACE_TRY_ENV);
  ACE_CHECK;

  if (truth == 0)
    return; // don't propogate the event further out.

  myadmin_.get_dispatcher ().dispatch_event (event, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_StructuredProxyPushConsumer_i::disconnect_structured_push_consumer (
                                                                               CORBA::Environment & //ACE_TRY_ENV
                                                                               )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
}
