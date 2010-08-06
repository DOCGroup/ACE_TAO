// $Id$

// EchoEventSupplierMain.cpp
// Main program for a PushSupplier of Echo events.

#include "EchoEventSupplier_i.h"

#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "ace/Thread_Manager.h"
#include <iostream>
#include <fstream>

const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

const int EVENT_DELAY_MS = 10;

ACE_THR_FUNC_RETURN orb_thread(void *orb_ptr)
{
  CORBA::ORB_var orb = CORBA::ORB::_duplicate((CORBA::ORB_ptr) orb_ptr);
  orb->run();
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the EC Factory so we can customize the EC
    TAO_EC_Default_Factory::init_svcs ();

    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    const ACE_TCHAR* ecname = ACE_TEXT ("EventService");
    const ACE_TCHAR* remote_ecname = 0;
    const ACE_TCHAR* iorfile = 0;
    for (int i = 0; argv[i] != 0; i++) {
      if (ACE_OS::strcmp(argv[i], ACE_TEXT("-ecname")) == 0) {
        if (argv[i+1] != 0) {
          i++;
          ecname = argv[i];
        } else {
          std::cerr << "Missing Event channel name" << std::endl;
        }
      }
      if (ACE_OS::strcmp(argv[i], ACE_TEXT("-gateway")) == 0) {
        if (argv[i+1] != 0) {
          i++;
          remote_ecname = argv[i];
        } else {
          std::cerr << "Missing Event channel name" << std::endl;
        }
      }
      if (ACE_OS::strcmp(argv[i], ACE_TEXT("-iorfile")) == 0) {
        if (argv[i+1] != 0) {
          i++;
          iorfile = argv[i];
        }
      }
    }

    // Get the POA
    CORBA::Object_var object = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (object.in ());
    PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
    poa_manager->activate ();

    // Spawn a thread for the orb
    ACE_Thread_Manager *thread_mgr = ACE_Thread_Manager::instance();
    thread_mgr->spawn(orb_thread, orb.in());

    // Create a local event channel and register it with the RootPOA.
    TAO_EC_Event_Channel_Attributes attributes (poa.in (), poa.in ());
    TAO_EC_Event_Channel ec_impl (attributes);
    ec_impl.activate ();
    PortableServer::ObjectId_var oid = poa->activate_object(&ec_impl);
    CORBA::Object_var ec_obj = poa->id_to_reference(oid.in());
    RtecEventChannelAdmin::EventChannel_var ec =
      RtecEventChannelAdmin::EventChannel::_narrow(ec_obj.in());

    // Find the Naming Service.
    object = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context = CosNaming::NamingContextExt::_narrow(object.in());
    CosNaming::Name_var name = root_context->to_name (ACE_TEXT_ALWAYS_CHAR (ecname));
    root_context->rebind(name.in(), ec.in());

    // Get a SupplierAdmin object from the EventChannel.
    RtecEventChannelAdmin::SupplierAdmin_var admin = ec->for_suppliers();

    // Get a ProxyPushConsumer from the SupplierAdmin.
    RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
                                        admin->obtain_push_consumer();

    // Instantiate an EchoEventSupplier_i servant.
    EchoEventSupplier_i servant(orb.in());

    // Register it with the RootPOA.
    oid = poa->activate_object(&servant);
    CORBA::Object_var supplier_obj = poa->id_to_reference(oid.in());
    RtecEventComm::PushSupplier_var supplier =
      RtecEventComm::PushSupplier::_narrow(supplier_obj.in());

    // Publish the events the supplier provides.
    ACE_SupplierQOS_Factory qos;
    qos.insert (MY_SOURCE_ID,      // Supplier's unique id
                MY_EVENT_TYPE,     // Event type
                0,                 // handle to the rt_info structure
                1);                // number of calls

    // Connect as a supplier of the published events.
    consumer->connect_push_supplier (supplier.in (),
                                     qos.get_SupplierQOS ());

    // Create an event (just a string in this case).
    const CORBA::String_var eventData = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (ecname));

    // Create an event set for one event
    RtecEventComm::EventSet event (1);
    event.length (1);
    // Initialize event header.
    event[0].header.source = MY_SOURCE_ID;
    event[0].header.ttl = 1;
    event[0].header.type = MY_EVENT_TYPE;
    // Initialize data fields in event.
    event[0].data.any_value <<= eventData;

    TAO_EC_Gateway_IIOP gateway;
    int gateway_initialized = 0;

    std::cout << "Supplier starting sending of events.\n";

    while (1) {

      consumer->push (event);
      ACE_Time_Value tv(0, 1000 * EVENT_DELAY_MS);
      orb->run(tv);

      if ((remote_ecname != 0)  && (!gateway_initialized)) {

        try {
          // Get the remote event channel object
          CORBA::Object_var obj = root_context->resolve_str (ACE_TEXT_ALWAYS_CHAR (remote_ecname));
          RtecEventChannelAdmin::EventChannel_var remote_ec =
            RtecEventChannelAdmin::EventChannel::_narrow(obj.in());

          int ok = 0;
          if (!CORBA::is_nil(remote_ec.in())) {
            // Now check if we can talk to it...
            try {
              RtecEventChannelAdmin::SupplierAdmin_var adm =
                remote_ec->for_suppliers();
              ok = 1;
            } catch(const CORBA::UserException&) {
              // What is the correct exception(s) to catch here?
            }
          }

          // There is a good remote event channel so initialize the
          // gateway.
          if (ok) {
            gateway.init(remote_ec.in(), ec.in());

            PortableServer::ObjectId_var gateway_oid =
              poa->activate_object(&gateway);
            CORBA::Object_var gateway_obj =
              poa->id_to_reference(gateway_oid.in());
            RtecEventChannelAdmin::Observer_var obs =
              RtecEventChannelAdmin::Observer::_narrow(gateway_obj.in());
            RtecEventChannelAdmin::Observer_Handle local_ec_obs_handle =
              ec->append_observer (obs.in ());
            ACE_UNUSED_ARG (local_ec_obs_handle);
            gateway_initialized = 1;
            std::cout << "Gateway initialized\n";
            if (iorfile != 0) {
              CORBA::String_var str = orb->object_to_string( ec.in() );
              std::ofstream iorFile( ACE_TEXT_ALWAYS_CHAR(iorfile) );
              iorFile << str.in() << std::endl;
              iorFile.close();
            }
          }
        } catch(const CosNaming::NamingContext::NotFound&) {
          // Try again later...
        }
      }
    }

    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& exc)
  {
    std::cerr << "Caught CORBA::Exception" << std::endl << exc << std::endl;
  }

  return 1;
}
