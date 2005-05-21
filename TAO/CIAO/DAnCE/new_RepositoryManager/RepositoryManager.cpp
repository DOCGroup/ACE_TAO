// $Id$

//RepositoryManager.cpp, v2.0 Stoyan

#include "new_RepositoryManager_Impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"
#include "ace/Auto_Ptr.h"
using namespace std;

const char * rm_ior = "RepositoryManagerDeamon.ior";


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

	//check the previous implementation for stuff to reuse!

	try
	{
		//init the ORB
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		//Get the root POA object
		CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
		
		//downcast to POA type
		PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

		//activate the POA manager
		PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
		mgr->activate ();

		//create a servant
		CIAO_new_RepositoryManagerDaemon_i* repo = new CIAO_new_RepositoryManagerDaemon_i (orb.in ());

		//trasfer ownership to the POA
		PortableServer::ServantBase_var distributor_owner_transfer(repo);

		//register servant
		CIAO::new_RepositoryManagerDaemon_var RepositoryManagerDeamon = repo->_this ();
		
		//convert the IOR to string
		CORBA::String_var ior = orb->object_to_string (RepositoryManagerDeamon.in ());

		//output the IOR to a file
		FILE* ior_out = ACE_OS::fopen (rm_ior, "w");
		
		if (ior_out == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           rm_ior),
                              1);
		
		ACE_OS::fprintf (ior_out, "%s", ior.in ());
		ACE_OS::fclose (ior_out);

		//Start accepting requests
		orb->run ();

		//allow objects registered with the POA ot get cleaned-up
		root_poa->destroy (1, 1);

		//shutdown the orb
		orb->shutdown (1);

		return 0;
	}
	catch (CORBA::Exception &ex) {
		cerr << "CORBA Exception: " << ex << endl;

		return 1;
	}


  return 0;
}

