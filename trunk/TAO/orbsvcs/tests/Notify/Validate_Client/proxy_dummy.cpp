// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB.h"
#include "tao/PortableServer/POAC.h"
#include "tao/PortableServer/POAManagerC.h"

const ACE_TCHAR *output_filename = ACE_TEXT ("persistence.notify.xml");

// must be char
const char *format =
  "<?xml version=\"1.0\"?>\n"
  "<notification_service version=\"1.0\" timestamp=\"197568243541\">\n"
  "  <channel_factory>\n"
  "    <channel TopologyID=\"2\" MaxQueueLength=\"0\" MaxConsumers=\"0\" MaxSuppliers=\"0\" RejectNewEvents=\"false\">\n"
  "      <filter_factory>\n"
  "        <filter FilterId=\"1\" Grammar=\"TCL\">\n"
  "          <constraint ConstraintId=\"3\" Expression=\"threshold &gt; 10\">\n"
  "            <EventType Domain=\"*\" Type=\"*\">\n"
  "            </EventType>\n"
  "          </constraint>\n"
  "          <constraint ConstraintId=\"4\" Expression=\"threshold &gt; 10\">\n"
  "            <EventType Domain=\"*\" Type=\"*\">\n"
  "            </EventType>\n"
  "          </constraint>\n"
  "        </filter>\n"
  "        <filter FilterId=\"2\" Grammar=\"TCL\">\n"
  "          <constraint ConstraintId=\"3\" Expression=\"threshold &lt; 15\">\n"
  "            <EventType Domain=\"*\" Type=\"*\">\n"
  "            </EventType>\n"
  "          </constraint>\n"
  "          <constraint ConstraintId=\"4\" Expression=\"threshold &lt; 15\">\n"
  "            <EventType Domain=\"*\" Type=\"*\">\n"
  "            </EventType>\n"
  "          </constraint>\n"
  "        </filter>\n"
  "        <filter FilterId=\"3\" Grammar=\"TCL\">\n"
  "          <constraint ConstraintId=\"3\" Expression=\"threshold &lt; 15\">\n"
  "            <EventType Domain=\"*\" Type=\"*\">\n"
  "            </EventType>\n"
  "          </constraint>\n"
  "          <constraint ConstraintId=\"4\" Expression=\"threshold &lt; 15\">\n"
  "            <EventType Domain=\"*\" Type=\"*\">\n"
  "            </EventType>\n"
  "          </constraint>\n"
  "        </filter>\n"
  "      </filter_factory>\n"
  "      <consumer_admin TopologyID=\"6\" InterFilterGroupOperator=\"0\">\n"
  "        <filter_admin>\n"
  "          <filter TopologyID=\"1\" MapId=\"2\">\n"
  "          </filter>\n"
  "        </filter_admin>\n"
  "        <subscriptions>\n"
  "          <subscription Domain=\"*\" Type=\"%%ALL\">\n"
  "          </subscription>\n"
  "        </subscriptions>\n"
  "        <structured_proxy_push_supplier TopologyID=\"11\" PeerIOR=\"%s\">\n"
  "          <subscriptions>\n"
  "            <subscription Domain=\"*\" Type=\"%%ALL\">\n"
  "            </subscription>\n"
  "          </subscriptions>\n"
  "        </structured_proxy_push_supplier>\n"
  "      </consumer_admin>\n"
  "      <consumer_admin TopologyID=\"7\" InterFilterGroupOperator=\"0\">\n"
  "        <filter_admin>\n"
  "          <filter TopologyID=\"1\" MapId=\"3\">\n"
  "          </filter>\n"
  "        </filter_admin>\n"
  "        <subscriptions>\n"
  "          <subscription Domain=\"*\" Type=\"%%ALL\">\n"
  "          </subscription>\n"
  "        </subscriptions>\n"
  "        <structured_proxy_push_supplier TopologyID=\"12\" PeerIOR=\"%s\">\n"
  "          <subscriptions>\n"
  "            <subscription Domain=\"*\" Type=\"%%ALL\">\n"
  "            </subscription>\n"
  "          </subscriptions>\n"
  "        </structured_proxy_push_supplier>\n"
  "      </consumer_admin>\n"
  "      <supplier_admin TopologyID=\"3\" InterFilterGroupOperator=\"0\">\n"
  "        <filter_admin>\n"
  "          <filter TopologyID=\"1\" MapId=\"1\">\n"
  "          </filter>\n"
  "        </filter_admin>\n"
  "        <subscriptions>\n"
  "          <subscription Domain=\"*\" Type=\"%%ALL\">\n"
  "          </subscription>\n"
  "        </subscriptions>\n"
  "        <structured_proxy_push_consumer TopologyID=\"9\" PeerIOR=\"%s\">\n"
  "          <subscriptions>\n"
  "            <subscription Domain=\"*\" Type=\"%%ALL\">\n"
  "            </subscription>\n"
  "          </subscriptions>\n"
  "        </structured_proxy_push_consumer>\n"
  "        <structured_proxy_push_consumer TopologyID=\"10\" PeerIOR=\"%s\">\n"
  "          <subscriptions>\n"
  "            <subscription Domain=\"*\" Type=\"%%ALL\">\n"
  "            </subscription>\n"
  "          </subscriptions>\n"
  "        </structured_proxy_push_consumer>\n"
  "      </supplier_admin>\n"
  "    </channel>\n"
  "    <reconnect_registry>\n"
  "    </reconnect_registry>\n"
  "  </channel_factory>\n"
  "</notification_service>\n"
  ;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        output_filename = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-o <output file> ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::String_var repo_id =
        "IDL:omg.org/CosNotifyComm/StructuredPushConsumer:1.0";

      CORBA::Object_var object =
        root_poa->create_reference (repo_id.in());

      CORBA::String_var ior = orb->object_to_string (object.in ());

      FILE *output_file= ACE_OS::fopen (output_filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %C\n"),
                           output_file),
                           1);
      const char * dummy_consumer_proxy = ior.in();

      ACE_OS::fprintf (output_file, format,
                       dummy_consumer_proxy, dummy_consumer_proxy,
                       dummy_consumer_proxy, dummy_consumer_proxy);
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
