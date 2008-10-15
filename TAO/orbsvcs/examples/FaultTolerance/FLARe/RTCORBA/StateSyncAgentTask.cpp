#include "StateSyncAgentTask.h"
#include "StateSynchronizationAgent_i.h"
#include <tao/PortableServer/POAC.h>

StateSyncAgentTask::StateSyncAgentTask (CORBA::ORB_ptr orb,
					StateSynchronizationAgent_i * agent,
					ACE_Barrier * sync)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    agent_ (agent),
    sync_ (sync),
    agent_ref_ (StateSynchronizationAgent::_nil ())
{
}

int
StateSyncAgentTask::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // ***************************************************
      // activate state synchronzation agent

      PortableServer::ObjectId_var ssa_oid =
	root_poa->activate_object (agent_);
 
      CORBA::Object_var ssa_object =
        root_poa->id_to_reference (ssa_oid.in ());

      agent_ref_ =
	StateSynchronizationAgent::_narrow (ssa_object.in ());

      poa_manager->activate ();

      // main loop has to wait until the agent servant is registered
      // within the root poa
      sync_->wait ();

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

StateSynchronizationAgent_ptr
StateSyncAgentTask::agent_ref ()
{
  return agent_ref_.in ();
}
