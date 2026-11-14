#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "tao/debug.h"

#include <cstring>

static CORBA::Long
expected_event_count (int sent, NS_FilterType consumer_filter, NS_FilterType supplier_filter)
{
  // By sending multiples of 9, all group/type combinations are covered evenly.
  ACE_ASSERT (sent % 9 == 0);

  if (consumer_filter == NS_FilterType::and_op && supplier_filter == NS_FilterType::and_op)
    return sent / 9;
  if (consumer_filter == NS_FilterType::and_op && supplier_filter == NS_FilterType::or_op)
    return sent * 3 / 9;
  if (consumer_filter == NS_FilterType::and_op && supplier_filter == NS_FilterType::none)
    return sent * 4 / 9;
  if (consumer_filter == NS_FilterType::or_op && supplier_filter == NS_FilterType::and_op)
    return sent * 3 / 9;
  if (consumer_filter == NS_FilterType::or_op && supplier_filter == NS_FilterType::or_op)
    return sent * 7 / 9;
  if (consumer_filter == NS_FilterType::or_op && supplier_filter == NS_FilterType::none)
    return sent * 8 / 9;
  if (consumer_filter == NS_FilterType::none && supplier_filter == NS_FilterType::or_op)
    return sent * 8 / 9;
  if (consumer_filter == NS_FilterType::none && supplier_filter == NS_FilterType::and_op)
    return sent * 4 / 9;

  return sent;
}

Notify_Push_Consumer::Notify_Push_Consumer (int sent,
                                            NS_FilterType consumer_filter,
                                            NS_FilterType supplier_filter,
                                            Notify_Test_Client& client)
  : expected_ (expected_event_count (sent, consumer_filter, supplier_filter)),
    client_ (client),
    consumer_filter_ (consumer_filter),
    supplier_filter_ (supplier_filter)
{
  this->client_.consumer_start (this);
}

void
Notify_Push_Consumer::_connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                                CosNotifyChannelAdmin::EventChannel_ptr notify_channel)
{
  CosNotifyComm::StructuredPushConsumer_var objref = this->_this ();
  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_id_);

  if (this->consumer_filter_ != NS_FilterType::none)
    {
      CosNotifyFilter::FilterFactory_var ffact = notify_channel->default_filter_factory ();
      CosNotifyFilter::Filter_var filter = ffact->create_filter ("ETCL");
      ACE_ASSERT (!CORBA::is_nil (filter.in ()));

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);
      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup ("$group != 1");

      filter->add_constraints (constraint_list);
      proxysupplier->add_filter (filter.in ());
    }

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxysupplier.in ());
  this->proxy_->connect_structured_push_consumer (objref.in ());

  // The POA owns the activated servant.
  this->_remove_ref ();
}

static void
validate_expression (bool expr, char const* msg)
{
  if (!expr)
    ACE_ERROR ((LM_ERROR, "Error: %s\n", msg));
}

#define validate(expr) validate_expression (expr, #expr)

void
Notify_Push_Consumer::push_structured_event (CosNotification::StructuredEvent const& event)
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

  if (this->consumer_filter_ == NS_FilterType::or_op)
    validate (type != 1 || group != 1);
  else if (this->consumer_filter_ == NS_FilterType::and_op)
    validate (type != 1 && group != 1);

  if (this->supplier_filter_ == NS_FilterType::or_op)
    validate (type != 0 || group != 0);
  else if (this->supplier_filter_ == NS_FilterType::and_op)
    validate (type != 0 && group != 0);

  if (received > this->expected_)
    {
      ACE_ERROR ((LM_ERROR, "\nError: Expected %d, Received %d\n", this->expected_, received));
      this->client_.consumer_done (this);
      return;
    }
  if (received >= this->expected_)
    {
      ACE_DEBUG ((LM_DEBUG, "\nConsumer received %d events.\n", received));
      this->client_.consumer_done (this);
    }
}
