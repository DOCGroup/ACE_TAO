// -*- C++ -*-
// $Id$

#include "tao/PortableServer/POAC.h"

#include "Barrier_Guard.h"

#include "StateSyncAgentTask.h"
#include "StateSynchronizationAgent_i.h"

StateSyncAgentTask::StateSyncAgentTask (
  CORBA::ORB_ptr orb,
  StateSynchronizationAgent_i * agent)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    agent_ (agent),
    sync_ (2),
    agent_ref_ (StateSynchronizationAgent::_nil ())
{
}

int
StateSyncAgentTask::svc (void)
{
  try
    {
      {
        Barrier_Guard barrier_guard (sync_);

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
      }

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SSA task thread: exception caught:");
      return 1;
    }

  return 0;
}

int
StateSyncAgentTask::activate (long /* flags */,
                              int /* n_threads */,
                              int /* force_active */,
                              long /* priority */,
                              int /* grp_id */,
                              ACE_Task_Base* /* task */,
                              ACE_hthread_t /* thread_handles */ [],
                              void* /* stack */ [],
                              size_t /* stack_size */ [],
                              ACE_thread_t /* thread_ids */ [],
                              const char* /* thr_name */ [])
{
  // This will end up back in our overridden svc() method. We
  // want to wait for it to execute the statements in its body
  // before returning control to the calling application.
  int retval = this->ACE_Task_Base::activate ();
  sync_.wait ();
  return retval;
}

StateSynchronizationAgent_ptr
StateSyncAgentTask::agent_ref (void)
{
  return agent_ref_.in ();
}
