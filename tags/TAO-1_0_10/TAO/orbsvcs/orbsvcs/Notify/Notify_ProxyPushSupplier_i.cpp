/* -*- C++ -*- $Id$ */
#include "Notify_ProxyPushSupplier_i.h"

// Implementation skeleton constructor
TAO_Notify_ProxyPushSupplier_i::TAO_Notify_ProxyPushSupplier_i
(TAO_Notify_ConsumerAdmin_i &consumeradmin)
  :TAO_Notify_ProxySupplier_i (consumeradmin)
{
}

// Implementation skeleton destructor
TAO_Notify_ProxyPushSupplier_i::~TAO_Notify_ProxyPushSupplier_i (void)
{
}

void
TAO_Notify_ProxyPushSupplier_i::dispatch_event (const CORBA::Any & data,
                                                CORBA::Environment &ACE_TRY_ENV)
{
  // check if it passes filter, disconnected etc.
  push_consumer_->push (data, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_ProxyPushSupplier_i::dispatch_event
(const CosNotification::StructuredEvent& event,
 CORBA::Environment &ACE_TRY_ENV)
{
  // check if it passes filter, disconnected etc.
  // pass the any data.
  push_consumer_->push (event.remainder_of_body, ACE_TRY_ENV);
  ACE_CHECK;
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ProxyPushSupplier_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

void TAO_Notify_ProxyPushSupplier_i::connect_any_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ))
  {
    push_consumer_ = CosEventComm::PushConsumer::_duplicate (push_consumer);
    //Add your implementation here
  }

void TAO_Notify_ProxyPushSupplier_i::suspend_connection (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyInactive,
    CosNotifyChannelAdmin::NotConnected
  ))

  {
    //Add your implementation here
  }

void
TAO_Notify_ProxyPushSupplier_i::resume_connection (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyActive,
    CosNotifyChannelAdmin::NotConnected
  ))

  {
    //Add your implementation here
  }


void
TAO_Notify_ProxyPushSupplier_i::disconnect_push_supplier (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ))
{
}
