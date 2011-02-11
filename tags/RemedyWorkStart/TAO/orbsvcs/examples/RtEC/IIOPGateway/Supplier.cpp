// $Id$

#include "Supplier.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

static const ACE_TCHAR *ecname = 0;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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
      // First parse our command line options
      if (this->parse_args(argc, argv) != 0)
      {
         return -1;
      }

      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

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

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
        supplier_admin->obtain_push_consumer ();

      RtecEventComm::PushSupplier_var supplier =
        this->_this ();

      // Publish the events the supplier provides.
      ACE_SupplierQOS_Factory qos;
      qos.insert (MY_SOURCE_ID,      // Supplier's unique id
                  MY_EVENT_TYPE,     // Event type
                  0,                 // handle to the rt_info structure
                  1);                // number of calls

      consumer->connect_push_supplier (supplier.in (), qos);

      // Push the events...
      ACE_Time_Value sleep_time (0, 10000); // 10 milliseconds

      RtecEventComm::EventSet event (1);
      event.length (1);
      for (int j = 0; j < 1; j++)
        {
           event[j].header.source = MY_SOURCE_ID;
           event[j].header.ttl = 1;
           event[j].header.type = MY_EVENT_TYPE;
        }

      for (int i = 1; i != 4000; ++i)
        {
          if (i % 100 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                         "Supplier (%P|%t): %d events send\n",
                         i));
            }
          consumer->push (event);
          ACE_OS::sleep (sleep_time);
        }

      // Disconnect from the EC
      consumer->disconnect_push_consumer ();

      // Destroy the EC....
      event_channel->destroy ();

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

int
Supplier::parse_args (int argc, ACE_TCHAR *argv[])
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

  // Indicates successful parsing of the command line
  return 0;
}

