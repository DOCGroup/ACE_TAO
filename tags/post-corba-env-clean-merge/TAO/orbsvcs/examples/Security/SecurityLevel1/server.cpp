// $Id$

#include "SLevel1_Test_i.h"

ACE_RCSID (SecurityLevel1,
           server,
           "$Id$")

const char *ior_output_file = 0;

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      /// Our regular ORB Initialization.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Get a reference to the RootPOA.
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      /// Narrow down the reference to the currect interface.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      SLevel1_Server_i level1_server ();

      SLevel1_Server_var server =
        level1_server._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in ()
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // Start the ORB
      orb->run ();

      root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCH (CORBA::SytemException, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "System Exception raised: %s", ex));
    }
  ACE_ENDTRY;

  return 0;
}
