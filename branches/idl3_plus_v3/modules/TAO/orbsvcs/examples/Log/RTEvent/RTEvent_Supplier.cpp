#include "RTEvent_Supplier.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID (RTEvent,
           RTEvent_Supplier,
           "$Id$")

#define NAMING_SERVICE_NAME "NameService"
#define EVENT_TLS_LOG_FACTORY_NAME "RTEventLogFactory"
#define LOG_EVENT_COUNT 29
#define QUERY_1 "id > 0"
#define QUERY_2 "id >= 0"
#define QUERY_LANG "TCL"

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

int
Supplier::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);


/*
      if (argc <= 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Supplier <event_channel_ior>\n"));
          return 1;
        }
*/

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the run() method.
     // this->orb_ = orb.in ();

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel, we could use a naming service, a
      // command line argument or resolve_initial_references(), but
      // this is simpler...
/*      object =
        orb->string_to_object (argv[1]);

      RtecEventChannelAdmin::EventChannel_var event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (object.in ());
*/

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references (NAMING_SERVICE_NAME);

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        throw CORBA::UNKNOWN ();

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());


      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (EVENT_TLS_LOG_FACTORY_NAME);

      CORBA::Object_var obj =
        this->naming_context_->resolve (name);

      this->event_log_factory_ =
        RTEventLogAdmin::EventLogFactory::_narrow (obj.in ());

      ACE_ASSERT (!CORBA::is_nil (this->event_log_factory_.in ()));





      // create a log..

      ACE_DEBUG ((LM_DEBUG,
                  "\nCalling EventLogFactory::create...\n"));

      DsLogAdmin::LogFullActionType logfullaction = DsLogAdmin::halt;
      DsLogAdmin::CapacityAlarmThresholdList threshold = 0;
      CORBA::ULongLong max_size = 0; // 0 means "infinite"

      DsLogAdmin::LogId logid = 0;

      RTEventLogAdmin::EventLog_var event_log =
      // DsLogAdmin::Log_var event_log =
        this->event_log_factory_->create (logfullaction,
                                          max_size,
                                          threshold,
                                          logid);


      ACE_DEBUG ((LM_DEBUG,
                  "Create returned logid = %d\n",logid));

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_log->for_suppliers ();

      this->consumer_ =
        supplier_admin->obtain_push_consumer ();

      RtecEventComm::PushSupplier_var supplier =
        this->_this ();


      // Simple publication, but usually the helper classes in
      // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
      RtecEventChannelAdmin::SupplierQOS qos;
      qos.publications.length (1);
      RtecEventComm::EventHeader& h0 =
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

      for (int i = 0; i != LOG_EVENT_COUNT; ++i)
        {
          this->consumer_->push (event);
          ACE_OS::sleep (sleep_time);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Writing %d records...\n", LOG_EVENT_COUNT));

      ACE_DEBUG ((LM_DEBUG,
                  "Calling EventLog::get_n_records...\n"));
#ifndef ACE_LACKS_LONGLONG_T
      CORBA::ULongLong retval = event_log->get_n_records ();
#else
      CORBA::Long retval = event_log->get_n_records ().lo();
#endif

      ACE_DEBUG ((LM_DEBUG, "Number of records in Log = %d\n", retval));

      ACE_DEBUG ((LM_DEBUG,
                  "Calling EventLog::get_current_size...\n"));
#ifndef ACE_LACKS_LONGLONG_T
      retval = event_log->get_current_size ();
#else
      retval = event_log->get_current_size ().lo();
#endif

      ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d\n", retval));

      ACE_DEBUG ((LM_DEBUG, "Querying the Log: %s\n", QUERY_1));
      DsLogAdmin::Iterator_var iter_out;
      DsLogAdmin::RecordList_var rec_list =
        event_log->query (QUERY_LANG, QUERY_1, iter_out);

      CORBA::ULong j = 0;
      for (; j < rec_list->length();++j)
#ifndef ACE_LACKS_LONGLONG_T
      ACE_DEBUG ((LM_DEBUG,
                  "id = %Q, time= %Q\n",
                  rec_list[j].id, rec_list[j].time));
#else
      ACE_DEBUG ((LM_DEBUG,
                  "id = %u, time= %u\n",
                  rec_list[j].id.lo(), rec_list[j].time.lo()));
#endif

      ACE_DEBUG ((LM_DEBUG,
                  "Deleting records...\n"));

      retval = event_log->delete_records (QUERY_LANG, QUERY_2);

      ACE_DEBUG ((LM_DEBUG,
                  "Calling EventLog::get_n_records...\n"));
#ifndef ACE_LACKS_LONGLONG_T
      retval = event_log->get_n_records ();
#else
      retval = event_log->get_n_records ().lo();
#endif

      ACE_DEBUG ((LM_DEBUG, "Number of records in Log after delete = %d\n",
                  retval));

      ACE_DEBUG ((LM_DEBUG, "Geting the current_size again...\n"));
#ifndef ACE_LACKS_LONGLONG_T
      retval = event_log->get_current_size ();
#else
      retval = event_log->get_current_size ().lo();
#endif

      ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d\n", retval));

     // Disconnect from the EC
     this->consumer_->disconnect_push_consumer ();

     // Destroy the EC....
     //event_channel->destroy ();
     event_log->destroy ();

     // Deactivate this object...
     PortableServer::ObjectId_var id =
       poa->servant_to_id (this);
     poa->deactivate_object (id.in ());

     // Destroy the POA
     poa->destroy (1, 0);


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Supplier::run");
      return 1;
    }
  return 0;
}

void
Supplier::disconnect_push_supplier (void)
{
}

