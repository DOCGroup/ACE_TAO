// -*- C++ -*-
// $Id$

#include "tao/ORBInitializer_Registry.h"

#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"

#include "ForwardingAgent_Thread.h"
#include "ForwardingAgent.h"
#include "Client_ORBInitializer.h"

ForwardingAgent_Thread::ForwardingAgent_Thread (
  CORBA::ORB_ptr orb,
  ForwardingAgent_i *agent)
  : orb_ (orb), 
    agent_ (agent),
    synchronizer_ (2)
{
}

int
ForwardingAgent_Thread::svc (void)
{
  try
    {
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
      poa_manager->activate ();

      TAO_Naming_Client naming_client;
      naming_client.init (this->orb_);
      CosNaming::NamingContext_var root_context =
        naming_client.get_context ();
        
      CosNaming::Name rm_name;
      rm_name.length (1UL);
      rm_name[0UL].id = "ReplicationManager";
      
      CORBA::Object_var tmp = root_context->resolve (rm_name);
      ReplicationManager_var rm =
        ReplicationManager::_narrow (tmp.in ());

      agent_->initialize (rm.in ());
      synchronizer_.wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("fw agent thread: exception caught:");
      return 1;
    }
    
  return 0;
}

int
ForwardingAgent_Thread::activate (long /* flags */,
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
  // Task activation flags. If we are using RTCORBA, we want to
  // eliminate the default THR_INHERIT_SCHED flag and also use
  // whatever flags may be set in the ORB.
  long flags =
    THR_NEW_LWP
    | THR_JOINABLE
    | orb_->orb_core ()->orb_params ()->thread_creation_flags ();

  // This will end up back in our overridden svc() method. We
  // want to wait for it to execute the statements in its body
  // before returning control to the calling client.
  int retval = this->ACE_Task_Base::activate (flags, 1, 0, 0);
  synchronizer_.wait ();
  return retval;
}

