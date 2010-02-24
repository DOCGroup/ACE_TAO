// $Id$
#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "tao/debug.h"

Notify_Push_Consumer::Notify_Push_Consumer (const char* name,
                                            int sent,
                                            NS_FilterType consumerFilter,
                                            NS_FilterType supplierFilter,
                                            Notify_Test_Client& client)
: name_ (name)
, sent_(sent)
, received_(0)
, expected_(sent)
, client_(client)
, consumerFilter_(consumerFilter)
, supplierFilter_(supplierFilter)
{
  // Calculate the expected number.
  ACE_ASSERT(sent % 9 == 0);

  // The supplier side filters combine group != 0 and type != 0, while the
  // consumer side combines group != 1 and type != 1
  if (consumerFilter == AndOp && supplierFilter == AndOp)
  {
    // group != 0 && type != 0 && group != 1 && type != 1
    expected_ = sent_ / 9;
  }
  else if (consumerFilter == AndOp && supplierFilter == OrOp)
  {
    // group != 0 || type != 0 && group != 1 && type != 1
    expected_ = sent_ * 3 / 9;
  }
  else if (consumerFilter == AndOp && supplierFilter == None)
  {
    // group != 1 && type != 1
    expected_ = sent_ * 4 / 9;
  }
  else if (consumerFilter == OrOp && supplierFilter == AndOp)
  {
    // group != 0 && type != 0 && group != 1 || type != 1
    expected_ = sent_ * 3 / 9;
  }
  else if (consumerFilter == OrOp && supplierFilter == OrOp)
  {
    // group != 0 || type != 0 && group != 1 || type != 1
    expected_ = sent_ * 7 / 9;
  }
  else if (consumerFilter == OrOp && supplierFilter == None)
  {
    // group != 1 || type != 1
    expected_ = sent_ * 8 / 9;
  }
  else if (consumerFilter == None && supplierFilter == OrOp)
  {
    // group != 0 && type != 0
    expected_ = sent_ * 8 / 9;
  }
  else if (consumerFilter == None && supplierFilter == AndOp)
  {
    // group != 0 && type != 0
    expected_ = sent_ * 4 / 9;
  }
  else if (consumerFilter == None && supplierFilter == None)
  {
    expected_ = sent_;
  }
  else
  {
    bool unknown_filter_combination = false;
    ACE_ASSERT(unknown_filter_combination);
    ACE_UNUSED_ARG(unknown_filter_combination);
  }

  this->client_.consumer_start (this);
}

void
Notify_Push_Consumer::_connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                                CosNotifyChannelAdmin::EventChannel_ptr notify_channel)
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
    CosNotifyChannelAdmin::STRUCTURED_EVENT,
    proxy_id_);

  if (consumerFilter_ != None)
  {
    CosNotifyFilter::FilterFactory_var ffact =
      notify_channel->default_filter_factory ();

    CosNotifyFilter::Filter_var filter =
      ffact->create_filter ("ETCL");

    ACE_ASSERT(! CORBA::is_nil (filter.in ()));

    CosNotifyFilter::ConstraintExpSeq constraint_list (1);
    constraint_list.length (1);

    constraint_list[0].event_types.length (0);
    constraint_list[0].constraint_expr = CORBA::string_dup ("$group != 1");

    filter->add_constraints (constraint_list);

    proxysupplier->add_filter (filter.in ());
  }


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

  if (consumerFilter_ == OrOp)
    validate(type != 1 || group != 1);
  else if (consumerFilter_ == AndOp)
    validate(type != 1 && group != 1);

  if (supplierFilter_ == OrOp)
    validate(type != 0 || group != 0);
  else if (supplierFilter_ == OrOp)
    validate(type != 0 && group != 0);

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
