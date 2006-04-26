// $Id$

#include "Hello.h"
#include "tao/EndpointPolicy/EndpointPolicy.h"
#include "tao/EndpointPolicy/IIOPEndpointValue_i.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

const char *good_ior_file = "good.ior";
const char *bad_ior_file = "bad.ior";
const char *root_ior_file = "root.ior";

CORBA::Short endpoint_port = 12345;
int verbose = 0;

int
parse_args (int argc, char *argv[])
{
  for (int c = 1; c < argc; c++) {
    if (ACE_OS::strcasecmp(argv[c],"-g") == 0)
      {
        good_ior_file = argv[++c];
      }
    else if (ACE_OS::strcasecmp(argv[c],"-b") == 0)
      {
        bad_ior_file = argv[++c];
      }
    else if (ACE_OS::strcasecmp(argv[c],"-p") == 0)
      {
        endpoint_port = ACE_OS::atoi (argv[++c]);
      }
    else if (ACE_OS::strcasecmp(argv[c],"-v") == 0)
      {
        verbose = 1;
      }
    else if (strstr(argv[c],"-ORB") == argv[c])
      {
        c++;
        continue;
      }
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         "usage:  %s "
                         "-g <goodiorfile> "
                         "-b <badiorfile> "
                         "-p <port> "
                         "-v "
                         "\n",
                         argv [0]),
                        -1);
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
add_endpoint_args(int &argc, char**argv, char** &largv)
{
  largv = new char*[argc+4];
  for (int i = 0; i < argc; i++)
    largv[i] = argv[i];
  largv[argc++] = "-ORBEndpoint";
  largv[argc] = new char[100];
  ACE_OS::sprintf (largv[argc++],"iiop://localhost:%d",endpoint_port);
  largv[argc++] = "-ORBEndpoint";
  largv[argc] = new char[100];
  ACE_OS::sprintf (largv[argc++],"iiop://localhost:%d/hostname_in_ior=unreachable",endpoint_port+1);
}

int
main (int argc, char *argv[])
{

  CORBA::ORB_var orb;
  CORBA::Object_var obj;
  PortableServer::POA_var root_poa;
  PortableServer::POAManagerFactory_var poa_manager_factory;

  try
    {
      if (parse_args (argc, argv) != 0)
        return 1;
      char **largv;
      add_endpoint_args(argc,argv, largv);
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

  // Currently, only single ENDPOINT_POLICY_TYPE policy is supported in
  // the POAManager. A PolicyError exception with BAD_POLICY reason is
  // raised if the POAManager has multiple policies.
  bool policy_error_test = false;
  CORBA::PolicyList policies;

  try
    {
      policies.length (2);
      PortableServer::POAManager_var poa_manager
        = poa_manager_factory->create_POAManager ("wrongPOAManager",
                                                  policies);
    }
  catch (CORBA::PolicyError &ex)
    {
      if (ex.reason == CORBA::BAD_POLICY)
        policy_error_test = true;
    }

  if (! policy_error_test)
    return 1;

  policy_error_test = false;

  // A PolicyError exception with BAD_POLICY reason is raised if
  // the POAManager has other policies.
  try
    {
      policies.length (1);

      CORBA::Any policy_value;
      policy_value <<= PortableServer::ORB_CTRL_MODEL;

      policies[0] = orb->create_policy (PortableServer::THREAD_POLICY_ID,
                                        policy_value);

      PortableServer::POAManager_var poa_manager
        = poa_manager_factory->create_POAManager ("wrongPOAManager",
                                                  policies);
    }
  catch (CORBA::PolicyError &ex)
    {
      if (ex.reason == CORBA::BAD_POLICY)
        policy_error_test = true;
    }

  if (! policy_error_test)
    return 1;

  policy_error_test = false;

#if 0
  // @@@ mesnier_p@ociweb.com
  // This test is currently blocked out due to the vagueries of hostname
  // creation at intermediate stages of endpoint production. See my note
  // in tao/EndpointPolicy/IIOPEndpointValue_i.cpp (validate_acceptor())
  // for more information about this situation.

  // A PolicyError exception with UNSUPPORTED_POLICY_VALUE reason
  // is raised if the Endpoint policy does not contain an endpoint
  // that match any endpoint the ORB is listening on.
  try
    {
      policies.length (1);

      CORBA::ULong port = 0xdead;
      EndpointPolicy::EndpointValueBase_var iiop_endpoint
        = new IIOPEndpointValue_i("localhost", port);

      EndpointPolicy::EndpointList list;
      list.length (1);

      list[0] = iiop_endpoint;

      CORBA::Any policy_value;
      policy_value <<= list;

      policies[0] = orb->create_policy (EndpointPolicy::ENDPOINT_POLICY_TYPE,
                                        policy_value);

      PortableServer::POAManager_var poa_manager
        = poa_manager_factory->create_POAManager ("wrongPOAManager",
                                                  policies);
    }
  catch (CORBA::PolicyError &ex)
    {
      if (ex.reason == CORBA::UNSUPPORTED_POLICY_VALUE)
        policy_error_test = true;
    }

  if (! policy_error_test)
    return 1;
#endif
  //-----------------------------------------------------------------------


  // Create two valid endpoint policies. One to match each of the generated
  // endpoint arguments supplied to ORB_init().
  PortableServer::POAManager_var good_pm;
  PortableServer::POAManager_var bad_pm;

  policies.length (1);

  EndpointPolicy::EndpointValueBase_var iiop_endpoint =
    new IIOPEndpointValue_i("localhost", endpoint_port);

  EndpointPolicy::EndpointList list;
  list.length (1);
  list[0] = iiop_endpoint;

  try
    {
      CORBA::Any policy_value;
      policy_value <<= list;
      policies[0] = orb->create_policy (EndpointPolicy::ENDPOINT_POLICY_TYPE,
                                        policy_value);
      good_pm = poa_manager_factory->create_POAManager ("goodPOAManager",
                                                        policies);
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
      PortableServer::ObjectId_var oid;
      CORBA::Object_var o = CORBA::Object::_nil();
      FILE *output_file= 0;
      // Create poas assiciated with the each the good poa manager and the
      // bad poa manager.
      policies.length(0);
      PortableServer::POA_var good_poa =
        root_poa->create_POA ("goodPOA",
                              good_pm.in (),
                              policies);

      PortableServer::POA_var bad_poa =
        root_poa->create_POA ("badPOA",
                              bad_pm.in (),
                              policies);

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      ACE_DEBUG ((LM_DEBUG, "Creating IOR from root poa\n"));

      oid = root_poa->activate_object (hello_impl);
      o = root_poa->id_to_reference (oid.in ());

      CORBA::String_var root_ior =
        orb->object_to_string (o.in ());

      output_file= ACE_OS::fopen (root_ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           root_ior_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", root_ior.in ());
      ACE_OS::fclose (output_file);

      ACE_DEBUG ((LM_DEBUG, "Creating IOR from bad poa\n"));

      oid = bad_poa->activate_object (hello_impl);
      o = bad_poa->id_to_reference (oid.in());

      CORBA::String_var bad_ior =
        orb->object_to_string (o.in ());

      ACE_DEBUG ((LM_DEBUG, "Creating IOR from good poa\n"));

      oid = good_poa->activate_object (hello_impl);
      o = good_poa->id_to_reference (oid.in ());

      CORBA::String_var good_ior =
        orb->object_to_string (o.in ());

      output_file= ACE_OS::fopen (good_ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           good_ior_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", good_ior.in ());
      ACE_OS::fclose (output_file);

      output_file= ACE_OS::fopen (bad_ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           bad_ior_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", bad_ior.in ());
      ACE_OS::fclose (output_file);

      good_pm->activate ();
      bad_pm->activate ();

      orb->run ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("cannot run server");
    }
  root_poa->destroy (1, 1);

  orb->destroy ();

  return 0;
}
