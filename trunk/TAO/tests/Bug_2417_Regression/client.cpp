// $Id$

#include "subscriber_impl.h"
#include "publisherC.h"
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

		Subscriber_impl subscriber(orb.in ());
		Subscriber_var subscriber_var = subscriber._this();

		CORBA::Object_var object = orb->string_to_object("file://ior.out");
		Publisher_var publisher = Publisher::_narrow(object.in());

		publisher->subscribe(subscriber_var.in());

		poa_manager->activate();

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
