// $Id$
#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "tao/debug.h"

Notify_Push_Consumer::Notify_Push_Consumer (const char* name,
                                            int sent,
                                            bool useFilter,
                                            Notify_Test_Client& client)
: name_ (name)
, sent_(sent)
, received_(0)
, expected_(sent)
, useFilter_(useFilter)
, client_(client)
{
  // By sending multiples of 9, we ensure that all combinations of group and type
  // are possible, and that our calculations come out evenly.
  ACE_ASSERT(sent % 9 == 0);

  // This test currently only supports one expression
  if (useFilter)
  {
    // group != 0 && type != 1
    expected_ = sent_ * 4 / 9;
  }

  this->client_.consumer_start (this);
}

void
Notify_Push_Consumer::_connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                                CosNotifyChannelAdmin::EventChannel_ptr notify_channel)
{
  ACE_UNUSED_ARG(notify_channel);
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
    CosNotifyChannelAdmin::STRUCTURED_EVENT,
    proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
    proxysupplier.in ());

  this->proxy_->connect_structured_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}

static void validate_expression(bool expr, const char* msg)
{
  if (! expr)
  {
    ACE_ERROR((LM_ERROR, "Error: %s\n", msg));
  }
}

#define validate(expr) validate_expression(expr, #expr)

void
Notify_Push_Consumer::push_structured_event (
  const CosNotification::StructuredEvent& event)
{
  ACE_DEBUG((LM_DEBUG, "-"));
  received_++;

  CORBA::ULong id = 0;
  CORBA::ULong group = 0;
  CORBA::ULong type = 0;

  ACE_ASSERT(event.filterable_data.length() == 3);
  ACE_ASSERT(ACE_OS::strcmp(event.filterable_data[0].name.in(), "id") == 0);
  ACE_ASSERT(ACE_OS::strcmp(event.filterable_data[1].name.in(), "group") == 0);
  ACE_ASSERT(ACE_OS::strcmp(event.filterable_data[2].name.in(), "type") == 0);
  event.filterable_data[0].value >>= id;
  event.filterable_data[1].value >>= group;
  event.filterable_data[2].value >>= type;

  if (useFilter_)
    validate(type != 1 && group != 0);

  if (received_ > expected_)
  {
    ACE_ERROR((LM_ERROR, "\nError: Expected %d, Received %d\n", expected_, received_));
    this->client_.consumer_done (this);
    return;
  }
  if (received_ >= expected_)
  {
    ACE_DEBUG((LM_DEBUG, "\nConsumer received %d events.\n", received_));
    this->client_.consumer_done (this);
    return;
  }
}
