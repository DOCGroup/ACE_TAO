// $Id$
// Adapted from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#include "Consumer.h"
#include "Supplier.h"
#include "AddrServer.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"

ACE_RCSID (EC_MT_Mcast,
           MCast,
           "$Id$")

const char *udp_mcast_address =
    ACE_DEFAULT_MULTICAST_ADDR ":10001";

static CORBA::ORB_var orb = CORBA::ORB::_nil ();
static bool terminate_threads = false;
static const unsigned pool_size = 2;
static const int data_items = 60000;

void *
run_orb_within_thread (void *)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  while (! terminate_threads)
    {
      ACE_TRY
        {
          CORBA::Boolean there_is_work =
            orb->work_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (there_is_work)
            {
              // We use a TAO extension. The CORBA mechanism does not
              // provide any decent way to control the duration of
              // perform_work() or work_pending(), so just calling
              // them results in a spin loop.
              ACE_Time_Value tv (0, 50000);
              orb->perform_work (tv ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "perform work");

          return 0;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);
    }

  return 0;
}

int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
  // Register the default factory in the Service Configurator.
  // If your platform supports static constructors then you can
  // simply using the ACE_STATIC_SVC_DEFINE() macro, unfortunately TAO
  // must run on platforms where static constructors do not work well,
  // so we have to explicitly invoke this function.
  TAO_EC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // **************** HERE IS THE ORB SETUP

      // Create the ORB, pass the argv list for parsing.
      orb = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **************** THAT COMPLETES THE ORB SETUP

      // **************** HERE IS THE LOCAL EVENT CHANNEL SETUP

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
      ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The event channel is activated as any other CORBA servant.
      // In this case we use the simple implicit activation with the
      // RootPOA
      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **************** THAT COMPLETES THE LOCAL EVENT CHANNEL SETUP

      // **************** HERE IS THE FEDERATION SETUP

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
        as_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // We need a local socket to send the data, open it and check
      // that everything is OK:
      TAO_ECG_UDP_Out_Endpoint endpoint;
      if (endpoint.dgram ().open (ACE_Addr::sap_any) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Cannot open send endpoint\n"),
                            1);
        }

      // Now we setup the sender:
      TAO_ECG_UDP_Sender sender;
      sender.init (event_channel.in (),
                   address_server.in (),
                   &endpoint
                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

      sender.open (sub ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // To receive events we need to setup an event handler:
      TAO_ECG_UDP_Receiver receiver;
      TAO_ECG_Mcast_EH mcast_eh (&receiver);

      // The event handler uses the ORB reactor to wait for multicast
      // traffic:
      mcast_eh.reactor (orb->orb_core ()->reactor ());

      // The multicast Event Handler needs to know to what multicast
      // groups it should listen to.  To do so it becomes an observer
      // with the event channel, to determine the list of events
      // required by all the local consumer.
      // Then it register for the multicast groups that carry those
      // events:
      int r = mcast_eh.open (event_channel.in ()
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (r == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Cannot open EH %p\n"), 1);
        }

      // Big events have to be fragmented when sent using multicast.
      // The receiver periodically checks the status of the fragments,
      // using the following timer.
      // If after <max_expiration_count> iterations the complete
      // message has not been received it is dropped:
      ACE_Time_Value expire (0, 50000);
      const int max_expiration_count = 5;

      // Again the receiver connects to the event channel as a
      // supplier of events, using the Observer features to detect
      // local consumers and their interests:
      receiver.init (event_channel.in (),
                     &endpoint,
                     address_server.in (),
                     orb->orb_core ()->reactor (),
                     expire,
                     max_expiration_count
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

      receiver.open (pub ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **************** THAT COMPLETES THE FEDERATION SETUP

      // **************** HERE IS THE CLIENT SETUP

      // First let us create consumers and connect them to the event
      // channel
      Consumer consumer1;
      Consumer consumer2;
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer1.connect (consumer_admin.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer2.connect (consumer_admin.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // And now create a supplier
      Supplier supplier;
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier.connect (supplier_admin.in ()
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **************** THAT COMPLETES THE CLIENT SETUP

      // **************** HERE IS THE EVENT LOOP

      // creating thread pool
      ACE_Thread_Manager the_ace_manager;
      the_ace_manager.open ();
      int thread_pool_id = the_ace_manager.spawn_n (
        pool_size, ACE_THR_FUNC (run_orb_within_thread), 0, THR_DETACHED | THR_NEW_LWP);
      if (thread_pool_id == -1) {
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot spawn thread pool\n"), 1);
      }
      ACE_OS::sleep (1); // simple solution ensures ready thread pool

      for (int i = 0; i < data_items; i++)
        {
          supplier.perform_push (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_OS::sleep (2); // simple solution ensures ready receivers
      terminate_threads = true; // terminate thread pool
      ACE_OS::sleep (1); // simple solution ensures terminated thread pool
      the_ace_manager.cancel_grp (thread_pool_id); // cancel any threads still alive
      the_ace_manager.close ();

      // **************** THAT COMPLETES THE EVENT LOOP

      // **************** HERE IS THE CLEANUP CODE

      // First the easy ones
      supplier.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer1.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer2.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now let us close the Receiver
      receiver.close (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      r = mcast_eh.close (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (r == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Closing MCast event handler\n"), 1);
        }

      // And also close the sender of events
      sender.close (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The event channel must be destroyed, so it can release its
      // resources, and inform all the clients that are still
      // connected that it is going away.
      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Deactivating the event channel implementation is not strictly
      // required, the POA will do it for us, but it is good manners:
      {
        // Using _this() activates with the default POA, we must gain
        // access to that POA to deactivate the object.
        // Notice that we 'know' that the default POA for this servant
        // is the root POA, but the code is more robust if we don't
        // rely on that.
        PortableServer::POA_var poa =
          ec_impl._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
        // Get the Object Id used for the servant..
        PortableServer::ObjectId_var oid =
          poa->servant_to_id (&ec_impl ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        // Deactivate the object
        poa->deactivate_object (oid.in () ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // Now we can destroy the POA, the flags mean that we want to
      // wait until the POA is really destroyed
      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Finally destroy the ORB
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **************** THAT COMPLETES THE CLEANUP CODE

      ACE_DEBUG ((LM_DEBUG,
                  "MCast example terminated\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "m:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
