/* -*- C++ -*-  */

#include "Filter.h"

constexpr char notify_factory_name[] = "NotifyEventChannelFactory";
constexpr char naming_service_name[] = "NameService";
constexpr char consumer_admin_filter[] = "threshold < 20";
constexpr char supplier_admin_filter_1[] = "threshold > 10";
constexpr char supplier_admin_filter_2[] = "'pc1' in targetlist";
constexpr char tcl_grammar[] = "TCL";
constexpr CORBA::ULong events_to_send = 30;
constexpr CORBA::ULong events_expected_to_receive = 9 * 4;

static std::atomic<CORBA::ULong> result_count {};

FilterClient::~FilterClient ()
{
  this->ec_->destroy ();
}

void
FilterClient::init (int argc, ACE_TCHAR *argv [])
{
  init_ORB (argc, argv);

  resolve_naming_service ();

  resolve_Notify_factory ();

  create_EC ();

  create_supplieradmin ();

  create_consumeradmin ();

  create_consumers ();

  create_suppliers ();
}

void
FilterClient::run ()
{
  send_events ();

  if (result_count.load () != events_expected_to_receive)
    { // if we still need to wait for events, run the orb.
      while (!this->done_.load ())
        if (this->orb_->work_pending ())
          this->orb_->perform_work ();
    }
}

void
FilterClient::done ()
{
  this->done_.store (true);
}

void
FilterClient::init_ORB (int argc, ACE_TCHAR *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  CORBA::Object_var const poa_object =
    this->orb_->resolve_initial_references ("RootPOA");

  if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")));
      return;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var const poa_manager =
    root_poa_->the_POAManager ();

  poa_manager->activate ();
}

void
FilterClient::resolve_naming_service ()
{
  CORBA::Object_var const naming_obj =
    this->orb_->resolve_initial_references (naming_service_name);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

void
FilterClient::resolve_Notify_factory ()
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (notify_factory_name);

  CORBA::Object_var const obj =
    this->naming_context_->resolve (name);

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
}

void
FilterClient::create_EC ()
{
  CosNotifyChannelAdmin::ChannelID id {};

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
FilterClient::create_supplieradmin ()
{
  CosNotifyChannelAdmin::AdminID adminid {};

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  CosNotifyFilter::FilterFactory_var const ffact =
    ec_->default_filter_factory ();

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var const sa_filter =
    ffact->create_filter (tcl_grammar);

  ACE_ASSERT (!CORBA::is_nil (sa_filter.in ()));

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (2);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (supplier_admin_filter_1);
  constraint_list[1].event_types.length (0);
  constraint_list[1].constraint_expr = CORBA::string_dup (supplier_admin_filter_2);

  sa_filter->add_constraints (constraint_list);

  supplier_admin_->add_filter (sa_filter.in ());
}

void
FilterClient::create_consumeradmin ()
{
  CosNotifyChannelAdmin::AdminID adminid {};

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  CosNotifyFilter::FilterFactory_var const ffact =
    ec_->default_filter_factory ();

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var const ca_filter =
    ffact->create_filter (tcl_grammar);

  ACE_ASSERT (!CORBA::is_nil (ca_filter.in ()));

  /*  struct ConstraintExp {
                CosNotification::EventTypeSeq event_types;
                string constraint_expr;
                };
  */
  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (consumer_admin_filter);

  ca_filter->add_constraints (constraint_list);

  consumer_admin_->add_filter (ca_filter.in ());
}

void
FilterClient::create_consumers ()
{
  // startup the first consumer.
  ACE_NEW_THROW_EX (consumer_1_,
                    Filter_StructuredPushConsumer (this, "consumer1"),
                    CORBA::NO_MEMORY ());

  consumer_1_->connect (consumer_admin_.in ());

  // startup the second consumer.
  ACE_NEW_THROW_EX (consumer_2_,
                    Filter_StructuredPushConsumer (this, "consumer2"),
                    CORBA::NO_MEMORY ());

  consumer_2_->connect (consumer_admin_.in ());
}

void
FilterClient::create_suppliers ()
{
  // startup the first supplier
  ACE_NEW_THROW_EX (supplier_1_,
                    Filter_StructuredPushSupplier ("supplier1"),
                    CORBA::NO_MEMORY ());

  supplier_1_->connect (supplier_admin_.in ());

  // startup the second supplier
  ACE_NEW_THROW_EX (supplier_2_,
                    Filter_StructuredPushSupplier ("supplier2"),
                    CORBA::NO_MEMORY ());

  supplier_2_->connect (supplier_admin_.in ());
}

void
FilterClient::send_events ()
{
  // operations:
  CosNotification::StructuredEvent event {};

  // EventHeader

  // FixedEventHeader
  // EventType
  // string
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_name = CORBA::string_dup("myevent");

  // OptionalHeaderFields
  // PropertySeq
  // sequence<Property>: string name, any value
  event.header.variable_header.length (1); // put nothing here

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (4);
  event.filterable_data[0].name = CORBA::string_dup("threshold");
  event.filterable_data[0].value <<= static_cast<CORBA::Long> (4);

  event.filterable_data[1].name = CORBA::string_dup("temperature");
  event.filterable_data[1].value <<= static_cast<CORBA::Long> (70);

  event.filterable_data[2].name = CORBA::string_dup("pressure");
  event.filterable_data[2].value <<= static_cast<CORBA::Long> (80);

  event.filterable_data[3].name = CORBA::string_dup("targetlist");
  char const *ids[] = { "pc1", "pc2", "pc3" };
  CORBA::StringSeq idseq (3, 3, const_cast<char **> (ids), false);
  event.filterable_data[3].value <<= idseq;

  // any
  event.remainder_of_body <<= static_cast<CORBA::Long> (4);

  for (CORBA::ULong i {}; i < events_to_send; ++i)
    {
      event.filterable_data[0].value <<= static_cast<CORBA::Long> (i);

      // any
      event.remainder_of_body <<= static_cast<CORBA::Long> (i);

      supplier_1_->send_event (event);

      supplier_2_->send_event (event);
    }
}

Filter_StructuredPushConsumer::Filter_StructuredPushConsumer (
  FilterClient *filter,
  char const *my_name)
  :filter_ (filter),
   my_name_ (my_name)
{
}

void
Filter_StructuredPushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var const objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var const proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_supplier_id_);

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::
    _narrow (proxysupplier.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (objref.in ());
}

void
Filter_StructuredPushConsumer::disconnect ()
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier();
}

void
Filter_StructuredPushConsumer::offer_change (
  CosNotification::EventTypeSeq const & /*added*/,
  CosNotification::EventTypeSeq const & /*removed*/)
{
}

void
Filter_StructuredPushConsumer::push_structured_event (
  CosNotification::StructuredEvent const &notification)
{
  CORBA::Long val {};

  notification.remainder_of_body >>= val;

  // @@ Pradeep: for your tests try to make sure that you count the
  // number of expected and sent events to verify that things work
  // correctly in an automatic way...
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%C received event, %d\n"), this->my_name_.fast_rep (), val));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("event count %u\n"), result_count.load ()));

  if (++result_count == events_expected_to_receive)
    this->filter_->done (); // all events received, we're done.
}

void
Filter_StructuredPushConsumer::disconnect_structured_push_consumer ()
{
}

Filter_StructuredPushSupplier::Filter_StructuredPushSupplier (char const *my_name)
  : my_name_ (my_name)
{
}

void
Filter_StructuredPushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin)
{
  CosNotifyComm::StructuredPushSupplier_var const objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxyConsumer_var const proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_consumer_id_);

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_structured_push_supplier (objref.in ());
}

void
Filter_StructuredPushSupplier::disconnect ()
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer();
}

void
Filter_StructuredPushSupplier::subscription_change (
  CosNotification::EventTypeSeq const & /*added*/,
  CosNotification::EventTypeSeq const & /*removed*/)
{
}

void
Filter_StructuredPushSupplier::send_event
  (CosNotification::StructuredEvent const &event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%C is sending an event\n"), this->my_name_.fast_rep ()));

  proxy_consumer_->push_structured_event (event);
}

void
Filter_StructuredPushSupplier::disconnect_structured_push_supplier ()
{
}

