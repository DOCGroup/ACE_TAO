// EchoEventSupplierMain.cpp
// Main program for a PushSupplier of Echo events.

#include "EchoEventSupplier_i.h"

#include "ace/OS_NS_string.h"
#include "ace/Time_Value.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "ace/Thread_Manager.h"

#include <iostream>
#include <fstream>

constexpr RtecEventComm::EventSourceID MY_SOURCE_ID = ACE_ES_EVENT_SOURCE_ANY + 1;
constexpr RtecEventComm::EventType MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

constexpr int EVENT_DELAY_MS = 10;

ACE_THR_FUNC_RETURN orb_thread (void *orb_ptr)
{
  CORBA::ORB_var const orb = CORBA::ORB::_duplicate (static_cast<CORBA::ORB_ptr> (orb_ptr));
  orb->run ();
  return ACE_THR_FUNC_RETURN_NULL;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  struct Command_Line_Options
  {
    ACE_TCHAR const *event_channel_name {ACE_TEXT ("EventService")};
    ACE_TCHAR const *gateway_event_channel_name {};
    ACE_TCHAR const *ior_file_name {};
  };

  try
  {
    // Initialize the EC Factory so we can customize the EC
    TAO_EC_Default_Factory::init_svcs ();

    // Initialize the ORB.
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    Command_Line_Options options {};
    for (int i = 1; i < argc; ++i)
      {
        ACE_TCHAR const * const option = argv[i];
        if (ACE_OS::strcmp (option, ACE_TEXT ("-ecname")) == 0)
          {
            if (++i < argc)
              {
                options.event_channel_name = argv[i];
              }
            else
              {
                std::cerr << "Missing Event channel name" << std::endl;
                return 1;
              }
          }
        else if (ACE_OS::strcmp (option, ACE_TEXT ("-gateway")) == 0)
          {
            if (++i < argc)
              {
                options.gateway_event_channel_name = argv[i];
              }
            else
              {
                std::cerr << "Missing remote Event channel name" << std::endl;
                return 1;
              }
          }
        else if (ACE_OS::strcmp (option, ACE_TEXT ("-iorfile")) == 0)
          {
            if (++i < argc)
              {
                options.ior_file_name = argv[i];
              }
            else
              {
                std::cerr << "Missing IOR file name" << std::endl;
                return 1;
              }
          }
      }

    // Get the POA
    CORBA::Object_var object = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const poa = PortableServer::POA::_narrow (object.in ());
    PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
    poa_manager->activate ();

    // Spawn a thread for the orb
    ACE_Thread_Manager * const thread_mgr = ACE_Thread_Manager::instance ();
    thread_mgr->spawn (orb_thread, orb.in ());

    // Create a local event channel and register it with the RootPOA.
    TAO_EC_Event_Channel_Attributes attributes (poa.in (), poa.in ());
    PortableServer::Servant_var<TAO_EC_Event_Channel> const ec_impl =
      new TAO_EC_Event_Channel (attributes);
    ec_impl->activate ();
    PortableServer::ObjectId_var const ec_oid = poa->activate_object (ec_impl.in ());
    CORBA::Object_var const ec_obj = poa->id_to_reference (ec_oid.in ());
    RtecEventChannelAdmin::EventChannel_var const ec =
      RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ());

    // Find the Naming Service.
    object = orb->resolve_initial_references ("NameService");
    CosNaming::NamingContextExt_var const root_context =
      CosNaming::NamingContextExt::_narrow (object.in ());
    CosNaming::Name_var const name =
      root_context->to_name (ACE_TEXT_ALWAYS_CHAR (options.event_channel_name));
    root_context->rebind (name.in (), ec.in ());

    // Get a SupplierAdmin object from the EventChannel.
    RtecEventChannelAdmin::SupplierAdmin_var const admin = ec->for_suppliers ();

    // Get a ProxyPushConsumer from the SupplierAdmin.
    RtecEventChannelAdmin::ProxyPushConsumer_var const consumer =
      admin->obtain_push_consumer ();

    // Instantiate an EchoEventSupplier_i servant.
    PortableServer::Servant_var<EchoEventSupplier_i> const servant =
      new EchoEventSupplier_i (orb.in ());

    // Register it with the RootPOA.
    PortableServer::ObjectId_var const supplier_oid = poa->activate_object (servant.in ());
    CORBA::Object_var const supplier_obj = poa->id_to_reference (supplier_oid.in ());
    RtecEventComm::PushSupplier_var const supplier =
      RtecEventComm::PushSupplier::_narrow (supplier_obj.in ());

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
    CORBA::String_var const event_data =
      CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (options.event_channel_name));

    // Create an event set for one event
    RtecEventComm::EventSet event (1);
    event.length (1);
    // Initialize event header.
    event[0].header.source = MY_SOURCE_ID;
    event[0].header.ttl = 1;
    event[0].header.type = MY_EVENT_TYPE;
    // Initialize data fields in event.
    event[0].data.any_value <<= event_data;

    PortableServer::Servant_var<TAO_EC_Gateway_IIOP> const gateway =
      new TAO_EC_Gateway_IIOP;
    bool gateway_initialized {};

    std::cout << "Supplier starting sending of events.\n";

    while (true)
      {
        consumer->push (event);
        ACE_Time_Value tv (0, 1000 * EVENT_DELAY_MS);
        orb->run (tv);

        if (options.gateway_event_channel_name != nullptr && !gateway_initialized)
          {
            try
              {
                CORBA::Object_var remote_obj = root_context->resolve_str (
                  ACE_TEXT_ALWAYS_CHAR (options.gateway_event_channel_name));
                RtecEventChannelAdmin::EventChannel_var const remote_ec =
                  RtecEventChannelAdmin::EventChannel::_narrow (remote_obj.in ());

                bool can_connect {};
                if (!CORBA::is_nil (remote_ec.in ()))
                  {
                    try
                      {
                        RtecEventChannelAdmin::SupplierAdmin_var const remote_admin =
                          remote_ec->for_suppliers ();
                        can_connect = true;
                      }
                    catch (CORBA::UserException const &)
                      {
                        // The remote channel may not be ready to accept suppliers yet.
                      }
                  }

                if (can_connect)
                  {
                    gateway->init (remote_ec.in (), ec.in ());

                    PortableServer::ObjectId_var const gateway_oid =
                      poa->activate_object (gateway.in ());
                    CORBA::Object_var const gateway_obj =
                      poa->id_to_reference (gateway_oid.in ());
                    RtecEventChannelAdmin::Observer_var const observer =
                      RtecEventChannelAdmin::Observer::_narrow (gateway_obj.in ());
                    RtecEventChannelAdmin::Observer_Handle const observer_handle =
                      ec->append_observer (observer.in ());
                    ACE_UNUSED_ARG (observer_handle);
                    gateway_initialized = true;
                    std::cout << "Gateway initialized\n";
                    if (options.ior_file_name != nullptr)
                      {
                        CORBA::String_var const ior = orb->object_to_string (ec.in ());
                        std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (options.ior_file_name));
                        ior_file << ior.in () << std::endl;
                      }
                  }
              }
            catch (CosNaming::NamingContext::NotFound const &)
              {
                // Try again after the next event dispatch interval.
              }
          }
      }
  }
  catch (CORBA::Exception const &exc)
    {
      std::cerr << "Caught CORBA::Exception" << std::endl << exc << std::endl;
    }

  return 1;
}
