// $Id$

#include "ace/OS_NS_unistd.h"
#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"
#include "tao/debug.h"

Notify_Push_Consumer::Notify_Push_Consumer (
  Notify_Test_Client& client)
  : client_ ( client )
  , messages_received_( 0 )
{
  this->client_.consumer_start (this);
}

void
Notify_Push_Consumer::_connect (
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
  CosNotifyFilter::Filter_ptr filter)
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
    CosNotifyChannelAdmin::STRUCTURED_EVENT,
    proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
    proxysupplier.in ());

  CosNotification::QoSProperties properties (3);
  properties.length (3);
  properties[0].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long) 3;
  properties[1].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[1].value <<= (TimeBase::TimeT) 1 * 1000 * 10000;

  this->proxy_->set_qos (properties);
  this->proxy_->add_filter (filter);
  this->proxy_->connect_structured_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Push_Consumer::push_structured_event (
  const CosNotification::StructuredEvent& event)
{
  messages_received_++;
  ACE_DEBUG((LM_DEBUG, "{%d} domain = %s\n", messages_received_,
    event.header.fixed_header.event_type.domain_name.in()));
  this->client_.consumer_done (this);
}

int
Notify_Push_Consumer::messages_received()
{
  return messages_received_;
}
