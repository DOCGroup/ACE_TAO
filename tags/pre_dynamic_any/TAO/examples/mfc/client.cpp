// $Id$

#include "tao\corba.h"
#include "W32_TestC.h"

int
main (int argc, char *argv[])
{
  const char *orb_name = "";

  cout << "Initializing the ORB!" << endl;
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
  cout << "Reading the IOR!" << endl;

  const char *filename =
    "file://ior.txt";

  orb_obj =
    the_orb->string_to_object (filename);

  cout << "Narrowing the IOR!" << endl;

  W32_Test_Interface_var mycall = 
    W32_Test_Interface::_narrow (orb_obj.in ());

  cout << "Sending the Request!" << endl;
  char *response = mycall->getresponse (1);
  cout << "The answer ..." << response << endl;

  // Free up the string.
  CORBA::string_free (response);

  return 0;
}
