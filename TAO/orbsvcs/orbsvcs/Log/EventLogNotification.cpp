/* -*- C++ -*- $Id$ */

#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/EventLogNotification.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/debug.h"

EventLogNotification::EventLogNotification (CosEventChannelAdmin::EventChannel_ptr ec)
: LogNotification (), event_channel_ (CosEventChannelAdmin::EventChannel::_duplicate (ec))
{
  obtain_proxy_consumer ();
}

EventLogNotification::~EventLogNotification (void)
{
  // No-Op.
}

void
EventLogNotification::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->consumer_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
EventLogNotification::obtain_proxy_consumer()
{  
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin = 
    event_channel_->for_suppliers ();

  consumer_ = supplier_admin->obtain_push_consumer ();

  CosEventComm::PushSupplier_var supplier =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);

  consumer_->connect_push_supplier (supplier.in());
}

void
EventLogNotification::send_notification (const CORBA::Any& any )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  consumer_->push (any);
}


