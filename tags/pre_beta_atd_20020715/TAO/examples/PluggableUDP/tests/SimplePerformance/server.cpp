// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/PluggableUDP/tests/SimplePerformance
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Implementation of the server running the UDP object.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================


#include "ace/Get_Opt.h"

#include "test_i.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

#if defined (VXWORKS)
# undef ACE_MAIN
# define ACE_MAIN testServer
#endif

const char *ior_output_file = "test.ior";

int main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "ORB_Test_Server" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_i server_impl (orb.in ());

      PortableServer::ObjectId_var oidServer = root_poa->activate_object (&server_impl ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var objOne = root_poa->id_to_reference (oidServer.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server = Simple_Server::_narrow (objOne.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
              orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

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

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
