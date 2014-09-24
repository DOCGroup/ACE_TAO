// $Id$

#include "Notify_Supplier.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "ace/OS_main.h"

#define NAMING_SERVICE_NAME "NameService"
#define NOTIFY_TLS_LOG_FACTORY_NAME "NotifyLogFactory"
#define LOG_EVENT_COUNT 10
#define QUERY_1 "id > 0"
#define QUERY_2 "id >= 0"
#define QUERY_LANG "TCL"
#define SA_FILTER "threshold > 10"
#define TCL_GRAMMAR "TCL"
#define EVENTS_TO_SEND 10

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Supplier supplier;

  return supplier.run (argc, argv);
}

// ****************************************************************

Supplier::Supplier (void)
{
}

Supplier::~Supplier ()
{

}

int
Supplier::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // ORB initialization boiler plate...
      this->orb_ =
        CORBA::ORB_init (argc, argv);



      CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references (NAMING_SERVICE_NAME);

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        throw CORBA::UNKNOWN ();

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());


      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (NOTIFY_TLS_LOG_FACTORY_NAME);

      CORBA::Object_var obj =
        this->naming_context_->resolve (name);

      this->notify_log_factory_ =
        DsNotifyLogAdmin::NotifyLogFactory::_narrow (obj.in ());

      ACE_ASSERT (!CORBA::is_nil (this->notify_log_factory_.in ()));

      // create a log..

      ACE_DEBUG ((LM_DEBUG,
                  "\nCalling NotifyLogFactory::create...\n"));

      DsLogAdmin::LogFullActionType logfullaction = DsLogAdmin::halt;
      DsLogAdmin::CapacityAlarmThresholdList threshold = 0;
      CORBA::ULongLong max_size = 0; // 0 means "infinite"

      DsLogAdmin::LogId logid = 0;
      CosNotification::QoSProperties initial_qos;
      CosNotification::AdminProperties initial_admin;

      this->notify_log_ =
        this->notify_log_factory_->create (logfullaction,
                                          max_size,
                                          threshold,
                                          initial_qos,
                                          initial_admin,
                                          logid);


      ACE_DEBUG ((LM_DEBUG,
                  "Create returned logid = %d\n",logid));

      // Create some fake log events.
      DsLogAdmin::Anys any_seq (LOG_EVENT_COUNT);
      any_seq.length (LOG_EVENT_COUNT);

      //DsLogAdmin::RecordIdList id_seq (LOG_EVENT_COUNT);
      //id_seq.length (LOG_EVENT_COUNT);

      for (int i = 0; i < LOG_EVENT_COUNT; i++)
        {
          any_seq [i] <<= i;
        }

      CosNotifyChannelAdmin::AdminID adminID = 0;
      this->supplier_admin_ =
        notify_log_->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, adminID);
      ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

      CosNotifyFilter::FilterFactory_var ffact =
        notify_log_->default_filter_factory ();

      // setup a filter at the consumer admin
      CosNotifyFilter::Filter_var sa_filter =
        ffact->create_filter (TCL_GRAMMAR);

      ACE_ASSERT (!CORBA::is_nil (sa_filter.in ()));

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup (SA_FILTER);

      sa_filter->add_constraints (constraint_list);

      supplier_admin_->add_filter (sa_filter.in ());

      // startup the first supplier
      ACE_NEW_THROW_EX (supplier_1,
                        Filter_StructuredPushSupplier (),
                        CORBA::NO_MEMORY ());

      supplier_1->connect (supplier_admin_.in ());

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

      for (int k = 0; k < EVENTS_TO_SEND; k++)
      {
        event.filterable_data[0].value <<= (CORBA::Long)k;

        // any
        event.remainder_of_body <<= (CORBA::Long)k;

        supplier_1->send_event (event);
      }

      ACE_DEBUG ((LM_DEBUG,
                  "Calling NotifyLog get_n_records...\n"));

      CORBA::ULongLong retval = notify_log_->get_n_records ();

      ACE_DEBUG ((LM_DEBUG, "Number of records in Log = %d\n", retval));

      ACE_DEBUG ((LM_DEBUG,
                  "Calling NotifyLog::get_current_size...\n"));
       retval = notify_log_->get_current_size ();

      ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d\n", retval));

      ACE_DEBUG ((LM_DEBUG, "Querying the Log: %s\n", QUERY_1));
      DsLogAdmin::Iterator_var iter_out;
      DsLogAdmin::RecordList_var rec_list =
        notify_log_->query (QUERY_LANG, QUERY_1, iter_out);

      CORBA::ULong j = 0;
      for (; j < rec_list->length();++j)
      ACE_DEBUG ((LM_DEBUG,
                   "id = %Q, time= %Q\n",
                   rec_list[j].id, rec_list[j].time));

     ACE_DEBUG ((LM_DEBUG,
                 "Deleting records...\n"));

     retval = notify_log_->delete_records (QUERY_LANG, QUERY_2);

     ACE_DEBUG ((LM_DEBUG,
                 "Calling NotifyLog::get_n_records...\n"));
     retval = notify_log_->get_n_records ();

     ACE_DEBUG ((LM_DEBUG, "Number of records in Log after delete = %d\n",
                 retval));

     ACE_DEBUG ((LM_DEBUG, "Geting the current_size again...\n"));
     retval = notify_log_->get_current_size ();

     ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d\n", retval));

     this->notify_log_->destroy();

     }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Supplier::run");
      return 1;
    }
  return 0;
}


Filter_StructuredPushSupplier::Filter_StructuredPushSupplier  (void)
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
    const CosNotification::EventTypeSeq & /*removed */)
{
  //No-Op.
}

void
Filter_StructuredPushSupplier::send_event
   (const CosNotification::StructuredEvent& event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  proxy_consumer_->push_structured_event (event);
}

void
Filter_StructuredPushSupplier::disconnect_structured_push_supplier
   (void)
{
  // No-Op.
}

