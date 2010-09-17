// $Id$

#include "tao/PortableServer/POAC.h"
#include "tao/PortableServer/POAManagerFactoryC.h"
#include "tao/EndpointPolicy/EndpointPolicy.h"
#include "tao/EndpointPolicy/IIOPEndpointValue_i.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_file = ACE_TEXT("test.ior");

CORBA::Short endpoint_port = 12345;
int verbose = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  for (int c = 1; c < argc; c++) {
    if (ACE_OS::strcasecmp(argv[c],ACE_TEXT("-o")) == 0)
      {
        ior_file = argv[++c];
      }
    else if (ACE_OS::strcasecmp(argv[c],ACE_TEXT("-p")) == 0)
      {
        endpoint_port = ACE_OS::atoi (argv[++c]);
      }
    else if (ACE_OS::strstr(argv[c],ACE_TEXT("-ORB")) == argv[c])
      {
        c++;
        continue;
      }
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         "usage:  %s "
                         "-o <iorfile> "
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  CORBA::Object_var obj;
  PortableServer::POA_var root_poa;
  PortableServer::POAManagerFactory_var poa_manager_factory;

  if (parse_args (argc, argv) != 0)
    return 1;
  ACE_TCHAR *extra[4];
#ifdef ACE_USES_WCHAR
  extra[0] = CORBA::wstring_dup (ACE_TEXT ("-ORBEndpoint"));
  extra[1] =CORBA::wstring_alloc (100);
#else
  extra[0] = CORBA::string_dup ("-ORBEndpoint");
  extra[1] = CORBA::string_alloc (100);
#endif
  ACE_OS::sprintf (extra[1],
                   ACE_TEXT ("iiop://localhost:%d/ssl_port=%d"),
                   endpoint_port, endpoint_port+1);

#ifdef ACE_USES_WCHAR
  extra[2] = CORBA::wstring_dup (ACE_TEXT ("-ORBEndpoint"));
  extra[3] = CORBA::wstring_alloc (100);
#else
  extra[2] = CORBA::string_dup ("-ORBEndpoint");
  extra[3] = CORBA::string_alloc (100);
#endif
  ACE_OS::sprintf (extra[3],
                   ACE_TEXT ("iiop://localhost:%d/ssl_port=%d"),
                   endpoint_port+10, endpoint_port+11);

  ACE_TCHAR **largv = new ACE_TCHAR *[argc+4];
  int i = 0;
  for (i = 0; i < argc; i++)
    largv[i] = argv[i];
  for (i = 0; i < 4; i++)
    largv[argc+i] = extra[i];
  argc += 4;

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

  for (i = 0; i < 4; i++)
#ifdef ACE_USES_WCHAR
    CORBA::wstring_free (extra[i]);
#else
    CORBA::string_free (extra[i]);
#endif

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
  list[0] = new IIOPEndpointValue_i("localhost", endpoint_port);

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

      ACE_DEBUG ((LM_DEBUG, "Creating IOR from good poa\n"));

      o = good_poa->create_reference ("IDL:JustATest:1.0");

      CORBA::String_var good_ior =
        orb->object_to_string (o.in ());

      output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", good_ior.in ());
      ACE_OS::fclose (output_file);
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("cannot run server");
    }
  root_poa->destroy (1, 1);

  orb->destroy ();

  return 0;
}
