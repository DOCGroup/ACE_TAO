// $Id$

#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/NotifyLogNotification.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NotifyLogNotification::TAO_NotifyLogNotification (
  CosNotifyChannelAdmin::EventChannel_ptr ec)
  : TAO_LogNotification (),
    event_channel_ (CosNotifyChannelAdmin::EventChannel::_duplicate (ec))
{

  CosNotifyComm::PushSupplier_var objref =
    this->_this ();
  ACE_ASSERT (!CORBA::is_nil (objref.in ()));

  CosNotifyChannelAdmin::AdminID adminID;
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel_->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, adminID);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin.in ()));

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::ANY_EVENT, proxy_consumer_id_);

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::ProxyPushConsumer::_narrow (proxyconsumer.in ());

  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  proxy_consumer_->connect_any_push_supplier (objref.in ());
}

TAO_NotifyLogNotification::~TAO_NotifyLogNotification (void)
{
  // No-Op.
}

void
TAO_NotifyLogNotification::send_notification (const CORBA::Any& any)
{
  this->proxy_consumer_->push (any);
}

void
TAO_NotifyLogNotification::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */)
{
  //No-Op.
}

void
TAO_NotifyLogNotification::disconnect_push_supplier (void)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_push_consumer();
}

TAO_END_VERSIONED_NAMESPACE_DECL
