/* -*- C++ -*- $Id$ */
#include "Subscribe.h"



#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"

#define DOMAIN_A "domain_a"
#define DOMAIN_B "domain_b"
#define DOMAIN_C "domain_c"

#define TYPE_A "type_a"
#define TYPE_B "type_b"
#define TYPE_C "type_c"

#define EVENT_COUNT 4 // number of events we expect the consumer to get from the EC

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> g_result_count = 0; // we wait for 4 events.

Subscribe::Subscribe (void)
  : done_ (0)
{
  // No-Op.
  ifgop_ = CosNotifyChannelAdmin::OR_OP;
}

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
Subscribe::run (void)
{
  this->send_events ();

  if (g_result_count != EVENT_COUNT) // if we still need to wait for events, run the orb.
    { // if we still need to wait for events, run the orb.
      while (!this->done_)
        if (this->orb_->work_pending ())
          this->orb_->perform_work ();
    }
}

void
Subscribe::done (void)
{
  this->done_ = 1;
}

void
Subscribe::init_ORB (int argc, ACE_TCHAR *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  CORBA::Object_ptr poa_object  =
    this->orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (poa_object))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
      return;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object);

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager ();

  poa_manager->activate ();
}

void
Subscribe::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

void
Subscribe::resolve_Notify_factory (void)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name);

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
}

void
Subscribe::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Subscribe::create_supplieradmin (void)
{
  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));
}

void
Subscribe:: create_consumeradmin (void)
{
  CosNotifyChannelAdmin::AdminID adminid;

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));
}

void
Subscribe::create_consumers (void)
{
  consumer_1_ = new Subscribe_StructuredPushConsumer (this);
  consumer_1_->connect (this->consumer_admin_.in ());

  consumer_2_ = new Subscribe_StructuredPushConsumer (this);
  consumer_2_->connect (this->consumer_admin_.in ());
}

void
Subscribe::create_suppliers (void)
{
  supplier_1_ = new Subscribe_StructuredPushSupplier ();
  supplier_1_->connect (this->supplier_admin_.in ());

  supplier_2_ = new Subscribe_StructuredPushSupplier ();
  supplier_2_->connect (this->supplier_admin_.in ());
}

void
Subscribe::send_events (void)
{
  // Setup the CA to receive event_type : "domain_A", "Type_a"
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);

  added[0].domain_name =  CORBA::string_dup (DOMAIN_A);
  added[0].type_name = CORBA::string_dup (TYPE_A);

  this->consumer_admin_->subscription_change (added, removed);

  // Setup the Consumer 1 to receive event_type : "domain_B", "Type_b"
  CosNotification::EventTypeSeq added_1(1);
  CosNotification::EventTypeSeq removed_1 (0);
  added_1.length (1);

  added_1[0].domain_name =  CORBA::string_dup (DOMAIN_B);
  added_1[0].type_name = CORBA::string_dup (TYPE_B);
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

  added_2[0].domain_name =  CORBA::string_dup (DOMAIN_C);
  added_2[0].type_name = CORBA::string_dup (TYPE_C);
  removed_2.length (0);

  this->consumer_2_->get_proxy_supplier ()->subscription_change (added_2, removed_2);
  // now the expected subscription for consumer 2 should be
  // A and C
  this->consumer_2_->expected_subscription_.length (2);
  this->consumer_2_->expected_subscription_[0] = added[0];
  this->consumer_2_->expected_subscription_[1] = added_2[0];

  // Create the events - one of each type
  // Event 1
  CosNotification::StructuredEvent event1;
  event1.header.fixed_header.event_type.domain_name =
    CORBA::string_dup(DOMAIN_A);
  event1.header.fixed_header.event_type.type_name =
    CORBA::string_dup(TYPE_A);
  event1.header.fixed_header.event_name = CORBA::string_dup("");
  event1.header.variable_header.length (0); // put nothing here
  event1.filterable_data.length (0);
  event1.remainder_of_body <<= (CORBA::Long)10;

  // Event 2
  CosNotification::StructuredEvent event2;
  event2.header.fixed_header.event_type.domain_name =
    CORBA::string_dup(DOMAIN_B);
  event2.header.fixed_header.event_type.type_name =
    CORBA::string_dup(TYPE_B);
  event2.header.fixed_header.event_name = CORBA::string_dup("");
  event2.header.variable_header.length (0); // put nothing here
  event2.filterable_data.length (0);
  event2.remainder_of_body <<= (CORBA::Long)10;

  // event 3
  CosNotification::StructuredEvent event3;
  event3.header.fixed_header.event_type.domain_name =
    CORBA::string_dup(DOMAIN_C);
  event3.header.fixed_header.event_type.type_name =
    CORBA::string_dup(TYPE_C);
  event3.header.fixed_header.event_name = CORBA::string_dup("");
  event3.header.variable_header.length (0); // put nothing here
  event3.filterable_data.length (0);
  event3.remainder_of_body <<= (CORBA::Long)10;

 // let supplier 1 send all these events
  for (int i = 0; i < 1; ++i)
    {
      supplier_1_->send_event (event1);
      supplier_1_->send_event (event2);
      supplier_1_->send_event (event3);
    }
}

/*****************************************************************/
Subscribe_StructuredPushConsumer::Subscribe_StructuredPushConsumer (Subscribe* subscribe)
  : expected_subscription_ (2),
    subscribe_ (subscribe)
{
}

Subscribe_StructuredPushConsumer::~Subscribe_StructuredPushConsumer ()
{
}

void
Subscribe_StructuredPushConsumer::connect
   (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
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
Subscribe_StructuredPushConsumer::disconnect (void)
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier();
}

void
Subscribe_StructuredPushConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    )
{
  // No-Op.
}

void
Subscribe_StructuredPushConsumer::push_structured_event
   (const CosNotification::StructuredEvent & notification)
{
  const char* domain_name =
    notification.header.fixed_header.event_type.domain_name;

  const char* type_name =
    notification.header.fixed_header.event_type.type_name;

  bool found = false;

  for (CORBA::ULong i = 0; i < expected_subscription_.length (); i++)
  {
    if ( ACE_OS::strcmp (expected_subscription_[i].domain_name, domain_name) == 0)
      found = true;
  }

  if (found)
    ACE_DEBUG ((LM_DEBUG, "Structured Subscribe Consumer %d received subscribed event, domain = %s, type =  %s\n",
                this->proxy_supplier_id_, domain_name, type_name));
  else
    ACE_ERROR ((LM_ERROR, "Error: Structured Subscribe Consumer %d received not subcribed event , domain = %s, type =  %s\n",
                this->proxy_supplier_id_, domain_name, type_name));

  if (++g_result_count == EVENT_COUNT)
    subscribe_->done ();

}

void
Subscribe_StructuredPushConsumer::disconnect_structured_push_consumer
   (void)
{
  // No-Op.
}

CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr
Subscribe_StructuredPushConsumer::get_proxy_supplier (void)
{
  return proxy_supplier_.in ();
}

/*****************************************************************/

Subscribe_StructuredPushSupplier::Subscribe_StructuredPushSupplier (void)
{
}

Subscribe_StructuredPushSupplier::~Subscribe_StructuredPushSupplier ()
{
}

void
Subscribe_StructuredPushSupplier::connect
   (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin)
{
  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_consumer_id_);

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_structured_push_supplier (objref.in ());
}

void
Subscribe_StructuredPushSupplier::disconnect (void)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer();
}

void
Subscribe_StructuredPushSupplier::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */
    )
{
  //No-Op.
}

void
Subscribe_StructuredPushSupplier::send_event
   (const CosNotification::StructuredEvent& event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  proxy_consumer_->push_structured_event (event);
}

void
Subscribe_StructuredPushSupplier::disconnect_structured_push_supplier
   (void)
{
  // No-Op.
}

