// $Id$

#include "test_i.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (TC_Alignment,
           server,
           "$Id$")

const char *ior_output_file = "test.ior";

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      test_i servant (orb.in ());

      test_var server =
        servant._this ();

      CORBA::String_var ior =
              orb->object_to_string (server.in ()
                               ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG,
                  "Activated as <%s>\n",
                  ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
              {
                FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

                if (output_file == 0)
            {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                                   "Can't open output file for writing IOR: %s",
                                                   ior_output_file),
                                                  1);
            }

                ACE_OS::fprintf (output_file,
                           "%s",
                           ior.in ());
                ACE_OS::fclose (output_file);
              }

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Server: exception caught - ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
