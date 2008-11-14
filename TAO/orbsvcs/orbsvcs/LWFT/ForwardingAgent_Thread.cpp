// -*- C++ -*-
// $Id$

#include "tao/ORBInitializer_Registry.h"

#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"

#include "ForwardingAgent_Thread.h"
#include "ForwardingAgent.h"
#include "Client_ORBInitializer.h"

ForwardingAgent_Thread::ForwardingAgent_Thread (
  CORBA::ORB_ptr orb,
  ForwardingAgent_i *agent,
  ACE_Barrier &barrier)
  : orb_ (orb), 
    agent_ (agent),
    synchronizer_ (barrier)
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
