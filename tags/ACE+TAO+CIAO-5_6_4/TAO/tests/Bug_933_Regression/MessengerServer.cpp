// $Id$

#include "Messenger_i.h"

const char *ior_output_file = "Messenger.ior";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    //Get reference to Root POA
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate POA Manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create an object
    Messenger_i messenger_servant (orb.in ());

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid =
      poa->activate_object( &messenger_servant );
    CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( messenger_obj.in() );
    FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
    if (output_file == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot open output file for writing IOR: %s\n",
                          ior_output_file),
                          1);
    ACE_OS::fprintf (output_file, "%s", str.in ());
    ACE_OS::fclose (output_file);

    // Accept requests
    orb->run();
    orb->destroy();
  }
  catch( const CORBA::Exception & ex)
  {
    ex._tao_print_exception ("Caught CORBA exception: ");
    return 1;
  }

  return 0;
}
