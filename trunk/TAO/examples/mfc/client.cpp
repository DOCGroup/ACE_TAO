// $Id$

#include "w32_testS.h"
#include "ace/streams.h"


int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      const char *orb_name = "";

	  ACE_DEBUG ((LM_DEBUG,
                  "Initializing the ORB!\n"));
      CORBA::ORB_var the_orb = CORBA::ORB_init (argc,
                                                argv,
                                                orb_name
                                                ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var orb_obj =
        the_orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var the_root_poa =
        PortableServer::POA::_narrow (orb_obj.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var the_poa_manager =
        the_root_poa->the_POAManager ();

      // Retrieving the servants IOR from a file
	  ACE_DEBUG ((LM_DEBUG,
                  "Reading the IOR!\n"));

      const char *filename =
        "file://ior.txt";

      orb_obj =
        the_orb->string_to_object (filename ACE_ENV_ARG_PARAMETER);

	  ACE_DEBUG ((LM_DEBUG,
                  "Narrowing the IOR!\n"));

      W32_Test_Interface_var mycall =
        W32_Test_Interface::_narrow (orb_obj.in () ACE_ENV_ARG_PARAMETER);

	  ACE_DEBUG ((LM_DEBUG,
                  "Sending the Request!\n"));
      char *response = mycall->getresponse (1);
	  ACE_DEBUG ((LM_DEBUG,
                  "The answer ...%s\n", response));

      // Free up the string.
      CORBA::string_free (response);
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
