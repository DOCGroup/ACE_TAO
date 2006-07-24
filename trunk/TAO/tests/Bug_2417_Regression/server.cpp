// $Id$

#include "publisher_impl.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

class ThreadPool : public ACE_Task_Base
{
public:
	ThreadPool (CORBA::ORB_ptr orb);
	virtual int svc (void);
private:
	CORBA::ORB_var orb_;
};

int
main (int argc, char *argv[])
{
	ACE_TRY_NEW_ENV
    {
		CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");
		CORBA::Object_var poa_object = orb->resolve_initial_references("RootPOA");

		if (CORBA::is_nil (poa_object.in ()))
			ACE_ERROR_RETURN ((LM_ERROR, " (%P|%t) Unable to initialize the POA.\n"), 1);

		PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in ());
		PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager();

		Publisher_impl publisher(orb.in ());
		Publisher_var publisher_var = publisher._this();

		CORBA::String_var ior = orb->object_to_string(publisher_var.in());
		ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in()));

		// output the ior
		FILE *output_file= ACE_OS::fopen ("ior.out", "w");
		if (output_file == 0)
			ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output file for writing IOR: ior.out"), 1);
        ACE_OS::fprintf (output_file, "%s", ior.in ());
        ACE_OS::fclose (output_file);

		poa_manager->activate();

			const CORBA::Object_var pmobj ( orb->resolve_initial_references( "ORBPolicyManager" ) );
			CORBA::PolicyManager_var policy_manager = CORBA::PolicyManager::_narrow( pmobj.in() );

			CORBA::Any orb_level;
			orb_level <<= Messaging::SYNC_NONE;
			CORBA::PolicyList policy_list;
			policy_list.length(1);
			policy_list[0] = orb->create_policy(Messaging::SYNC_SCOPE_POLICY_TYPE, orb_level);
			policy_manager->set_policy_overrides(policy_list, CORBA::SET_OVERRIDE);

		ThreadPool pool (orb.in ());
		if (pool.activate(THR_NEW_LWP | THR_JOINABLE, 5) != 0)
			ACE_ERROR_RETURN ((LM_ERROR, "Cannot activate client threads\n"), 1);

		pool.thr_mgr ()->wait ();

		ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
	}
	ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
      return 1;
    }
	ACE_ENDTRY;

	return 0;
}

// ****************************************************************

ThreadPool::ThreadPool(CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate (orb))
{
}

int ThreadPool::svc (void)
{
	try
	{
		this->orb_->run ();
    }
	catch (...)
    {
    }
	return 0;
}
