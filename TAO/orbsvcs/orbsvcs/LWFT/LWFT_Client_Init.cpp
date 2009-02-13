// -*- C++ -*-
// $Id$

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/ORBInitializer_Registry.h"

#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"

#include "LWFT_Client_Init.h"
#include "ForwardingAgent.h"
#include "Client_ORBInitializer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

LWFT_Client_Init::LWFT_Client_Init (void)
  : agent_ (new ForwardingAgent_i ())
{
}

LWFT_Client_Init::~LWFT_Client_Init (void)
{
}

CORBA::ORB_ptr
LWFT_Client_Init::init (int &argc,
                        char *argv[],
                        const ACE_TCHAR *orb_name)
{
  CORBA::ORB_var the_orb;

  try
    {
      // Create an ORB initializer and register it.
      PortableInterceptor::ORBInitializer_var orb_initializer =
        new Client_ORBInitializer (agent_);

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
      
      // fa_thread_ and calling app both own ORB reference.
      the_orb = CORBA::ORB_init (argc, argv, orb_name);
      
      //====================================================
      // Must do this so we can create the Forwarding Agent
      // object reference. If the calling application also
      // does it, the second set of steps is idempotent.
      
      CORBA::Object_var obj =
        the_orb->resolve_initial_references ("RootPOA");
        
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());
        
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
        
      poa_manager->activate ();
      
      //====================================================

      // Resolve the Naming Service and get the objref of the RM.
      TAO_Naming_Client naming_client;
      naming_client.init (the_orb);
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
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("LWFT_Client_Init::init :  exception caught :");
      return CORBA::ORB::_nil ();;
    }
  
  return the_orb._retn ();
}
