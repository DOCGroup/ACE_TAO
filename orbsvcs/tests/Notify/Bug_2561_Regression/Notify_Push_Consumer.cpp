// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/TimeBaseC.h"

// ******************************************************************
// Data Section
// ******************************************************************

// This should be less than or equal to the value of num_events in
// Supplier.cpp
static const int max_events = 70;
static const TimeBase::TimeT PACING = 1000;

// ******************************************************************
// Subroutine Section
// ******************************************************************

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
  Notify_Test_Client& client)
  : count_ (0),
    client_ (client)
{
  this->client_.consumer_start (this);
}


void
Notify_Sequence_Push_Consumer::_connect (
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  CosNotifyComm::SequencePushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
    CosNotifyChannelAdmin::SEQUENCE_EVENT,
    proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (
    proxysupplier.in ());

  CosNotification::QoSProperties properties (1);
  properties.length (1);
  properties[0].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[0].value <<= PACING;

  this->proxy_->set_qos (properties);
  this->proxy_->connect_sequence_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Sequence_Push_Consumer::push_structured_events (
  const CosNotification::EventBatch& events)
{
  this->count_ += events.length ();
  if (this->count_ >= max_events)
    {
      this->client_.consumer_done (this);
    }
}
