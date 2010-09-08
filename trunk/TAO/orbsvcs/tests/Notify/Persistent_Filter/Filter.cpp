/* -*- C++ -*- $Id$ */

#include "Filter.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"



#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"
#define CA_FILTER "threshold < 20"
#define SA_FILTER "threshold > 10"

#define MOD_CA_FILTER "threshold < 15"
#define MOD_SA_FILTER "threshold > 10"

#define TCL_GRAMMAR "TCL"
#define EVENTS_TO_SEND 30
int EVENTS_EXPECTED_TO_RECEIVE = 9*4;  // 2 consumers get the same events from 2 suppliers
#define DOMAIN_NAME "*"
#define TYPE_NAME "*"

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> g_result_count = 0;

FilterClient::FilterClient (void)
  :consumer_1 (0),
  consumer_2 (0),
  supplier_1 (0),
  supplier_2 (0),
  adminid_1_id_ (0),
  adminid_2_id_ (0),
  channel_id_ (0),
  use_persistent_ (false),
  modify_constraint_ (false),
  done_ (0)
{
  g_result_count = 0;
  // No-Op.
  ifgop_ = CosNotifyChannelAdmin::AND_OP;
}

FilterClient::~FilterClient ()
{
}


int
FilterClient::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("pm"));
  int c;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'p':
        this->use_persistent_ = true;
        break;
      case 'm':
        this->modify_constraint_ = true;
        EVENTS_EXPECTED_TO_RECEIVE = 4*4;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %C [-p] [-m]\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

void
FilterClient::init_supplier (int argc, ACE_TCHAR *argv [])
{
  init_ORB (argc, argv);

  if (parse_args (argc, argv) == -1)
  {
    ACE_OS::exit (1);
  }

  resolve_naming_service ();

  resolve_Notify_factory ();

  if (this->use_persistent_)
  {
    get_EC ();
  }
  else
  {
    create_EC ();
  }

  if (this->use_persistent_)
  {
    get_supplieradmin ();
  }
  else
  {
    create_supplieradmin ();
  }

  create_suppliers ();
}


void
FilterClient::init_consumer (int argc, ACE_TCHAR *argv [])
{
  init_ORB (argc, argv);

  if (parse_args (argc, argv) == -1)
  {
    ACE_OS::exit (1);
  }

  resolve_naming_service ();

  resolve_Notify_factory ();

  get_EC ();

  if (this->use_persistent_)
  {
    get_consumeradmin ();
  }
  else
  {
    create_consumeradmin ();
  }

  create_consumers ();
}

void
FilterClient::wait_ready ()
{
  while ( 1 )
  {
    CosNotifyChannelAdmin::AdminIDSeq_var cons_ids
      = ec_->get_all_consumeradmins ();

    CosNotifyChannelAdmin::AdminIDSeq_var sup_ids
      = ec_->get_all_supplieradmins ();

    if (cons_ids->length () + sup_ids->length () == 3)
      break;
    else
      ACE_OS::sleep (1);
  }
}


void
FilterClient::wait_consumer_complete ()
{
  int i = 0;
  const int TIMEOUT = 30;

  while ( i < TIMEOUT )
  {
    try
    {
      CosNotifyChannelAdmin::AdminIDSeq_var ids
        = this->ec_->get_all_consumeradmins();

      if (ids->length () > 0)
      {
        ACE_OS::sleep (1);
        ++i;
      }
      else
        break;
    }
    catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      break;
    }
  }
}


void
FilterClient::run_supplier ()
{
  this->wait_ready ();

  //Add delay so consumer won't miss any events.
  ACE_OS::sleep (5);

  send_events ();

  this->wait_consumer_complete ();
}

void
FilterClient::run_consumer ()
{
  this->wait_ready ();

  if (g_result_count != EVENTS_EXPECTED_TO_RECEIVE)
    { // if we still need to wait for events, run the orb.
      while (!this->done_)
        if (this->orb_->work_pending ())
          this->orb_->perform_work ();
    }

  //ACE_DEBUG ((LM_DEBUG, "(%P|%t)destroying consumer admins ... \n"));

  //this->consumer_admin_1_->destroy ();
  //this->consumer_admin_2_->destroy ();
}

void
FilterClient::done (void)
{
  this->done_ = 1;
}

void
FilterClient::init_ORB (int& argc, ACE_TCHAR **& argv)
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
FilterClient::resolve_naming_service ()
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
FilterClient::resolve_Notify_factory ()
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
FilterClient::create_EC ()
{
  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         channel_id_);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
FilterClient::get_EC ()
{
  int i = 0;
  const int TIMEOUT = 20;
  while (i < TIMEOUT)
  {
    CosNotifyChannelAdmin::ChannelIDSeq_var ids
      = notify_factory_->get_all_channels ();

    if (ids->length () > 0)
    {
      //ACE_DEBUG ((LM_DEBUG, "(%P|%t)get_EC %d \n", ids->length ()));
      ec_ = notify_factory_->get_event_channel (ids[0]);

      ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
      break;
    }
    else
    {
      ++ i;
      ACE_OS::sleep (1);
    }
  }
}

void
FilterClient::create_supplieradmin ()
{
  CosNotifyChannelAdmin::AdminID adminid = 0;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t)create_supplieradmin %d \n"), adminid));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory ();

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var sa_filter =
    ffact->create_filter (TCL_GRAMMAR);

  ACE_ASSERT (!CORBA::is_nil (sa_filter.in ()));

  CosNotifyFilter::ConstraintExpSeq constraint_list (2);
  constraint_list.length (2);

  constraint_list[0].event_types.length (1);
  constraint_list[0].event_types[0].domain_name = CORBA::string_dup(DOMAIN_NAME);
  constraint_list[0].event_types[0].type_name = CORBA::string_dup(TYPE_NAME);

  constraint_list[0].constraint_expr = CORBA::string_dup (SA_FILTER);
  constraint_list[1].event_types.length (1);
  constraint_list[1].event_types[0].domain_name = CORBA::string_dup(DOMAIN_NAME);
  constraint_list[1].event_types[0].type_name = CORBA::string_dup(TYPE_NAME);
  constraint_list[1].constraint_expr = CORBA::string_dup (SA_FILTER);

  sa_filter->add_constraints (constraint_list);

  supplier_admin_->add_filter (sa_filter.in ());
}


void
FilterClient::get_supplieradmin ()
{
  CosNotifyChannelAdmin::AdminIDSeq_var ids
    = ec_->get_all_supplieradmins();

  ACE_ASSERT (ids->length () == 1);

  supplier_admin_ = ec_->get_supplieradmin (ids[0]);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t)get_supplieradmin %d \n"), ids[0]));

  CosNotifyFilter::FilterAdmin_var admin
    = CosNotifyFilter::FilterAdmin::_narrow (supplier_admin_.in ());
  verify_filter (admin, SA_FILTER, MOD_SA_FILTER);
}


void
FilterClient::create_consumeradmin ()
{
  consumer_admin_1_ =
    ec_->new_for_consumers (this->ifgop_, this->adminid_1_id_);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_1_.in ()));

  consumer_admin_2_ =
    ec_->new_for_consumers (this->ifgop_, this->adminid_2_id_);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_2_.in ()));

  ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t)create_consumeradmin %d %d\n"),
    adminid_1_id_, adminid_2_id_));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory ();

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var ca_filter_1 =
    ffact->create_filter (TCL_GRAMMAR);

  ACE_ASSERT (!CORBA::is_nil (ca_filter_1.in ()));

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var ca_filter_2 =
    ffact->create_filter (TCL_GRAMMAR);

  ACE_ASSERT (!CORBA::is_nil (ca_filter_2.in ()));

  /*  struct ConstraintExp {
                CosNotification::EventTypeSeq event_types;
                string constraint_expr;
                };
  */
  CosNotifyFilter::ConstraintExpSeq constraint_list (2);
  constraint_list.length (2);

  constraint_list[0].event_types.length (1);
  constraint_list[0].event_types[0].domain_name = CORBA::string_dup(DOMAIN_NAME);
  constraint_list[0].event_types[0].type_name = CORBA::string_dup(TYPE_NAME);

  constraint_list[0].constraint_expr = CORBA::string_dup (CA_FILTER);
  constraint_list[1].event_types.length (1);
  constraint_list[1].event_types[0].domain_name = CORBA::string_dup(DOMAIN_NAME);
  constraint_list[1].event_types[0].type_name = CORBA::string_dup(TYPE_NAME);
  constraint_list[1].constraint_expr = CORBA::string_dup (CA_FILTER);

  ca_filter_1->add_constraints (constraint_list);
  ca_filter_2->add_constraints (constraint_list);

  consumer_admin_1_->add_filter (ca_filter_1.in ());

  consumer_admin_2_->add_filter (ca_filter_2.in ());
}

void
FilterClient::get_consumeradmin ()
{
  CosNotifyChannelAdmin::AdminIDSeq_var ids
    = ec_->get_all_consumeradmins();

  ACE_ASSERT (ids->length () == 2);

  this->adminid_1_id_ = ids[0];
  this->adminid_2_id_ = ids[1];

  consumer_admin_1_ = ec_->get_consumeradmin (this->adminid_1_id_);
  consumer_admin_2_ = ec_->get_consumeradmin (this->adminid_2_id_);

  ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t)get_consumeradmin %d %d\n"),
        adminid_1_id_, adminid_2_id_));

  CosNotifyFilter::FilterAdmin_var admin
    = CosNotifyFilter::FilterAdmin::_narrow (consumer_admin_1_.in ());
  verify_filter (admin, CA_FILTER, MOD_CA_FILTER);
  admin = CosNotifyFilter::FilterAdmin::_narrow (consumer_admin_2_.in ());
  verify_filter (admin, CA_FILTER, MOD_CA_FILTER);
}


void
FilterClient::verify_filter (CosNotifyFilter::FilterAdmin_var& admin,
                             const char* constraint_expr,
                             const char* mod_constraint_expr)
{
  ACE_UNUSED_ARG (constraint_expr);
  // only used to validate assert, which is
  // compiled out for nondebug builds.
  CosNotifyFilter::FilterIDSeq_var ids
    = admin->get_all_filters ();

  ACE_ASSERT (ids->length () == 1);

  CosNotifyFilter::Filter_var filter
    = admin->get_filter (ids[0]);

  ACE_ASSERT (! CORBA::is_nil (filter.in ()));

  CosNotifyFilter::ConstraintInfoSeq_var infos
    = filter->get_all_constraints();

  ACE_ASSERT (infos->length () == 2);

  u_int index = 0;

  for (index = 0; index < infos->length (); ++ index)
  {
    CosNotifyFilter::ConstraintID id = infos[index].constraint_id;
    ACE_UNUSED_ARG (id);
    // only used to validate assert, which is
    // compiled out for nondebug builds.
    ACE_ASSERT (id != 0);
    ACE_ASSERT (ACE_OS::strcmp (infos[index].constraint_expression.constraint_expr.in (), constraint_expr) == 0);

    CosNotification::EventTypeSeq& events = infos[index].constraint_expression.event_types;
    ACE_UNUSED_ARG (events);
    // only used to validate assert, which is
    // compiled out for nondebug builds.
    ACE_ASSERT (events.length () == 1);

    ACE_ASSERT (ACE_OS::strcmp (events[0].domain_name.in (), DOMAIN_NAME) == 0);
    ACE_ASSERT (ACE_OS::strcmp (events[0].type_name.in (), TYPE_NAME) == 0);
  }

  if (modify_constraint_)
  {
    CosNotifyFilter::ConstraintIDSeq_var ids = new CosNotifyFilter::ConstraintIDSeq (2);
    ids->length (2);
    for (index = 0; index < infos->length (); ++ index)
    {
      ids[index] = infos[index].constraint_id;
      ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t)modify constraint %d \n"), ids[index]));

      infos[index].constraint_expression.constraint_expr
        = CORBA::string_dup (mod_constraint_expr);
    }

    filter->modify_constraints (ids.in (), infos.in());
  }
}


void
FilterClient::create_consumers ()
{
  // startup the first consumer.
  ACE_NEW_THROW_EX (consumer_1,
                    Filter_StructuredPushConsumer (this, "consumer1"),
                    CORBA::NO_MEMORY ());

  consumer_1->connect (consumer_admin_1_.in ());

  // startup the second consumer.
  ACE_NEW_THROW_EX (consumer_2,
                    Filter_StructuredPushConsumer (this, "consumer2"),
                    CORBA::NO_MEMORY ());

  consumer_2->connect (consumer_admin_2_.in ());
}

void
FilterClient::create_suppliers ()
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
FilterClient::send_events ()
{
  // operations:
  CosNotification::StructuredEvent event;

  // EventHeader

  // FixedEventHeader
  // EventType
  // string
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup(DOMAIN_NAME);
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup(TYPE_NAME);
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
Filter_StructuredPushConsumer::disconnect ()
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier();
}

void
Filter_StructuredPushConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/)
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
                ACE_TEXT ("%C received event, %d\n"),
    my_name_.fast_rep (), val));

    ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("event count %d\n"), g_result_count.value ()));

    if (++g_result_count == EVENTS_EXPECTED_TO_RECEIVE)
      this->filter_->done (); // all events received, we're done.
}

void
Filter_StructuredPushConsumer::disconnect_structured_push_consumer ()
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
Filter_StructuredPushSupplier::disconnect ()
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer();
}

void
Filter_StructuredPushSupplier::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */)
{
  //No-Op.
}

void
Filter_StructuredPushSupplier::send_event
   (const CosNotification::StructuredEvent& event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%C is sending an event \n"), my_name_.fast_rep ()));

  proxy_consumer_->push_structured_event (event);
}

void
Filter_StructuredPushSupplier::disconnect_structured_push_supplier ()
{
  // No-Op.
}


