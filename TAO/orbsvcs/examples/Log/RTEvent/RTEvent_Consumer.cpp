#include "RTEvent_Consumer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "ace/OS_main.h"

constexpr char naming_service_name[] = "NameService";
constexpr char event_tls_log_factory_name[] = "RTEventLogFactory";

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Consumer consumer;

  return consumer.run (argc, argv);
}

int
Consumer::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the run() method.
      this->orb_ = orb.in ();
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel.
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references (naming_service_name);

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        throw CORBA::UNKNOWN ();

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (event_tls_log_factory_name);

      CORBA::Object_var obj =
        this->naming_context_->resolve (name);

      this->event_log_factory_ =
        RTEventLogAdmin::EventLogFactory::_narrow (obj.in ());

      // The canonical protocol to connect to the EC

      this->supplier_ =
        this->event_log_factory_->obtain_push_supplier ();

      RtecEventComm::PushConsumer_var consumer =
        this->_this ();

      // Simple subscription, but usually the helper classes in
      // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
      RtecEventChannelAdmin::ConsumerQOS qos {};
      qos.dependencies.length (2);
      RtecEventComm::EventHeader& h0 =
        qos.dependencies[0].event.header;
      h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
      h0.source = ACE_ES_EVENT_SOURCE_ANY;

      RtecEventComm::EventHeader& h1 =
        qos.dependencies[1].event.header;
      h1.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
      h1.source = ACE_ES_EVENT_SOURCE_ANY;

      this->supplier_->connect_push_consumer (consumer.in (), qos);

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();

      // We don't do any cleanup, it is hard to do it after shutdown,
      // and would complicate the example; plus it is almost
      // impossible to do cleanup after ORB->run() because the POA is
      // in the holding state.  Applications should use
      // work_pending()/perform_work() to do more interesting stuff.
      // Check the supplier for the proper way to do cleanup.
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Consumer::run");
      return 1;
    }
  return 0;
}

void
Consumer::push (RtecEventComm::EventSet const &events)
{
  ACE_UNUSED_ARG (events);

  ++this->event_count_;

  ACE_DEBUG ((LM_DEBUG,
              "Consumer (%P|%t): %d log generated events received\n",
              this->event_count_));
}

void
Consumer::disconnect_push_consumer ()
{
  // In this example we shutdown the ORB when we disconnect from the
  // EC (or rather the EC disconnects from us), but this doesn't have
  // to be the case....
  this->orb_->shutdown (false);
}

