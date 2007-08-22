// $Id$

#include "w32_testS.h"
#include "ace/streams.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      const char *orb_name = "";

      ACE_DEBUG ((LM_DEBUG,
                  "Initializing the ORB!\n"));
      CORBA::ORB_var the_orb = CORBA::ORB_init (argc,
                                                argv,
                                                orb_name);

      CORBA::Object_var orb_obj =
        the_orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var the_root_poa =
        PortableServer::POA::_narrow (orb_obj.in ());

      PortableServer::POAManager_var the_poa_manager =
        the_root_poa->the_POAManager ();

      // Retrieving the servants IOR from a file
      ACE_DEBUG ((LM_DEBUG,
                  "Reading the IOR!\n"));

      const char *filename =
        "file://ior.txt";

      orb_obj =
        the_orb->string_to_object (filename);

      ACE_DEBUG ((LM_DEBUG,
                  "Narrowing the IOR!\n"));

      W32_Test_Interface_var mycall =
        W32_Test_Interface::_narrow (orb_obj.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Sending the Request!\n"));
      char *response = mycall->getresponse (1);
      ACE_DEBUG ((LM_DEBUG,
                  "The answer ...%s\n", response));

      // Free up the string.
      CORBA::string_free (response);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
