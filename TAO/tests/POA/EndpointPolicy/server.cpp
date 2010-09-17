// $Id$

#include "Hello.h"
#include "tao/PolicyC.h"
#include "tao/EndpointPolicy/EndpointPolicy.h"
#include "tao/EndpointPolicy/IIOPEndpointValue_i.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *good_ior_file = ACE_TEXT ("good.ior");
const ACE_TCHAR *bad_ior_file = ACE_TEXT ("bad.ior");
const ACE_TCHAR *root_ior_file = ACE_TEXT("root.ior");
const ACE_TCHAR *svc_conf_file = ACE_TEXT("multi_prot.conf");

int load_advanced_resources =
ACE_Service_Config::process_directive (ace_svc_desc_TAO_Advanced_Resource_Factory);

CORBA::Short endpoint_port = 12345;
int verbose = 0;

enum HostNameForm
  {
    from_hostname,
    use_localhost,
    use_defaulted,
    multi_protocol
  };

const ACE_TCHAR *form_arg;

HostNameForm host_form = from_hostname;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  for (int c = 1; c < argc; c++) {
    if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-g")) == 0)
      {
        good_ior_file = argv[++c];
      }
    else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-b")) == 0)
      {
        bad_ior_file = argv[++c];
      }
    else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-c")) == 0)
      {
        svc_conf_file = argv[++c];
      }
    else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-p")) == 0)
      {
        endpoint_port = ACE_OS::atoi (argv[++c]);
      }
    else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-v")) == 0)
      {
        verbose = 1;
      }
    else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-h")) == 0)
      {
        ++c;
        if (c == argc)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "host form option requires an argument\n"),-1);
        form_arg = argv[c];
        if (ACE_OS::strcasecmp (form_arg, ACE_TEXT ("local")) == 0)
          host_form = use_localhost;
        else if (ACE_OS::strcasecmp (form_arg, ACE_TEXT ("default")) == 0)
          host_form = use_defaulted;
        else if (ACE_OS::strcasecmp (form_arg, ACE_TEXT ("multi")) == 0)
          host_form = multi_protocol;
        else
          ACE_ERROR_RETURN ((LM_ERROR,
                             "invalid host form arg, '%s'\n", form_arg), -1);
      }
    else if (ACE_OS::strstr (argv[c], ACE_TEXT ("-ORB")) == argv[c])
      {
        c++;
        continue;
      }
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         "usage:  %s "
                         "-g <goodiorfile> "
                         "-b <badiorfile> "
                         "-c <svc_conf_file>"
                         "-p <port> "
                         "-v "
                         "\n",
                         argv [0]),
                        -1);
  }
  // Indicates successful parsing of the command line
  return 0;
}

int
make_ior (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          Hello * servant,
          const ACE_TCHAR *ior_file)
{
  CORBA::String_var poa_name = poa->the_name();
  ACE_DEBUG ((LM_DEBUG, "Creating IOR from %C\n", poa_name.in()));

  PortableServer::ObjectId_var oid = poa->activate_object (servant);
  CORBA::Object_var o = poa->id_to_reference (oid.in ());


  if (host_form == from_hostname || host_form == use_localhost)
    {
      CORBA::String_var ior =
        orb->object_to_string (o.in ());

      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file %s for writing IOR: %C",
                           ior_file,
                           ior.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }
  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  CORBA::Object_var obj;
  PortableServer::POA_var root_poa;
  PortableServer::POAManagerFactory_var poa_manager_factory;

  if (parse_args (argc, argv) != 0)
    return 1;

  const ACE_TCHAR *e1_format= ACE_TEXT ("iiop://%s:%d");
  const ACE_TCHAR *e2_format= ACE_TEXT ("iiop://%s:%d/hostname_in_ior=unreachable");
  ACE_TCHAR hostname[256];
  int num_extra = 4;

  switch (host_form)
    {
    case from_hostname:
      ACE_OS::hostname (hostname, sizeof(hostname) / sizeof (ACE_TCHAR));
      break;

    case use_localhost:
      ACE_OS::strcpy (hostname, ACE_TEXT ("localhost"));
      break;

    case use_defaulted:
      hostname[0] = ACE_TEXT ('\0');
      break;

    case multi_protocol:
      hostname[0] = ACE_TEXT ('\0');
      e2_format = ACE_TEXT ("diop://");
      num_extra = 6;
      break;
    }

  size_t hostname_len = ACE_OS::strlen (hostname);
  size_t e1_len = ACE_OS::strlen (e1_format) + 5; // 5 for the port#
  size_t e2_len = ACE_OS::strlen (e2_format) + 5;
  ACE_TCHAR **extra = 0;
  ACE_NEW_RETURN (extra, ACE_TCHAR *[num_extra], -1);

  extra[0] = ACE::strnew (ACE_TEXT ("-ORBEndpoint"));
  ACE_NEW_RETURN (extra[1],
                  ACE_TCHAR[e1_len + hostname_len + 1],
                  -1);
  ACE_OS::sprintf (extra[1], e1_format, hostname, endpoint_port);
  extra[2] = ACE::strnew (ACE_TEXT ("-ORBEndpoint"));
  ACE_NEW_RETURN (extra[3],
                  ACE_TCHAR[e2_len + hostname_len + 1],
                  -1);
  ACE_OS::sprintf (extra[3], e2_format, hostname, endpoint_port+1);
  if (host_form == multi_protocol)
    {
      extra[4] = ACE::strnew (ACE_TEXT ("-ORBSvcConf"));
      extra[5] = ACE::strnew (svc_conf_file);
    }

  ACE_TCHAR **largv = new ACE_TCHAR *[argc+num_extra];
  int i = 0;
  for (i = 0; i < argc; i++)
    largv[i] = argv[i];

  ACE_DEBUG ((LM_DEBUG,"server adding args: "));
  for (i = 0; i < num_extra; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "%s ", extra[i]));
      largv[argc+i] = extra[i];
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));

  argc += num_extra;

  try
    {
      orb =
        CORBA::ORB_init (argc, largv, "EndpointPolicy");

      obj =
        orb->resolve_initial_references("RootPOA");

      root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      poa_manager_factory
        = root_poa->the_POAManagerFactory ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception("initialization error ");
      return 1;
    }

  for (i = 0; i < num_extra; i++)
    ACE::strdelete (extra[i]);
  delete [] extra;
  delete [] largv;

  //-----------------------------------------------------------------------

  // Create two valid endpoint policies. One to match each of the generated
  // endpoint arguments supplied to ORB_init().
  PortableServer::POAManager_var good_pm;
  PortableServer::POAManager_var bad_pm;
  CORBA::PolicyList policies;
  policies.length (1);

  EndpointPolicy::EndpointList list;
  list.length (1);
  list[0] = new IIOPEndpointValue_i (ACE_TEXT_ALWAYS_CHAR (hostname), endpoint_port);

  try
    {
      CORBA::Any policy_value;
      policy_value <<= list;
      policies[0] = orb->create_policy (EndpointPolicy::ENDPOINT_POLICY_TYPE,
                                        policy_value);
      good_pm = poa_manager_factory->create_POAManager ("goodPOAManager",
                                                        policies);

      if (host_form == use_defaulted)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "ERROR: Expected to catch policy error with "
                             "defaulted hostname, but didn't.\n"),1);
        }
    }
  catch (CORBA::PolicyError &)
    {
      if (host_form == use_defaulted)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Defaulted hostname properly rejected\n"), 0);
        }
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "ERROR: Unexpectedly caught PolicyError "
                         "exception host_form = %s\n", form_arg), 1);
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Failed to create reachable POA manager");
      return 1;
    }

  list[0] = new IIOPEndpointValue_i("unreachable", endpoint_port+1);
  try
    {
      CORBA::Any policy_value;
      policy_value <<= list;
      policies[0] = orb->create_policy (EndpointPolicy::ENDPOINT_POLICY_TYPE,
                                        policy_value);
      bad_pm = poa_manager_factory->create_POAManager ("badPOAManager",
                                                        policies);
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Failed to create unreachable POA manager");
      return 1;
    }

  try
    {

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      // Create poas assiciated with the each the good poa manager and the
      // bad poa manager.
      policies.length(0);
      PortableServer::POA_var good_poa =
        root_poa->create_POA ("goodPOA",
                              good_pm.in (),
                              policies);

      int result = 0;
      result = make_ior (orb.in(), root_poa.in(), hello_impl, root_ior_file);
      if (result != 0)
        return result;

      result = make_ior (orb.in(), good_poa.in(), hello_impl, good_ior_file);
      if (result != 0)
        return result;

      good_pm->activate ();

      PortableServer::POA_var bad_poa;

      if (host_form != multi_protocol)
        {
          bad_poa =
          root_poa->create_POA ("badPOA",
                                bad_pm.in (),
                                policies);
          result = make_ior (orb.in(), bad_poa.in(), hello_impl, bad_ior_file);
          if (result != 0)
            return result;

          bad_pm->activate ();
        }

      if (host_form == from_hostname || host_form == use_localhost)
        {
          orb->run ();
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
        }
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("cannot run server");
    }
  root_poa->destroy (1, 1);

  orb->destroy ();

  return 0;
}
