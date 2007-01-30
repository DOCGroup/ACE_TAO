// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_stdio.h"
#include "CrashPoint.h"

ACE_RCSID (Redirection,
           server,
           "$Id$")

const char *ior_file = 0;
int number = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:c:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      case 'c':
        crash_point = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'n':
        number = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o <IOR> "
                           "-c <CrashPoint> "
                           "-n <server number> \n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (::parse_args (argc, argv) != 0)
        return -1;

      test_i *test_impl;
      ACE_NEW_RETURN (test_impl, test_i (number, orb.in ()), 1);
      PortableServer::ServantBase_var owner_transfer (test_impl);

      RedirectionTest::test_var test =
        test_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (test.in ());

      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file <%s> for writing "
                           "IOR: %s",
                           ior.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (1, 1);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
