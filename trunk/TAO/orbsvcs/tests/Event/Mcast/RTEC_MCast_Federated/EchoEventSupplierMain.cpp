// $Id$

// EchoEventSupplierMain.cpp
// Main program for a PushSupplier of Echo events.

#include "EchoEventSupplier_i.h"
#include "SimpleAddressServer.h"
#include "BarrierC.h"

#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Time_Utilities.h"
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

#include "ace/Auto_Ptr.h"
#include <fstream>

const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

const int EVENT_DELAY_MS = 10;

// Initialize the ORB.

int main (int argc, char* argv[])
{
  try
  {
    // Initialize the EC Factory so we can customize the EC
    TAO_EC_Default_Factory::init_svcs ();

    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    const char* ecname = "EventService";
    const char* address = "localhost";
    const char* iorfile = 0;
    u_short port = 12345;
    u_short listenport = 12345;

    int mcast = 1;

    for (int i = 0; argv[i] != 0; i++)
      {
        if (strcasecmp(argv[i], "-ecname") == 0)
          {
            if (argv[i+1] != 0)
                ecname = argv[++i];
            else
              ACE_ERROR_RETURN ((LM_ERROR,  "Missing Event channel name\n"),0);
          }
        else if (strcasecmp(argv[i], "-address") == 0)
          {
            if (argv[i+1] != 0)
              address = argv[++i];
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing address\n"),0);
          }
        else if (strcasecmp(argv[i], "-port") == 0)
          {
            if (argv[i+1] != 0)
              port = ACE_OS::atoi(argv[++i]);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing port\n"),0);
          }
        else if (strcasecmp(argv[i], "-listenport") == 0)
          {
            if (argv[i+1] != 0)
              listenport = ACE_OS::atoi(argv[++i]);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing port\n"), 0);
          }
        else if (strcmp(argv[i], "-iorfile") == 0)
          {
            if (argv[i+1] != 0)
              iorfile = argv[++i];
             else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing ior file\n"), 0);
          }
        else if (strcmp(argv[i], "-udp") == 0)
          mcast = 0;
      }

    // Get the POA
    CORBA::Object_var tmpobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (tmpobj.in ());
    PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
    poa_manager->activate ();

    // Create a local event channel and register it
    TAO_EC_Event_Channel_Attributes attributes (poa.in (), poa.in ());
    TAO_EC_Event_Channel ec_impl (attributes);
    ec_impl.activate ();
    PortableServer::ObjectId_var oid = poa->activate_object(&ec_impl);
    tmpobj = poa->id_to_reference(oid.in());
    RtecEventChannelAdmin::EventChannel_var ec =
      RtecEventChannelAdmin::EventChannel::_narrow(tmpobj.in());

    // Find the Naming Service.
    tmpobj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context =
      CosNaming::NamingContextExt::_narrow(tmpobj.in());

    // Bind the Event Channel using Naming Services
    CosNaming::Name_var name = root_context->to_name(ecname);
    root_context->rebind(name.in(), ec.in());

    // Get a proxy push consumer from the EventChannel.
    RtecEventChannelAdmin::SupplierAdmin_var admin = ec->for_suppliers();
    RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
      admin->obtain_push_consumer();

    // Instantiate an EchoEventSupplier_i servant.
    EchoEventSupplier_i servant(orb.in());

    // Register it with the RootPOA.
    oid = poa->activate_object(&servant);
    tmpobj = poa->id_to_reference(oid.in());
    RtecEventComm::PushSupplier_var supplier =
      RtecEventComm::PushSupplier::_narrow(tmpobj.in());

    // Connect to the EC.
    ACE_SupplierQOS_Factory qos;
    qos.insert (MY_SOURCE_ID, MY_EVENT_TYPE, 0, 1);
    consumer->connect_push_supplier (supplier.in (), qos.get_SupplierQOS ());

    // Initialize the address server with the desired address. This will
    // be used by the sender object and the multicast receiver only if
    // one is not otherwise available via the naming service.
    ACE_INET_Addr send_addr (port, address);
    SimpleAddressServer addr_srv_impl (send_addr);

    try
      {
        tmpobj = root_context->resolve_str ("Echo_address");
      }
    catch (const ::CosNaming::NamingContext::NotFound &)
      {
        // Create an instance of the addr server for local use

        PortableServer::ObjectId_var addr_srv_oid =
          poa->activate_object(&addr_srv_impl);
        tmpobj =
          poa->id_to_reference(addr_srv_oid.in());
      }

    RtecUDPAdmin::AddrServer_var addr_srv =
      RtecUDPAdmin::AddrServer::_narrow(tmpobj.in());

    // Create and initialize the sender object
    TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> sender =
                                TAO_ECG_UDP_Sender::create();
    TAO_ECG_UDP_Out_Endpoint endpoint;
    if (endpoint.dgram ().open (ACE_Addr::sap_any) == -1) {
      ACE_ERROR_RETURN ((LM_ERROR,
       "Cannot open send endpoint\n"),
      1);
    }

   // TAO_ECG_UDP_Sender::init() takes a TAO_ECG_Refcounted_Endpoint.
    // If we don't clone our endpoint and pass &endpoint, the sender will
    // attempt to delete endpoint during shutdown.
    TAO_ECG_UDP_Out_Endpoint* clone;
    ACE_NEW_RETURN (clone,
                    TAO_ECG_UDP_Out_Endpoint (endpoint),
                    -1);
    sender->init (ec.in (), addr_srv.in (), clone);

    // Setup the subscription and connect to the EC
    ACE_ConsumerQOS_Factory cons_qos_fact;
    cons_qos_fact.start_disjunction_group ();
    cons_qos_fact.insert (ACE_ES_EVENT_SOURCE_ANY, ACE_ES_EVENT_ANY, 0);
    RtecEventChannelAdmin::ConsumerQOS sub = cons_qos_fact.get_ConsumerQOS ();
    sender->connect (sub);

    // Create and initialize the receiver
    TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> receiver =
                                      TAO_ECG_UDP_Receiver::create();

    // TAO_ECG_UDP_Receiver::init() takes a TAO_ECG_Refcounted_Endpoint.
    // If we don't clone our endpoint and pass &endpoint, the receiver will
    // attempt to delete endpoint during shutdown.
    ACE_NEW_RETURN (clone,
                    TAO_ECG_UDP_Out_Endpoint (endpoint),
                    -1);
    receiver->init (ec.in (), clone, addr_srv.in ());

    // Setup the registration and connect to the event channel
    ACE_SupplierQOS_Factory supp_qos_fact;
    supp_qos_fact.insert (MY_SOURCE_ID, MY_EVENT_TYPE, 0, 1);
    RtecEventChannelAdmin::SupplierQOS pub = supp_qos_fact.get_SupplierQOS ();
    receiver->connect (pub);

    // Create the appropriate event handler and register it with the reactor
    auto_ptr<ACE_Event_Handler> eh;
    if (mcast) {
      auto_ptr<TAO_ECG_Mcast_EH> mcast_eh(new TAO_ECG_Mcast_EH (receiver.in()));
      mcast_eh->reactor (orb->orb_core ()->reactor ());
      mcast_eh->open (ec.in());
      ACE_AUTO_PTR_RESET(eh,mcast_eh.release(),ACE_Event_Handler);
      //eh.reset(mcast_eh.release());
    } else {
      auto_ptr<TAO_ECG_UDP_EH> udp_eh (new TAO_ECG_UDP_EH (receiver.in()));
      udp_eh->reactor (orb->orb_core ()->reactor ());
      ACE_INET_Addr local_addr (listenport);
      if (udp_eh->open (local_addr) == -1)
        ACE_ERROR ((LM_ERROR,"Cannot open EH\n"));

      ACE_AUTO_PTR_RESET(eh,udp_eh.release(),ACE_Event_Handler);
      //eh.reset(udp_eh.release());
    }

    // Check to see if there is a Barrier server available and if so,
    // wait for it.
     try
      {
        tmpobj = root_context->resolve_str ("Echo_barrier");
        Barrier_var barrier = Barrier::_narrow (tmpobj.in());
        if (!CORBA::is_nil(barrier.in()))
            barrier->wait();
      }
    catch (const ::CORBA::Exception &)
      {
        // no worries, just keep going.
      }

    // Create an event (just a string in this case).
    const CORBA::String_var eventData = CORBA::string_dup(ecname);

    // Create an event set for one event
    RtecEventComm::EventSet event (1);
    event.length (1);

    // Initialize event header.
    event[0].header.source = MY_SOURCE_ID;
    event[0].header.ttl = 1;
    event[0].header.type = MY_EVENT_TYPE;

    // Initialize data fields in event.
    event[0].data.any_value <<= eventData;

    if (iorfile != 0) {
      CORBA::String_var str = orb->object_to_string( ec.in() );
      std::ofstream iorFile( iorfile );
      iorFile << str.in() << std::endl;
      iorFile.close();
    }
    ACE_DEBUG ((LM_DEBUG,
    "Starting main loop\n"));

    const int EVENT_DELAY_MS = 10;

    while (1) {
      consumer->push (event);

      ACE_Time_Value tv(0, 1000 * EVENT_DELAY_MS);
      orb->run(tv);
    }

    orb->destroy();
    return 0;
  }
  catch (CORBA::Exception& exc)
  {
    ACE_ERROR ((LM_ERROR,
    "Caught CORBA::Exception\n%s (%s)\n",
    exc._name (), exc._rep_id ()));
  }
  return 1;
}
