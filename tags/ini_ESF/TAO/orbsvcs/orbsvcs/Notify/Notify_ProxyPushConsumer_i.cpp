/* -*- C++ -*- $Id$ */

#include "Notify_ProxyPushConsumer_i.h"
#include "Notify_SupplierAdmin_i.h"

// Implementation skeleton constructor
TAO_Notify_ProxyPushConsumer_i::TAO_Notify_ProxyPushConsumer_i
(TAO_Notify_SupplierAdmin_i &supplieradmin)
  :TAO_Notify_ProxyConsumer_i (supplieradmin)
{
}

// Implementation skeleton destructor
TAO_Notify_ProxyPushConsumer_i::~TAO_Notify_ProxyPushConsumer_i (void)
{
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_ProxyPushConsumer_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

void TAO_Notify_ProxyPushConsumer_i::connect_any_push_supplier (
    CosEventComm::PushSupplier_ptr push_supplier,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ))
  {
    push_supplier_ = CosEventComm::PushSupplier::_duplicate (push_supplier);
  }

void
TAO_Notify_ProxyPushConsumer_i::push (const CORBA::Any& data,
                                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  // Filtering comes here.
  myadmin_.get_dispatcher ().dispatch_event (data, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_ProxyPushConsumer_i::disconnect_push_consumer
(CORBA::Environment & /* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
}
