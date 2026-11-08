#include "tao/PortableServer/POAC.h"
#include "tao/PortableServer/POAManagerFactoryC.h"
#include "tao/EndpointPolicy/EndpointPolicy.h"
#include "tao/EndpointPolicy/IIOPEndpointValue_i.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"

#include <memory>
#include <vector>

struct Server_Options
{
  ACE_TCHAR const *ior_file {ACE_TEXT("test.ior")};
  CORBA::Short endpoint_port {12345};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  for (int c = 1; c < argc; ++c)
    {
      if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-o")) == 0)
        {
          if (++c >= argc)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("usage: %s -o <iorfile> -p <port>\n"),
                               argv[0]),
                              -1);
          options.ior_file = argv[c];
        }
      else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-p")) == 0)
        {
          if (++c >= argc)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("usage: %s -o <iorfile> -p <port>\n"),
                               argv[0]),
                              -1);
          const int port = ACE_OS::atoi (argv[c]);
          if (port < 1 || port > 32756)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Endpoint port must be between 1 and 32756\n")),
                              -1);
          options.endpoint_port = static_cast<CORBA::Short> (port);
        }
      else if (ACE_OS::strstr (argv[c], ACE_TEXT ("-ORB")) == argv[c])
        {
          if (c + 1 < argc && argv[c + 1][0] != ACE_TEXT ('-'))
            ++c;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile> -p <port>\n"),
                           argv[0]),
                          -1);
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  if (parse_args (argc, argv, options) != 0)
    return 1;

  ACE_TCHAR first_endpoint[100] {};
  ACE_TCHAR second_endpoint[100] {};
  ACE_OS::snprintf (first_endpoint,
                    sizeof (first_endpoint) / sizeof (first_endpoint[0]),
                    ACE_TEXT ("iiop://localhost:%d/ssl_port=%d"),
                    options.endpoint_port,
                    options.endpoint_port + 1);
  ACE_OS::snprintf (second_endpoint,
                    sizeof (second_endpoint) / sizeof (second_endpoint[0]),
                    ACE_TEXT ("iiop://localhost:%d/ssl_port=%d"),
                    options.endpoint_port + 10,
                    options.endpoint_port + 11);

  ACE_TCHAR first_option[] = ACE_TEXT ("-ORBEndpoint");
  ACE_TCHAR second_option[] = ACE_TEXT ("-ORBEndpoint");
  std::vector<ACE_TCHAR *> orb_argv;
  orb_argv.reserve (argc + 5);
  for (int c = 0; c < argc; ++c)
    orb_argv.push_back (argv[c]);
  orb_argv.push_back (first_option);
  orb_argv.push_back (first_endpoint);
  orb_argv.push_back (second_option);
  orb_argv.push_back (second_endpoint);
  orb_argv.push_back (nullptr);
  int orb_argc = argc + 4;

  CORBA::ORB_var orb;
  CORBA::Object_var obj;
  PortableServer::POA_var root_poa;
  PortableServer::POAManagerFactory_var poa_manager_factory;
  try
    {
      orb = CORBA::ORB_init (orb_argc, orb_argv.data (), "EndpointPolicy");
      obj = orb->resolve_initial_references ("RootPOA");
      root_poa = PortableServer::POA::_narrow (obj.in ());
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil RootPOA\n")),
                          1);
      poa_manager_factory = root_poa->the_POAManagerFactory ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("initialization error ");
      return 1;
    }

  // Create a valid endpoint policy matching the first ORB endpoint argument.
  PortableServer::POAManager_var good_pm;
  CORBA::PolicyList policies;
  policies.length (1);
  EndpointPolicy::EndpointList list;
  list.length (1);
  list[0] = new IIOPEndpointValue_i ("localhost", options.endpoint_port);

  try
    {
      CORBA::Any policy_value;
      policy_value <<= list;
      policies[0] = orb->create_policy (EndpointPolicy::ENDPOINT_POLICY_TYPE,
                                        policy_value);
      good_pm = poa_manager_factory->create_POAManager ("goodPOAManager",
                                                        policies);
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Failed to create reachable POA manager");
      return 1;
    }

  try
    {
      CORBA::Object_var object = CORBA::Object::_nil ();
      policies.length (0);
      PortableServer::POA_var good_poa = root_poa->create_POA ("goodPOA",
                                                               good_pm.in (),
                                                               policies);
      ACE_DEBUG ((LM_DEBUG, "Creating IOR from good poa\n"));
      object = good_poa->create_reference ("IDL:JustATest:1.0");
      CORBA::String_var good_ior = orb->object_to_string (object.in ());
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (options.ior_file, ACE_TEXT ("w")), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           options.ior_file),
                          1);
      ACE_OS::fprintf (output_file.get (), "%s", good_ior.in ());
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("cannot run server");
      return 1;
    }

  root_poa->destroy (true, true);
  orb->destroy ();
  return 0;
}
