/* -*- C++ -*- $Id$ */
#include "Subscribe.h"

ACE_RCSID(Notify, Subscribe, "$Id$")

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
{
  // No-Op.
  ifgop_ = CosNotifyChannelAdmin::OR_OP;
}

Subscribe::~Subscribe ()
{
  this->ec_->destroy ();
}

void
Subscribe::init (int argc, char *argv [] ACE_ENV_ARG_DECL)
{
  init_ORB (argc, argv ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  resolve_Notify_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  create_EC (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  create_supplieradmin (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  create_consumeradmin (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  create_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  create_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->send_events (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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
Subscribe::init_ORB (int argc,
                      char *argv []
                      ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_ptr poa_object  =
    this->orb_->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (poa_object))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
      return;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe::resolve_Notify_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe::create_EC (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Subscribe::create_supplieradmin (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));
}

void
Subscribe:: create_consumeradmin (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid;

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));
}

void
Subscribe::create_consumers (ACE_ENV_SINGLE_ARG_DECL)
{
  consumer_1_ = new Subscribe_StructuredPushConsumer (this);
  consumer_1_->connect (this->consumer_admin_.in ()
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer_2_ = new Subscribe_StructuredPushConsumer (this);
  consumer_2_->connect (this->consumer_admin_.in ()
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe::create_suppliers (ACE_ENV_SINGLE_ARG_DECL)
{
  supplier_1_ = new Subscribe_StructuredPushSupplier ();
  supplier_1_->connect (this->supplier_admin_.in ()
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_2_ = new Subscribe_StructuredPushSupplier ();
  supplier_2_->connect (this->supplier_admin_.in ()
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe::send_events (ACE_ENV_SINGLE_ARG_DECL)
{
  // Setup the CA to receive event_type : "domain_A", "Type_a"
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (1);
  added.length (1);
  removed.length (1);

  added[0].domain_name =  CORBA::string_dup (DOMAIN_A);
  added[0].type_name = CORBA::string_dup (TYPE_A);

  removed[0].domain_name =  CORBA::string_dup ("*");
  removed[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the Consumer 1 to receive event_type : "domain_B", "Type_b"
  CosNotification::EventTypeSeq added_1(1);
  CosNotification::EventTypeSeq removed_1 (0);

  added_1[0].domain_name =  CORBA::string_dup (DOMAIN_B);
  added_1[0].type_name = CORBA::string_dup (TYPE_B);
  added_1.length (1);
  removed_1.length (0);

  this->consumer_1_->get_proxy_supplier ()->subscription_change (added_1, removed_1
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the Consumer 2 to receive event_type : "domain_C", "Type_c"
  CosNotification::EventTypeSeq added_2(1);
  CosNotification::EventTypeSeq removed_2 (0);

  added_2[0].domain_name =  CORBA::string_dup (DOMAIN_C);
  added_2[0].type_name = CORBA::string_dup (TYPE_C);
  added_2.length (1);
  removed_2.length (0);

  this->consumer_2_->get_proxy_supplier ()->subscription_change (added_2, removed_2
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

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
      supplier_1_->send_event (event1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      supplier_1_->send_event (event2 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      supplier_1_->send_event (event3 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

/*****************************************************************/
Subscribe_StructuredPushConsumer::Subscribe_StructuredPushConsumer (Subscribe* subscribe)
  : subscribe_ (subscribe)
{
}

Subscribe_StructuredPushConsumer::~Subscribe_StructuredPushConsumer ()
{
}

void
Subscribe_StructuredPushConsumer::connect
   (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
    ACE_ENV_ARG_DECL)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_supplier_id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::
    _narrow (proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe_StructuredPushConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe_StructuredPushConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  // No-Op.
}

void
Subscribe_StructuredPushConsumer::push_structured_event
   (const CosNotification::StructuredEvent & notification
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  const char* domain_name =
    notification.header.fixed_header.event_type.domain_name;

  const char* type_name =
    notification.header.fixed_header.event_type.type_name;

  ACE_DEBUG ((LM_DEBUG, "Structured Subscribe Consumer %d received event, domain = %s, type =  %s\n", this->proxy_supplier_id_, domain_name, type_name));

  if (++g_result_count == EVENT_COUNT)
    subscribe_->done ();

}

void
Subscribe_StructuredPushConsumer::disconnect_structured_push_consumer
   (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
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
   (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin
    ACE_ENV_ARG_DECL)
{
  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_consumer_id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_structured_push_supplier (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe_StructuredPushSupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Subscribe_StructuredPushSupplier::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  //No-Op.
}

void
Subscribe_StructuredPushSupplier::send_event
   (const CosNotification::StructuredEvent& event
    ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  proxy_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Subscribe_StructuredPushSupplier::disconnect_structured_push_supplier
   (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // No-Op.
}

/*****************************************************************/

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
