// $Id$

#include "Consumer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_string.h"

const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

static const ACE_TCHAR *ecname = 0;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Consumer consumer;

  return consumer.run (argc, argv);
}

// ****************************************************************

Consumer::Consumer (void)
  : event_count_ (0)
{
}

int
Consumer::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // First parse our command line options
      if (this->parse_args(argc, argv) != 0)
      {
         return -1;
      }

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

      // Obtain the event channel from the naming service
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(ecname));

      CORBA::Object_var ec_obj =
        naming_context->resolve (name);

      RtecEventChannelAdmin::EventChannel_var event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ());

      if (CORBA::is_nil (event_channel.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get Event Channel.\n"),
                          1);

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

      RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
        consumer_admin->obtain_push_supplier ();

      RtecEventComm::PushConsumer_var consumer =
        this->_this ();

      ACE_ConsumerQOS_Factory qos;
      qos.start_disjunction_group ();
      qos.insert (MY_SOURCE_ID,   // Source ID
                  MY_EVENT_TYPE,  // Event Type
                  0);             // handle to the rt_info
      for (int i = 0; i < 10; i++)
        {
          qos.insert (MY_SOURCE_ID  + i,   // Source ID
                      MY_EVENT_TYPE + i,  // Event Type
                      0);             // handle to the rt_info
        }
      supplier->connect_push_consumer (consumer.in (), qos);

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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Consumer::run");
      return 1;
    }
  return 0;
}

void
Consumer::push (const RtecEventComm::EventSet& events)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t) no events\n"));
      return;
    }

  this->event_count_ += events.length ();
  if (this->event_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t): %d events received\n",
                  this->event_count_));
    }
}

void
Consumer::disconnect_push_consumer (void)
{
  // In this example we shutdown the ORB when we disconnect from the
  // EC (or rather the EC disconnects from us), but this doesn't have
  // to be the case....
  this->orb_->shutdown (0);
}

int
Consumer::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, ACE_TEXT("-e")) == 0)
        {
          arg_shifter.consume_arg ();
          ecname = arg_shifter.get_current ();
        }

      arg_shifter.ignore_arg ();
    }

  // Indicates sucessful parsing of the command line
  return 0;
}

