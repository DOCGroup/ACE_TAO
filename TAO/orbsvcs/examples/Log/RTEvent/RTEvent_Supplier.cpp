#include "RTEvent_Supplier.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"

constexpr char naming_service_name[] = "NameService";
constexpr char event_tls_log_factory_name[] = "RTEventLogFactory";
constexpr int log_event_count = 29;
constexpr char query_1[] = "id > 0";
constexpr char query_2[] = "id >= 0";
constexpr char query_lang[] = "TCL";

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Supplier supplier;

  return supplier.run (argc, argv);
}

int
Supplier::run (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var const poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      CORBA::Object_var const naming_obj =
        orb->resolve_initial_references (naming_service_name);

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        throw CORBA::UNKNOWN ();

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (event_tls_log_factory_name);

      CORBA::Object_var const obj =
        this->naming_context_->resolve (name);

      this->event_log_factory_ =
        RTEventLogAdmin::EventLogFactory::_narrow (obj.in ());

      ACE_ASSERT (!CORBA::is_nil (this->event_log_factory_.in ()));

      // Create a log.

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nCalling EventLogFactory::create...\n")));

      DsLogAdmin::LogFullActionType const logfullaction = DsLogAdmin::halt;
      DsLogAdmin::CapacityAlarmThresholdList threshold {};
      CORBA::ULongLong const max_size {}; // 0 means "infinite"

      DsLogAdmin::LogId logid {};

      RTEventLogAdmin::EventLog_var const event_log =
        this->event_log_factory_->create (logfullaction,
                                          max_size,
                                          threshold,
                                          logid);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Create returned logid = %u\n"), logid));

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var const supplier_admin =
        event_log->for_suppliers ();

      this->consumer_ =
        supplier_admin->obtain_push_consumer ();

      RtecEventComm::PushSupplier_var const supplier =
        this->_this ();

      // Simple publication, but usually the helper classes in
      // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
      RtecEventChannelAdmin::SupplierQOS qos {};
      qos.publications.length (1);
      RtecEventComm::EventHeader &h0 =
        qos.publications[0].event.header;
      h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
      h0.source = 1;                      // first free event source

      this->consumer_->connect_push_supplier (supplier.in (), qos);
      // Create some fake log events.
      // Push the events...
      ACE_Time_Value sleep_time (0, 10000); // 10 milliseconds

      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event[0].header.source = 1;
      event[0].header.ttl    = 1;

      for (int i {}; i != log_event_count; ++i)
        {
          this->consumer_->push (event);
          ACE_OS::sleep (sleep_time);
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Writing %d records...\n"), log_event_count));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Calling EventLog::get_n_records...\n")));
      CORBA::ULongLong const record_count = event_log->get_n_records ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Number of records in Log = %Q\n"), record_count));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Calling EventLog::get_current_size...\n")));
      CORBA::ULongLong const current_size = event_log->get_current_size ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Size of data in Log = %Q\n"), current_size));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Querying the Log: %C\n"), query_1));
      DsLogAdmin::Iterator_var iter_out;
      DsLogAdmin::RecordList_var const rec_list =
        event_log->query (query_lang, query_1, iter_out);

      for (CORBA::ULong j {}; j < rec_list->length (); ++j)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("id = %Q, time= %Q\n"),
                    rec_list[j].id, rec_list[j].time));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Deleting records...\n")));

      event_log->delete_records (query_lang, query_2);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Calling EventLog::get_n_records...\n")));
      CORBA::ULongLong const remaining_records = event_log->get_n_records ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Number of records in Log after delete = %Q\n"),
                  remaining_records));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Getting the current_size again...\n")));
      CORBA::ULongLong const remaining_size = event_log->get_current_size ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Size of data in Log = %Q\n"), remaining_size));

      // Disconnect from the EC.
      this->consumer_->disconnect_push_consumer ();

      // Destroy the event log.
      event_log->destroy ();

      // Deactivate this object.
      PortableServer::ObjectId_var const id =
        poa->servant_to_id (this);
      poa->deactivate_object (id.in ());

      // Destroy the POA.
      poa->destroy (true, false);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Supplier::run");
      return 1;
    }
  return 0;
}

void
Supplier::disconnect_push_supplier ()
{
}

