#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "tao/debug.h"

#include <cstring>

Notify_Push_Consumer::Notify_Push_Consumer (int sent,
                                            bool use_filter,
                                            Notify_Test_Client& client)
  : expected_ (use_filter ? sent * 4 / 9 : sent),
    use_filter_ (use_filter),
    client_ (client)
{
  // By sending multiples of 9, we ensure that all combinations of group and type
  // are possible, and that our calculations come out evenly.
  ACE_ASSERT (sent % 9 == 0);

  // This test currently only supports one expression
  this->client_.consumer_start (this);
}

void
Notify_Push_Consumer::_connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                                CosNotifyChannelAdmin::EventChannel_ptr)
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

  this->proxy_->connect_structured_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}

static void
validate_expression (bool expr, char const* msg)
{
  if (!expr)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: %C\n"), msg));
}

#define validate(expr) validate_expression(expr, #expr)

void
Notify_Push_Consumer::push_structured_event (
  CosNotification::StructuredEvent const& event)
{
  ACE_DEBUG ((LM_DEBUG, "-"));
  CORBA::Long const received = this->received_.fetch_add (1, std::memory_order_relaxed) + 1;

  CORBA::ULong group {};
  CORBA::ULong type {};

  ACE_ASSERT (event.filterable_data.length () == 3);
  ACE_ASSERT (std::strcmp (event.filterable_data[0].name.in (), "id") == 0);
  ACE_ASSERT (std::strcmp (event.filterable_data[1].name.in (), "group") == 0);
  ACE_ASSERT (std::strcmp (event.filterable_data[2].name.in (), "type") == 0);
  event.filterable_data[1].value >>= group;
  event.filterable_data[2].value >>= type;

  if (this->use_filter_)
    validate (type != 1 && group != 0);

  if (received > this->expected_)
    {
      ACE_ERROR ((LM_ERROR, "\nError: Expected %d, Received %d\n",
                  this->expected_, received));
      this->client_.consumer_done (this);
      return;
    }
  if (received >= this->expected_)
    {
      ACE_DEBUG ((LM_DEBUG, "\nConsumer received %d events.\n", received));
      this->client_.consumer_done (this);
      return;
    }
}
