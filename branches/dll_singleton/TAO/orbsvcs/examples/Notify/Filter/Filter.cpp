/* -*- C++ -*- $Id$ */

#include "Filter.h"

ACE_RCSID(Filter, Filter, "$Id$")

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"
#define CA_FILTER "threshold < 20"
#define SA_FILTER "threshold > 10"
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
FilterClient::init (int argc, char *argv [] ACE_ENV_ARG_DECL)
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
FilterClient::run (ACE_ENV_SINGLE_ARG_DECL)
{
  send_events (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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
FilterClient::init_ORB (int argc,
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
    PortableServer::POA::_narrow (poa_object
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
FilterClient::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
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
FilterClient::resolve_Notify_factory (ACE_ENV_SINGLE_ARG_DECL)
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
FilterClient::create_EC (ACE_ENV_SINGLE_ARG_DECL)
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
FilterClient::create_supplieradmin (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var sa_filter =
    ffact->create_filter (TCL_GRAMMAR ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (sa_filter.in ()));

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (SA_FILTER);

  sa_filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_admin_->add_filter (sa_filter.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FilterClient:: create_consumeradmin (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var ca_filter =
    ffact->create_filter (TCL_GRAMMAR ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

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

  ca_filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer_admin_->add_filter (ca_filter.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the CA to receive all type of events
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FilterClient::create_consumers (ACE_ENV_SINGLE_ARG_DECL)
{
  // startup the first consumer.
  ACE_NEW_THROW_EX (consumer_1,
                    Filter_StructuredPushConsumer (this, "consumer1"),
                    CORBA::NO_MEMORY ());

  consumer_1->connect (consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // startup the second consumer.
  ACE_NEW_THROW_EX (consumer_2,
                    Filter_StructuredPushConsumer (this, "consumer2"),
                    CORBA::NO_MEMORY ());

  consumer_2->connect (consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FilterClient::create_suppliers (ACE_ENV_SINGLE_ARG_DECL)
{
  // startup the first supplier
  ACE_NEW_THROW_EX (supplier_1,
                    Filter_StructuredPushSupplier ("supplier1"),
                    CORBA::NO_MEMORY ());

  supplier_1->connect (supplier_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // startup the second supplier
  ACE_NEW_THROW_EX (supplier_2,
                    Filter_StructuredPushSupplier ("supplier2"),
                    CORBA::NO_MEMORY ());

  supplier_2->connect (supplier_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FilterClient::send_events (ACE_ENV_SINGLE_ARG_DECL)
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
  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup("threshold");

  event.filterable_data[1].name = CORBA::string_dup("temperature");
  event.filterable_data[1].value <<= (CORBA::Long)70;

  event.filterable_data[2].name = CORBA::string_dup("pressure");
  event.filterable_data[2].value <<= (CORBA::Long)80;

  event.filterable_data[0].value <<= (CORBA::Long)4;

  // any
  event.remainder_of_body <<= (CORBA::Long)4;

   for (int i = 0; i < EVENTS_TO_SEND; i++)
    {
      event.filterable_data[0].value <<= (CORBA::Long)i;

      // any
      event.remainder_of_body <<= (CORBA::Long)i;

      supplier_1->send_event (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      supplier_2->send_event (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
Filter_StructuredPushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL)
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
Filter_StructuredPushConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Filter_StructuredPushConsumer::offer_change
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
Filter_StructuredPushConsumer::push_structured_event
   (const CosNotification::StructuredEvent & notification
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
    CORBA::Long val;

    notification.remainder_of_body >>= val;

    // @@ Pradeep: for your tests try to make sure that you count the
    // number of expected and sent events to verify that things work
    // correctly in an automatic way...


    ACE_DEBUG ((LM_DEBUG,
                "%s received event, %d\n", my_name_.fast_rep (), val));

    ACE_DEBUG ((LM_DEBUG,"event count %d\n", g_result_count.value ()));

    if (++g_result_count == EVENTS_EXPECTED_TO_RECEIVE)
      this->filter_->done (); // all events received, we're done.
}

void
Filter_StructuredPushConsumer::disconnect_structured_push_consumer
   (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
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
Filter_StructuredPushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin ACE_ENV_ARG_DECL)
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
Filter_StructuredPushSupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Filter_StructuredPushSupplier::subscription_change
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
Filter_StructuredPushSupplier::send_event
   (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  ACE_DEBUG ((LM_DEBUG,
              "%s is sending an event \n", my_name_.fast_rep ()));

  proxy_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Filter_StructuredPushSupplier::disconnect_structured_push_supplier
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
