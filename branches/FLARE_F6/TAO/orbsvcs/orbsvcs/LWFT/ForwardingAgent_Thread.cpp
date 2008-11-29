// -*- C++ -*-
// $Id$

#include "tao/ORBInitializer_Registry.h"

#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"

#include "tao/PortableServer/Servant_Base.h"
#include "tao/ORBInitializer_Registry.h"

#include "ForwardingAgent_Thread.h"
#include "ForwardingAgent.h"
#include "Client_ORBInitializer.h"

ForwardingAgent_Thread::ForwardingAgent_Thread (void)
  : synchronizer_ (2)
{
}

ForwardingAgent_Thread::~ForwardingAgent_Thread (void)
{
}

int
ForwardingAgent_Thread::svc (void)
{
  try
    {
      // Resolve the Naming Service and get the objref of the RM.
      TAO_Naming_Client naming_client;
      naming_client.init (orb_);
      CosNaming::NamingContext_var root_context =
        naming_client.get_context ();
        
      CosNaming::Name rm_name;
      rm_name.length (1UL);
      rm_name[0UL].id = "ReplicationManager";
      
      CORBA::Object_var tmp = root_context->resolve (rm_name);
      ReplicationManager_var rm =
        ReplicationManager::_narrow (tmp.in ());

      // Give ownership to the POA in this thread.
      PortableServer::ServantBase_var owner_transfer (agent_);
      
      // Create the object reference and register it with the RM.
      agent_->initialize (rm.in ());
      
      // Make sure the steps above finish before returning from
      // activate() to the calling LWFT client.
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

void
ForwardingAgent_Thread::register_orb_initializer (void)
{
  // Create the forwarding agent - ownership will be given to
  // the POA in svc().
  agent_ = new ForwardingAgent_i;

  // Create an ORB initializer and register it.
  PortableInterceptor::ORBInitializer_var orb_initializer =
    new Client_ORBInitializer (agent_);

  PortableInterceptor::register_orb_initializer (
    orb_initializer.in ());
}

void
ForwardingAgent_Thread::orb (CORBA::ORB_ptr the_orb)
{
  orb_ = the_orb;
}

