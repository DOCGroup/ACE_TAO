// -*- C++ -*-
// $Id$

#include "tao/PortableServer/POAC.h"

#include "Barrier_Guard.h"

#include "AppOptions.h"

#include "StateSyncAgentTask.h"
#include "StateSynchronizationAgent_i.h"

StateSyncAgentTask *
StateSyncAgentTask::instance (void)
{
  return
    ACE_Singleton<StateSyncAgentTask, ACE_SYNCH_MUTEX>::instance ();
}

StateSyncAgentTask::StateSyncAgentTask (void)
  : orb_ (AppOptions::instance ()->orb ()),
    sync_ (2),
    agent_ref_ (StateSynchronizationAgent::_nil ()),
    activated_ (false)
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
        // Create state synchronzation agent.
        
        AppOptions *app_opts = AppOptions::instance ();

        agent_ =
	        new StateSynchronizationAgent_i (
            app_opts->host_id (),
            app_opts->process_id (),
            ! app_opts->use_dds ());

        PortableServer::ServantBase_var owner_transfer (agent_);

        // ***************************************************
        // Activate state synchronzation agent.

        PortableServer::ObjectId_var ssa_oid =
	        root_poa->activate_object (agent_);
   
        CORBA::Object_var ssa_object =
          root_poa->id_to_reference (ssa_oid.in ());

        agent_ref_ =
	        StateSynchronizationAgent::_narrow (ssa_object.in ());

        poa_manager->activate ();
      }

      this->orb_->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("SSA task thread: exception caught:"));
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
  if (activated_)
    {
      return 0;
    }
  
  activated_ = true;
    
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
