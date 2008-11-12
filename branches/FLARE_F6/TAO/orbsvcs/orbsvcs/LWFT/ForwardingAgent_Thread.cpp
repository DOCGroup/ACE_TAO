// -*- C++ -*-
// $Id$

#include "tao/ORBInitializer_Registry.h"

#include "ForwardingAgent_Thread.h"
#include "ForwardingAgent.h"
#include "Client_ORBInitializer.h"

ForwardingAgent_Thread::ForwardingAgent_Thread (
  CORBA::ORB_ptr orb)
  : orb_ (orb), 
    agent_ (0),
    synchronizer_ (2)
{
}

int
ForwardingAgent_Thread::svc (void)
{
  try
    {
      ACE_NEW_RETURN (this->agent_,
                      ForwardingAgent_i,
                      1);
      PortableServer::ServantBase_var owner_transfer (this->agent_);

      // ******************************************************

      // ******************************************************
      // register request interceptor
      
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer (this->agent_),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
      
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
const char *rm_ior = "rm.ior";
      CORBA::Object_var tmp =
        this->orb_->string_to_object (rm_ior);
      ReplicationManager_var rm =
        ReplicationManager::_narrow (tmp.in ());

      this->agent_->initialize (rm.in ());
      this->synchronizer_.wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("fw agent thread: exception caught:");
      return 1;
    }
    
  return 0;
}
