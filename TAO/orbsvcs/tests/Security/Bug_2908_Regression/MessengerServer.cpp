// $Id$

#include "Messenger_i.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("server.ior");

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    //Get reference to Root POA
    CORBA::Object_var POA_obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( POA_obj.in() );

    // Activate POA Manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create an object
    Messenger_i messenger_servant (orb);
    Messenger_var messenger_factory = messenger_servant._this ();

    CORBA::String_var ior =
      orb->object_to_string (messenger_factory.in ());

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

    // Accept requests
    orb->run();
    orb->destroy();
  }
  catch (const CORBA::Exception&)
  {
    ACE_ERROR((LM_ERROR, "Caught a CORBA exception: "));
    return 1;
  }

  return 0;
}
