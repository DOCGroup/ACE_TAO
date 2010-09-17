// $Id$

#include "Consumer.h"
#include "Supplier.h"
#include "AddrServer.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *udp_mcast_address =
    ACE_TEXT (ACE_DEFAULT_MULTICAST_ADDR) ACE_TEXT(":10001");

int parse_args (int argc, ACE_TCHAR *argv[]);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Register the default factory in the Service Configurator.
  // If your platform supports static constructors then you can
  // simply using the ACE_STATIC_SVC_DEFINE() macro, unfortunately TAO
  // must run on platforms where static constructors do not work well,
  // so we have to explicitly invoke this function.
  TAO_EC_Default_Factory::init_svcs ();

  // The exception macros are described in $ACE_ROOT/docs/exceptions.html
  // and defined in $ACE_ROOT/ace/CORBA_macros.h.
  // If your platform supports native exceptions, and TAO was compiled
  // with native exception support then you can simply use try/catch
  // and avoid the argument.
  // Unfortunately many embedded systems cannot use exceptions due to
  // the space and time overhead.
  //
  try
    {
      // **************** HERE STARTS THE ORB SETUP

      // Create the ORB, pass the argv list for parsing.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Parse the arguments, you usually want to do this after
      // invoking ORB_init() because ORB_init() will remove all the
      // -ORB options from the command line.
      if (parse_args (argc, argv) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Service [-m udp_mcast_addr]\n"));
          return 1;
        }

      // This is the standard code to get access to the POA and
      // activate it.
      // The POA starts in the holding state, if it is not activated
      // it will not process any requests.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // **************** THAT COMPLETS THE ORB SETUP

      // **************** HERE START THE LOCAL EVENT CHANNEL SETUP

      // This structure is used to define the startup time event
      // channel configuration.
      // This structure is described in
      //
      // $TAO_ROOT/docs/ec_options.html
      //
      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());

      // Create the Event Channel implementation class
      TAO_EC_Event_Channel ec_impl (attributes);

      // Activate the Event Channel, depending on the configuration
      // that may involve creating some threads.
      // But it should always be invoked because several internal data
      // structures are initialized at that point.
      ec_impl.activate ();

      // The event channel is activated as any other CORBA servant.
      // In this case we use the simple implicit activation with the
      // RootPOA
      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this ();

      // **************** THAT COMPLETES THE LOCAL EVENT CHANNEL SETUP

      // **************** HERE STARTS THE FEDERATION SETUP

      // The next step is to setup the multicast gateways.
      // There are two gateways involved, one sends the locally
      // generated events to the federated peers, the second gateway
      // receives multicast traffic and turns it into local events.

      // The sender requires a helper object to select what
      // multicast group will carry what traffic, this is the
      // so-called 'Address Server'.
      // The intention is that advanced applications can use different
      // multicast groups for different events, this can exploit
      // network interfaces that filter unwanted multicast traffic.
      // The helper object is accessed through an IDL interface, so it
      // can reside remotely.
      // In this example, and in many application, using a fixed
      // multicast group is enough, and a local address server is the
      // right approach.

      // First we convert the string into an INET address, then we
      // convert that into the right IDL structure:
      ACE_INET_Addr udp_addr (udp_mcast_address);
      ACE_DEBUG ((LM_DEBUG,
                  "Multicast address is: %s\n",
                  udp_mcast_address));
      RtecUDPAdmin::UDP_Addr addr;
      addr.ipaddr = udp_addr.get_ip_address ();
      addr.port   = udp_addr.get_port_number ();

      // Now we create and activate the servant
      AddrServer as_impl (addr);
      RtecUDPAdmin::AddrServer_var address_server =
        as_impl._this ();

      // We need a local socket to send the data, open it and check
      // that everything is OK:
      TAO_ECG_Refcounted_Endpoint endpoint(new TAO_ECG_UDP_Out_Endpoint);
      if (endpoint->dgram ().open (ACE_Addr::sap_any) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Cannot open send endpoint\n"),
                            1);
        }

      // Now we setup the sender:
      TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> sender = TAO_ECG_UDP_Sender::create();
      sender->init (event_channel.in (),
                    address_server.in (),
                    endpoint);

      // Now we connect the sender as a consumer of events, it will
      // receive any event from any source and send it to the "right"
      // multicast group, as defined by the address server set above:
      RtecEventChannelAdmin::ConsumerQOS sub;
      sub.is_gateway = 1;

      sub.dependencies.length (1);
      sub.dependencies[0].event.header.type =
        ACE_ES_EVENT_ANY;        // first free event type
      sub.dependencies[0].event.header.source =
        ACE_ES_EVENT_SOURCE_ANY; // Any source is OK

      sender->connect (sub);

      // To receive events we need to setup an event handler:
      TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> receiver = TAO_ECG_UDP_Receiver::create();
      TAO_ECG_Mcast_EH mcast_eh (&(*receiver));

      // The event handler uses the ORB reactor to wait for multicast
      // traffic:
      mcast_eh.reactor (orb->orb_core ()->reactor ());

      // The multicast Event Handler needs to know to what multicast
      // groups it should listen to.  To do so it becomes an observer
      // with the event channel, to determine the list of events
      // required by all the local consumer.
      // Then it register for the multicast groups that carry those
      // events:
      mcast_eh.open (event_channel.in ());

      // Again the receiver connects to the event channel as a
      // supplier of events, using the Observer features to detect
      // local consumers and their interests:
      receiver->init (event_channel.in (),
                      endpoint,
                      address_server.in ());

      // The Receiver is also a supplier of events.  The exact type of
      // events is only known to the application, because it depends
      // on the traffic carried by all the multicast groups that the
      // different event handlers subscribe to.
      // In this example we choose to simply describe our publications
      // using wilcards, any event from any source.  More advanced
      // application could use the Observer features in the event
      // channel to update this information (and reduce the number of
      // multicast groups that each receive subscribes to).
      // In a future version the event channel could perform some of
      // those tasks automatically
      RtecEventChannelAdmin::SupplierQOS pub;
      pub.publications.length (1);
      pub.publications[0].event.header.type   = ACE_ES_EVENT_ANY;
      pub.publications[0].event.header.source = ACE_ES_EVENT_SOURCE_ANY;
      pub.is_gateway = 1;

      receiver->connect (pub);

      // **************** THAT COMPLETES THE FEDERATION SETUP

      // **************** HERE STARTS THE CLIENT SETUP

      // First let us create a consumer and connect it to the event
      // channel
      Consumer consumer;
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();
      consumer.connect (consumer_admin.in ());

      // And now create a supplier
      Supplier supplier;
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();
      supplier.connect (supplier_admin.in ());

      // **************** THAT COMPLETES THE CLIENT SETUP

      // **************** HERE STARTS THE EVENT LOOP

      // Wait for events, including incoming multicast data.
      // We could also use orb->run(), but that will not let us
      // terminate the application in a nice way.
      for (int i = 0; i != 1000; ++i)
        {
          CORBA::Boolean there_is_work =
            orb->work_pending ();
          if (there_is_work)
            {
              // We use a TAO extension. The CORBA mechanism does not
              // provide any decent way to control the duration of
              // perform_work() or work_pending(), so just calling
              // them results in a spin loop.
              ACE_Time_Value tv (0, 50000);
              orb->perform_work (tv);
            }
          ACE_Time_Value tv (0, 100000);
          ACE_OS::sleep (tv);
          supplier.perform_push ();
        }

      // **************** THAT COMPLETES THE EVENT LOOP

      // **************** HERE STARTS THE CLEANUP CODE

      // First the easy ones
      supplier.disconnect ();
      consumer.disconnect ();

      // Now let us close the Receiver
      receiver->shutdown ();

      int r = mcast_eh.shutdown ();

      if (r == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Closing MCast event handler\n"), 1);
        }

      // And also close the sender of events
      sender->shutdown ();

      // The event channel must be destroyed, so it can release its
      // resources, and inform all the clients that are still
      // connected that it is going away.
      event_channel->destroy ();

      // Deactivating the event channel implementation is not strictly
      // required, the POA will do it for us, but it is good manners:
      {
        // Using _this() activates with the default POA, we must gain
        // access to that POA to deactivate the object.
        // Notice that we 'know' that the default POA for this servant
        // is the root POA, but the code is more robust if we don't
        // rely on that.
        PortableServer::POA_var poa =
          ec_impl._default_POA ();
        // Get the Object Id used for the servant..
        PortableServer::ObjectId_var oid =
          poa->servant_to_id (&ec_impl);
        // Deactivate the object
        poa->deactivate_object (oid.in ());
      }

      // Now we can destroy the POA, the flags mean that we want to
      // wait until the POA is really destroyed
      poa->destroy (1, 1);

      // Finally destroy the ORB
      orb->destroy ();

      // **************** THAT COMPLETES THE CLEANUP CODE

      ACE_DEBUG ((LM_DEBUG,
                  "MCast example terminated\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}

// ****************************************************************

int parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("m:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'm':
        udp_mcast_address = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-m udp_mcast_address]"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

