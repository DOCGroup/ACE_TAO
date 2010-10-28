// $Id$

// EchoEventSupplierMain.cpp
// Main program for a PushSupplier of Echo events.

#include "EchoEventSupplier_i.h"
#include "SimpleAddressServer.h"

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
#include "ace/OS_NS_strings.h"

#include "ace/Auto_Ptr.h"
#include <fstream>

const RtecEventComm::EventSourceID MY_SOURCE_ID  = ACE_ES_EVENT_SOURCE_ANY + 1;
const RtecEventComm::EventType     MY_EVENT_TYPE = ACE_ES_EVENT_UNDEFINED + 1;

// Initialize the ORB.

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the EC Factory so we can customize the EC
    TAO_EC_Default_Factory::init_svcs ();

    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    const ACE_TCHAR *ecname = ACE_TEXT ("EventService");
    const ACE_TCHAR *address = ACE_TEXT ("localhost");
    const ACE_TCHAR *iorfile = 0;
    u_short port = 12345;
    u_short listenport = 12345;

    int mcast = 1;

    for (int i = 0; argv[i] != 0; i++)
      {
        if (ACE_OS::strcasecmp(argv[i], ACE_TEXT ("-ecname")) == 0)
          {
            if (argv[i+1] != 0)
                ecname = argv[++i];
            else
              ACE_ERROR_RETURN ((LM_ERROR,  "Missing Event channel name\n"),0);
          }
        else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT ("-address")) == 0)
          {
            if (argv[i+1] != 0)
              address = argv[++i];
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing address\n"),0);
          }
        else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT ("-port")) == 0)
          {
            if (argv[i+1] != 0)
              port = ACE_OS::atoi(argv[++i]);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing port\n"),0);
          }
        else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT ("-listenport")) == 0)
          {
            if (argv[i+1] != 0)
              listenport = ACE_OS::atoi(argv[++i]);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing port\n"), 0);
          }
        else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT ("-iorfile")) == 0)
          {
            if (argv[i+1] != 0)
              iorfile = argv[++i];
             else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing ior file\n"), 0);
          }
        else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT ("-udp")) == 0)
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
    CosNaming::Name_var name =
      root_context->to_name (ACE_TEXT_ALWAYS_CHAR (ecname));
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

    // Create an instance of the addr server for local use

    PortableServer::ObjectId_var addr_srv_oid =
      poa->activate_object(&addr_srv_impl);
    tmpobj =
      poa->id_to_reference(addr_srv_oid.in());

    RtecUDPAdmin::AddrServer_var addr_srv =
      RtecUDPAdmin::AddrServer::_narrow(tmpobj.in());

    // Create and initialize the sender object
    PortableServer::Servant_var<TAO_ECG_UDP_Sender> sender =
                                TAO_ECG_UDP_Sender::create();
    TAO_ECG_UDP_Out_Endpoint endpoint;
    // need to be explicit about the address type when built with
    // IPv6 support, otherwise SOCK_DGram::open defaults to ipv6 when
    // given a sap_any address. This causes trouble on at least solaris
    // and windows, or at most on not-linux.
    if (endpoint.dgram ().open (ACE_Addr::sap_any,
                                send_addr.get_type()) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open send endpoint\n"),
                          1);
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
    PortableServer::Servant_var<TAO_ECG_UDP_Receiver> receiver =
                                      TAO_ECG_UDP_Receiver::create();

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

    // Create an event (just a string in this case).

    // Create an event set for one event
    RtecEventComm::EventSet event (1);
    event.length (1);

    // Initialize event header.
    event[0].header.source = MY_SOURCE_ID;
    event[0].header.ttl = 1;
    event[0].header.type = MY_EVENT_TYPE;

#if !defined (TAO_LACKS_EVENT_CHANNEL_ANY)
    // Initialize data fields in event.
    const CORBA::String_var eventData =
      CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (ecname));

    event[0].data.any_value <<= eventData;
#else
    // Use the octet sequence payload instead
    char *tmpstr = const_cast<char *>(ACE_TEXT_ALWAYS_CHAR (ecname));
    size_t len = ACE_OS::strlen(tmpstr) +1;
    event[0].data.payload.replace (
      len,
      len,
      reinterpret_cast<CORBA::Octet *> (tmpstr));
#endif  /* !TAO_LACKS_EVENT_CHANNEL_ANY */

    if (iorfile != 0) {
      CORBA::String_var str = orb->object_to_string( ec.in() );
      std::ofstream iorFile( ACE_TEXT_ALWAYS_CHAR(iorfile) );
      iorFile << str.in() << std::endl;
      iorFile.close();
    }
    ACE_DEBUG ((LM_DEBUG,
    "Starting main loop\n"));

    const int EVENT_DELAY_MS = 1000;

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
    "Caught CORBA::Exception\n%C (%C)\n",
    exc._name (),
    exc._rep_id () ));
  }
  return 1;
}
