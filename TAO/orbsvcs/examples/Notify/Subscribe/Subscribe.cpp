/* -*- C++ -*-  */
#include "Subscribe.h"
#include <cstring>

constexpr char notify_factory_name[] = "NotifyEventChannelFactory";
constexpr char naming_service_name[] = "NameService";

constexpr char domain_a[] = "domain_a";
constexpr char domain_b[] = "domain_b";
constexpr char domain_c[] = "domain_c";

constexpr char type_a[] = "type_a";
constexpr char type_b[] = "type_b";
constexpr char type_c[] = "type_c";

constexpr CORBA::ULong event_count = 4; // Expected events delivered to the consumers.

static std::atomic<CORBA::ULong> result_count {};

Subscribe::~Subscribe ()
{
  this->ec_->destroy ();
}

void
Subscribe::init (int argc, ACE_TCHAR *argv [])
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
Subscribe::run ()
{
  this->send_events ();

  if (result_count.load () != event_count)
    {
      while (!this->done_.load ())
        if (this->orb_->work_pending ())
          this->orb_->perform_work ();
    }
}

void
Subscribe::done ()
{
  this->done_.store (true);
}

void
Subscribe::init_ORB (int argc, ACE_TCHAR *argv [])
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
Subscribe::resolve_naming_service ()
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
Subscribe::resolve_Notify_factory ()
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
Subscribe::create_EC ()
{
  CosNotifyChannelAdmin::ChannelID id {};

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Subscribe::create_supplieradmin ()
{
  CosNotifyChannelAdmin::AdminID adminid {};

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));
}

void
Subscribe::create_consumeradmin ()
{
  CosNotifyChannelAdmin::AdminID adminid {};

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));
}

void
Subscribe::create_consumers ()
{
  consumer_1_ = std::make_unique<Subscribe_StructuredPushConsumer> (this);
  consumer_1_->connect (this->consumer_admin_.in ());

  consumer_2_ = std::make_unique<Subscribe_StructuredPushConsumer> (this);
  consumer_2_->connect (this->consumer_admin_.in ());
}

void
Subscribe::create_suppliers ()
{
  supplier_1_ = std::make_unique<Subscribe_StructuredPushSupplier> ();
  supplier_1_->connect (this->supplier_admin_.in ());

  supplier_2_ = std::make_unique<Subscribe_StructuredPushSupplier> ();
  supplier_2_->connect (this->supplier_admin_.in ());
}

void
Subscribe::send_events ()
{
  // Setup the CA to receive event_type : "domain_A", "Type_a"
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);

  added[0].domain_name = CORBA::string_dup (domain_a);
  added[0].type_name = CORBA::string_dup (type_a);

  this->consumer_admin_->subscription_change (added, removed);

  // Setup the Consumer 1 to receive event_type : "domain_B", "Type_b"
  CosNotification::EventTypeSeq added_1(1);
  CosNotification::EventTypeSeq removed_1 (0);
  added_1.length (1);

  added_1[0].domain_name = CORBA::string_dup (domain_b);
  added_1[0].type_name = CORBA::string_dup (type_b);
  removed_1.length (0);

  this->consumer_1_->get_proxy_supplier ()->subscription_change (added_1, removed_1);
  // now the expected subscription for consumer 1 should be
  // A and B
  this->consumer_1_->expected_subscription_.length (2);
  this->consumer_1_->expected_subscription_[0] = added[0];
  this->consumer_1_->expected_subscription_[1] = added_1[0];

  // Setup the Consumer 2 to receive event_type : "domain_C", "Type_c"
  CosNotification::EventTypeSeq added_2(1);
  CosNotification::EventTypeSeq removed_2 (0);
  added_2.length (1);

  added_2[0].domain_name = CORBA::string_dup (domain_c);
  added_2[0].type_name = CORBA::string_dup (type_c);
  removed_2.length (0);

  this->consumer_2_->get_proxy_supplier ()->subscription_change (added_2, removed_2);
  // now the expected subscription for consumer 2 should be
  // A and C
  this->consumer_2_->expected_subscription_.length (2);
  this->consumer_2_->expected_subscription_[0] = added[0];
  this->consumer_2_->expected_subscription_[1] = added_2[0];

  // Create the events - one of each type
  // Event 1
  CosNotification::StructuredEvent event1 {};
  event1.header.fixed_header.event_type.domain_name =
    CORBA::string_dup (domain_a);
  event1.header.fixed_header.event_type.type_name =
    CORBA::string_dup (type_a);
  event1.header.fixed_header.event_name = CORBA::string_dup ("");
  event1.header.variable_header.length (0); // put nothing here
  event1.filterable_data.length (0);
  event1.remainder_of_body <<= static_cast<CORBA::Long> (10);

  // Event 2
  CosNotification::StructuredEvent event2 {};
  event2.header.fixed_header.event_type.domain_name =
    CORBA::string_dup (domain_b);
  event2.header.fixed_header.event_type.type_name =
    CORBA::string_dup (type_b);
  event2.header.fixed_header.event_name = CORBA::string_dup ("");
  event2.header.variable_header.length (0); // put nothing here
  event2.filterable_data.length (0);
  event2.remainder_of_body <<= static_cast<CORBA::Long> (10);

  // event 3
  CosNotification::StructuredEvent event3 {};
  event3.header.fixed_header.event_type.domain_name =
    CORBA::string_dup (domain_c);
  event3.header.fixed_header.event_type.type_name =
    CORBA::string_dup (type_c);
  event3.header.fixed_header.event_name = CORBA::string_dup ("");
  event3.header.variable_header.length (0); // put nothing here
  event3.filterable_data.length (0);
  event3.remainder_of_body <<= static_cast<CORBA::Long> (10);

  // Let supplier 1 send all three events.
  supplier_1_->send_event (event1);
  supplier_1_->send_event (event2);
  supplier_1_->send_event (event3);
}

Subscribe_StructuredPushConsumer::Subscribe_StructuredPushConsumer (Subscribe *subscribe)
  : expected_subscription_ (2),
    subscribe_ (subscribe)
{
}

void
Subscribe_StructuredPushConsumer::connect (
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
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
Subscribe_StructuredPushConsumer::disconnect ()
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier();
}

void
Subscribe_StructuredPushConsumer::offer_change (
  CosNotification::EventTypeSeq const & /*added*/,
  CosNotification::EventTypeSeq const & /*removed*/)
{
}

void
Subscribe_StructuredPushConsumer::push_structured_event (
  CosNotification::StructuredEvent const &notification)
{
  char const * const domain_name =
    notification.header.fixed_header.event_type.domain_name;

  char const * const type_name =
    notification.header.fixed_header.event_type.type_name;

  bool found {};

  for (CORBA::ULong i {}; i < this->expected_subscription_.length (); ++i)
    if (std::strcmp (this->expected_subscription_[i].domain_name, domain_name) == 0)
      found = true;

  if (found)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Structured Subscribe Consumer %d received subscribed event, domain = %C, type = %C\n"),
                this->proxy_supplier_id_, domain_name, type_name));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error: Structured Subscribe Consumer %d received not subscribed event, "
                          "domain = %C, type = %C\n"),
                this->proxy_supplier_id_, domain_name, type_name));

  if (++result_count == event_count)
    this->subscribe_->done ();
}

void
Subscribe_StructuredPushConsumer::disconnect_structured_push_consumer ()
{
}

CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr
Subscribe_StructuredPushConsumer::get_proxy_supplier ()
{
  return proxy_supplier_.in ();
}

/*****************************************************************/

void
Subscribe_StructuredPushSupplier::connect (
  CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin)
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
Subscribe_StructuredPushSupplier::disconnect ()
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer();
}

void
Subscribe_StructuredPushSupplier::subscription_change (
  CosNotification::EventTypeSeq const & /*added*/,
  CosNotification::EventTypeSeq const & /*removed*/)
{
}

void
Subscribe_StructuredPushSupplier::send_event (
  CosNotification::StructuredEvent const &event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  proxy_consumer_->push_structured_event (event);
}

void
Subscribe_StructuredPushSupplier::disconnect_structured_push_supplier ()
{
}

