// EchoEventSupplierMain.cpp
// Main program for a PushSupplier of Echo events.

#include "EchoEventSupplier_i.h"
#include "SimpleAddressServer.h"

#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.h"
#include "orbsvcs/Event/ECG_UDP_EH.h"

#include "tao/ORB_Core.h"
#include "ace/Time_Value.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <utility>

constexpr RtecEventComm::EventSourceID MY_SOURCE_ID = ACE_ES_EVENT_SOURCE_ANY + 1;
constexpr RtecEventComm::EventType MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

constexpr int EVENT_DELAY_MS = 10;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  struct Command_Line_Options
  {
    ACE_TCHAR const *event_channel_name { ACE_TEXT ("EventService") };
    ACE_TCHAR const *address { ACE_TEXT ("localhost") };
    ACE_TCHAR const *ior_file {};
    u_short port { 12345 };
    u_short listen_port { 12345 };
    bool multicast { true };
  };

  try
  {
    // Initialize the EC Factory so we can customize the EC
    TAO_EC_Default_Factory::init_svcs ();

    // Initialize the ORB.
    Command_Line_Options options;
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    for (int i = 0; argv[i] != nullptr; ++i)
      {
        if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-ecname")) == 0)
          {
            if (argv[i + 1] != nullptr)
              {
                options.event_channel_name = argv[++i];
              }
            else
              {
                std::cerr << "Missing Event channel name" << std::endl;
              }
          }
        else if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-address")) == 0)
          {
            if (argv[i + 1] != nullptr)
              {
                options.address = argv[++i];
              }
            else
              {
                std::cerr << "Missing address" << std::endl;
              }
          }
        else if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-port")) == 0)
          {
            if (argv[i + 1] != nullptr)
              {
                options.port = ACE_OS::atoi (argv[++i]);
              }
            else
              {
                std::cerr << "Missing port" << std::endl;
              }
          }
        else if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-listenport")) == 0)
          {
            if (argv[i + 1] != nullptr)
              {
                options.listen_port = ACE_OS::atoi (argv[++i]);
              }
            else
              {
                std::cerr << "Missing port" << std::endl;
              }
          }
        else if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-iorfile")) == 0)
          {
            if (argv[i + 1] != nullptr)
              {
                options.ior_file = argv[++i];
              }
          }
        else if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-udp")) == 0)
          {
            options.multicast = false;
          }
      }

    // Get the POA
    CORBA::Object_var object = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const poa = PortableServer::POA::_narrow (object.in ());
    PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
    poa_manager->activate ();

    // Create a local event channel and register it
    TAO_EC_Event_Channel_Attributes attributes (poa.in (), poa.in ());
    PortableServer::Servant_var<TAO_EC_Event_Channel> const ec_impl =
      new TAO_EC_Event_Channel (attributes);
    ec_impl->activate ();
    PortableServer::ObjectId_var oid = poa->activate_object (ec_impl.in ());
    CORBA::Object_var const ec_obj = poa->id_to_reference (oid.in ());
    RtecEventChannelAdmin::EventChannel_var const ec =
      RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ());

    // Find the Naming Service.
    CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
    CosNaming::NamingContextExt_var const root_context =
      CosNaming::NamingContextExt::_narrow (obj.in ());

    // Bind the Event Channel using Naming Services
    CosNaming::Name_var const name =
      root_context->to_name (ACE_TEXT_ALWAYS_CHAR (options.event_channel_name));
    root_context->rebind (name.in (), ec.in ());

    // Get a proxy push consumer from the EventChannel.
    RtecEventChannelAdmin::SupplierAdmin_var const admin = ec->for_suppliers ();
    RtecEventChannelAdmin::ProxyPushConsumer_var const consumer = admin->obtain_push_consumer ();

    // Instantiate an EchoEventSupplier_i servant.
    PortableServer::Servant_var<EchoEventSupplier_i> const servant =
      new EchoEventSupplier_i (orb.in ());

    // Register it with the RootPOA.
    oid = poa->activate_object (servant.in ());
    CORBA::Object_var const supplier_obj = poa->id_to_reference (oid.in ());
    RtecEventComm::PushSupplier_var const supplier =
      RtecEventComm::PushSupplier::_narrow (supplier_obj.in ());

    // Connect to the EC.
    ACE_SupplierQOS_Factory qos;
    qos.insert (MY_SOURCE_ID, MY_EVENT_TYPE, 0, 1);
    consumer->connect_push_supplier (supplier.in (), qos.get_SupplierQOS ());

    // Initialize the address server with the desired address.
    // This will be used by the sender object and the multicast
    // receiver.
    ACE_INET_Addr send_addr (options.port, options.address);
    PortableServer::Servant_var<SimpleAddressServer> addr_srv_impl =
      new SimpleAddressServer (send_addr);

    PortableServer::ObjectId_var addr_srv_oid =
      poa->activate_object (addr_srv_impl.in ());
    CORBA::Object_var const addr_srv_obj = poa->id_to_reference (addr_srv_oid.in ());
    RtecUDPAdmin::AddrServer_var const addr_srv =
      RtecUDPAdmin::AddrServer::_narrow (addr_srv_obj.in ());

    // Create and initialize the sender object
    PortableServer::Servant_var<TAO_ECG_UDP_Sender> const sender =
      TAO_ECG_UDP_Sender::create ();
    TAO_ECG_UDP_Out_Endpoint endpoint;
    if (endpoint.dgram ().open (ACE_Addr::sap_any) == -1)
      {
        std::cerr << "Cannot open send endpoint" << std::endl;
        return 1;
      }

    // TAO_ECG_UDP_Sender::init() takes a TAO_ECG_Refcounted_Endpoint.
    // If we don't clone our endpoint and pass &endpoint, the sender will
    // attempt to delete endpoint during shutdown.
    TAO_ECG_Refcounted_Endpoint clone (new TAO_ECG_UDP_Out_Endpoint (endpoint));
    sender->init (ec.in (), addr_srv.in (), clone);

    // Setup the subscription and connect to the EC
    ACE_ConsumerQOS_Factory cons_qos_fact;
    cons_qos_fact.start_disjunction_group ();
    cons_qos_fact.insert (ACE_ES_EVENT_SOURCE_ANY, ACE_ES_EVENT_ANY, 0);
    RtecEventChannelAdmin::ConsumerQOS sub = cons_qos_fact.get_ConsumerQOS ();
    sender->connect (sub);

    // Create and initialize the receiver
    PortableServer::Servant_var<TAO_ECG_UDP_Receiver> const receiver =
      TAO_ECG_UDP_Receiver::create ();

    // TAO_ECG_UDP_Receiver::init() takes a TAO_ECG_Refcounted_Endpoint.
    // If we don't clone our endpoint and pass &endpoint, the receiver will
    // attempt to delete endpoint during shutdown.
    TAO_ECG_Refcounted_Endpoint clone2 (new TAO_ECG_UDP_Out_Endpoint (endpoint));
    receiver->init (ec.in (), clone2, addr_srv.in ());

    // Setup the registration and connect to the event channel
    ACE_SupplierQOS_Factory supp_qos_fact;
    supp_qos_fact.insert (MY_SOURCE_ID, MY_EVENT_TYPE, 0, 1);
    RtecEventChannelAdmin::SupplierQOS pub = supp_qos_fact.get_SupplierQOS ();
    receiver->connect (pub);

    // Create the appropriate event handler and register it with the reactor
    std::unique_ptr<ACE_Event_Handler> event_handler {};
    if (options.multicast)
      {
        std::unique_ptr<TAO_ECG_Mcast_EH> multicast_event_handler =
          std::make_unique<TAO_ECG_Mcast_EH> (receiver.in ());
        multicast_event_handler->reactor (orb->orb_core ()->reactor ());
        multicast_event_handler->open (ec.in ());
        event_handler = std::move (multicast_event_handler);
      }
    else
      {
        std::unique_ptr<TAO_ECG_UDP_EH> udp_event_handler =
          std::make_unique<TAO_ECG_UDP_EH> (receiver.in ());
        udp_event_handler->reactor (orb->orb_core ()->reactor ());
        ACE_INET_Addr local_addr (options.listen_port);
        if (udp_event_handler->open (local_addr) == -1)
          {
            std::cerr << "Cannot open EH" << std::endl;
          }
        event_handler = std::move (udp_event_handler);
      }

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

    if (options.ior_file != nullptr)
      {
        CORBA::String_var const str = orb->object_to_string (ec.in ());
        std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (options.ior_file));
        ior_file << str.in () << std::endl;
      }
    std::cout << "Starting main loop" << std::endl;

    while (true)
      {
        consumer->push (event);

        ACE_Time_Value tv (0, 1000 * EVENT_DELAY_MS);
        orb->run (tv);
      }
  }
  catch (CORBA::Exception const &exc)
    {
      std::cerr << "Caught CORBA::Exception" << std::endl << exc << std::endl;
    }
  return 1;
}
