#include "ace/Get_Opt.h"

#include "test_i.h"
#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"

#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_stdio.h"


ACE_RCSID (ORB_Shutdown,
           server,
           "$Id$")


const char *ior_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o IOR\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      bool destroy_called = false;

#if TAO_HAS_INTERCEPTORS == 1
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer (destroy_called),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB" ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (::parse_args (argc, argv) != 0)
        return -1;

      test_i servant (orb.in ());

      CORBA::Object_var obj =
        servant._this ();

      CORBA::String_var ior =
        orb->object_to_string (obj.in ()
                               ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG,
                  "test servant: <%s>\n",
                  ior.in ()));

      // Write IOR to a file.
      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file <%s> for writing "
                           "IOR: %s",
                           ior.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

      orb->destroy ();

      ACE_ASSERT (destroy_called == true);

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
