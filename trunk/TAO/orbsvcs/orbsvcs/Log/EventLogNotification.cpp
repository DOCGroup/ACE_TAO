// $Id$

#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/EventLogNotification.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EventLogNotification::TAO_EventLogNotification (CosEventChannelAdmin::EventChannel_ptr ec)
: TAO_LogNotification (), event_channel_ (CosEventChannelAdmin::EventChannel::_duplicate (ec))
{
  obtain_proxy_consumer ();
}

TAO_EventLogNotification::~TAO_EventLogNotification (void)
{
  // No-Op.
}

void
TAO_EventLogNotification::disconnect_push_supplier (void)
{
  this->consumer_->disconnect_push_consumer ();
}

void
TAO_EventLogNotification::obtain_proxy_consumer()
{
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
    event_channel_->for_suppliers ();

  consumer_ = supplier_admin->obtain_push_consumer ();

  CosEventComm::PushSupplier_var supplier =
    this->_this ();

  consumer_->connect_push_supplier (supplier.in());
}

void
TAO_EventLogNotification::send_notification (const CORBA::Any& any)
{
  consumer_->push (any);
}

TAO_END_VERSIONED_NAMESPACE_DECL
