// $Id$

#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/CORBA_macros.h"

const char *ior_output_file = "test.ior";

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      test_impl svnt;
      test_var server = svnt._this ();

      CORBA::String_var ior =
	      orb->object_to_string (server.in ());

	    FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

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

      poa_manager->activate ();
      orb->run ();

      /// Destroy the POA, waiting until the destruction terminates,
      /// then the ORB.
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Server: exception caught:");
      return 1;
    }

  return 0;
}
