// $Id$

#include "tao\corba.h"
#include "W32_TestC.h"

int
main (int argc, char *argv[])
{
	const char* orb_name = "";
	CORBA::ORB_var _the_orb;
	PortableServer::POA_var _the_root_poa;
	PortableServer::POAManager_var _the_poa_manager;

	CORBA::Object_var orb_obj;

	cout << "Initializing the ORB!" << endl;
	_the_orb = CORBA::ORB_init (argc,argv,orb_name);

    orb_obj = _the_orb->resolve_initial_references("RootPOA");

	_the_root_poa = PortableServer::POA::_narrow (orb_obj.in ());
	_the_poa_manager = _the_root_poa->the_POAManager ();

	// Retrieving the servants IOR from a file
	cout << "Reading the IOR!" << endl;
	const char* filename = "file://ior.txt";
	orb_obj = _the_orb->string_to_object (filename);

	cout << "Narrowing the IOR!" << endl;
	W32_Test_Interface_var mycall = 
		  W32_Test_Interface::_narrow (orb_obj.in ());

	CORBA::String response;

	cout << "Sending the Request!" << endl;
	response = mycall->getresponse(1);

	cout << "The answer ..." << response << endl;

	return 0;
}
