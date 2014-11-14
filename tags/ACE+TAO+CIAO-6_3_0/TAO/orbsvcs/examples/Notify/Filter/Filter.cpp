/* -*- C++ -*- $Id$ */

#include "Filter.h"

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"
#define CA_FILTER "threshold < 20"
#define SA_FILTER1 "threshold > 10"
#define SA_FILTER2 "'pc1' in targetlist"
#define TCL_GRAMMAR "TCL"
#define EVENTS_TO_SEND 30
#define EVENTS_EXPECTED_TO_RECEIVE 9*4  // 2 consumers get the same events from 2 suppliers

ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> g_result_count = 0;

FilterClient::FilterClient (void)
  :done_ (0)
{
  g_result_count = 0;
  // No-Op.
  ifgop_ = CosNotifyChannelAdmin::AND_OP;
}

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
FilterClient::run (void)
{
  send_events ();

  if (g_result_count != EVENTS_EXPECTED_TO_RECEIVE)
    { // if we still need to wait for events, run the orb.
      while (!this->done_)
        if (this->orb_->work_pending ())
          this->orb_->perform_work ();
    }
}

void
FilterClient::done (void)
{
  this->done_ = 1;
}

void
FilterClient::init_ORB (int argc, ACE_TCHAR *argv [])
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
FilterClient::resolve_naming_service (void)
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
FilterClient::resolve_Notify_factory (void)
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
FilterClient::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
FilterClient::create_supplieradmin (void)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory ();

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var sa_filter =
    ffact->create_filter (TCL_GRAMMAR);

  ACE_ASSERT (!CORBA::is_nil (sa_filter.in ()));

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (2);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (SA_FILTER1);
  constraint_list[1].event_types.length (0);
  constraint_list[1].constraint_expr = CORBA::string_dup (SA_FILTER2);

  sa_filter->add_constraints (constraint_list);

  supplier_admin_->add_filter (sa_filter.in ());
}

void
FilterClient:: create_consumeradmin (void)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory ();

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var ca_filter =
    ffact->create_filter (TCL_GRAMMAR);

  ACE_ASSERT (!CORBA::is_nil (ca_filter.in ()));

  /*  struct ConstraintExp {
                CosNotification::EventTypeSeq event_types;
                string constraint_expr;
                };
  */
  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (CA_FILTER);

  ca_filter->add_constraints (constraint_list);

  consumer_admin_->add_filter (ca_filter.in ());
}

void
FilterClient::create_consumers (void)
{
  // startup the first consumer.
  ACE_NEW_THROW_EX (consumer_1,
                    Filter_StructuredPushConsumer (this, "consumer1"),
                    CORBA::NO_MEMORY ());

  consumer_1->connect (consumer_admin_.in ());

  // startup the second consumer.
  ACE_NEW_THROW_EX (consumer_2,
                    Filter_StructuredPushConsumer (this, "consumer2"),
                    CORBA::NO_MEMORY ());

  consumer_2->connect (consumer_admin_.in ());
}

void
FilterClient::create_suppliers (void)
{
  // startup the first supplier
  ACE_NEW_THROW_EX (supplier_1,
                    Filter_StructuredPushSupplier ("supplier1"),
                    CORBA::NO_MEMORY ());

  supplier_1->connect (supplier_admin_.in ());

  // startup the second supplier
  ACE_NEW_THROW_EX (supplier_2,
                    Filter_StructuredPushSupplier ("supplier2"),
                    CORBA::NO_MEMORY ());

  supplier_2->connect (supplier_admin_.in ());
}

void
FilterClient::send_events (void)
{
  // operations:
  CosNotification::StructuredEvent event;

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
  event.filterable_data[0].value <<= (CORBA::Long)4;

  event.filterable_data[1].name = CORBA::string_dup("temperature");
  event.filterable_data[1].value <<= (CORBA::Long)70;

  event.filterable_data[2].name = CORBA::string_dup("pressure");
  event.filterable_data[2].value <<= (CORBA::Long)80;

  event.filterable_data[3].name = CORBA::string_dup("targetlist");
  const char* ids[] = { "pc1", "pc2", "pc3" };
  CORBA::StringSeq idseq(3, 3, const_cast<char**> (ids), 0);
  event.filterable_data[3].value <<= idseq;

  // any
  event.remainder_of_body <<= (CORBA::Long)4;

   for (int i = 0; i < EVENTS_TO_SEND; i++)
    {
      event.filterable_data[0].value <<= (CORBA::Long)i;

      // any
      event.remainder_of_body <<= (CORBA::Long)i;

      supplier_1->send_event (event);

      supplier_2->send_event (event);
    }
}


Filter_StructuredPushConsumer::Filter_StructuredPushConsumer (FilterClient* filter, const char* my_name)
  :filter_ (filter),
   my_name_ (my_name)
{
}

Filter_StructuredPushConsumer::~Filter_StructuredPushConsumer (void)
{
}

void
Filter_StructuredPushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
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
Filter_StructuredPushConsumer::disconnect (void)
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier();
}

void
Filter_StructuredPushConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    )
{
  // No-Op.
}

void
Filter_StructuredPushConsumer::push_structured_event
   (const CosNotification::StructuredEvent & notification)
{
    CORBA::Long val;

    notification.remainder_of_body >>= val;

    // @@ Pradeep: for your tests try to make sure that you count the
    // number of expected and sent events to verify that things work
    // correctly in an automatic way...


    ACE_DEBUG ((LM_DEBUG,
                "%C received event, %d\n", my_name_.fast_rep (), val));

    ACE_DEBUG ((LM_DEBUG,"event count %d\n", g_result_count.value ()));

    if (++g_result_count == EVENTS_EXPECTED_TO_RECEIVE)
      this->filter_->done (); // all events received, we're done.
}

void
Filter_StructuredPushConsumer::disconnect_structured_push_consumer
   (void)
{
  // No-Op.
}


/*****************************************************************/

Filter_StructuredPushSupplier::Filter_StructuredPushSupplier  (const char* my_name)
    :my_name_ (my_name)
{
}

Filter_StructuredPushSupplier::~Filter_StructuredPushSupplier ()
{
}

void
Filter_StructuredPushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin)
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
Filter_StructuredPushSupplier::disconnect (void)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer();
}

void
Filter_StructuredPushSupplier::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */
    )
{
  //No-Op.
}

void
Filter_StructuredPushSupplier::send_event
   (const CosNotification::StructuredEvent& event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  ACE_DEBUG ((LM_DEBUG,
              "%C is sending an event\n", my_name_.fast_rep ()));

  proxy_consumer_->push_structured_event (event);
}

void
Filter_StructuredPushSupplier::disconnect_structured_push_supplier
   (void)
{
  // No-Op.
}

