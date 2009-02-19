// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"

#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"

#include "orbsvcs/orbsvcs/LWFT/ForwardingAgent.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManager.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Server_Init.h"
#include "orbsvcs/orbsvcs/LWFT/AppOptions.h"
#include "orbsvcs/orbsvcs/LWFT/StateSyncAgentTask.h"

#include "RMOptions.h"

char *ior_output_file = "rm.ior";

class RegistrationTask : public ACE_Task_Base
{
public:
  RegistrationTask (ReplicationManager_ptr primary_rm,
                    ReplicationManager_ptr local_rm,
                    CORBA::ORB_ptr orb)
    : primary_rm_ (ReplicationManager::_duplicate (primary_rm)),
      local_rm_ (ReplicationManager::_duplicate (local_rm)),
      orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  int svc (void)
  {
    try
      {
        // register ReplicationManager with itself
        primary_rm_->register_application (
          local_rm_->object_id (),
          0.0,
          AppOptions::instance ()->host_id ().c_str (),
          AppOptions::instance ()->process_id ().c_str (),
          AppOptions::instance ()->role (),
          local_rm_.in ());
      }
    catch (CORBA::Exception &)
      {
        ACE_ERROR ((LM_ERROR, "RM: Could not register reference to "
                    " the ReplicationManager with the primary "
                    "ReplicationManager.\n"));
        return 1;
      }

    return 0;
  }

private:
  ReplicationManager_var primary_rm_;
  ReplicationManager_var local_rm_;
  CORBA::ORB_var orb_;
};

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      AppOptions::instance ()->parse_args (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
	      root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
  
      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
	      root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var poa = 
        root_poa->create_POA ("Servant POA",
                              poa_manager.in (),
                              policies);

      poa_manager->activate ();

      //      std::cout << "Before StateSyncAgent." << std::endl;

      StateSynchronizationAgent_i* ssa_servant =
	      new StateSynchronizationAgent_i (
	        orb.in (),
		AppOptions::instance ()->host_id (),
		AppOptions::instance ()->process_id (),
		!(AppOptions::instance ()->use_dds ()));

      PortableServer::ServantBase_var owner_xfer_ssa (ssa_servant);

      // create task for state synchronization agent
      StateSyncAgentTask sync_agent_thread (orb.in (),
                                            ssa_servant);

      int result = sync_agent_thread.activate ();
      
      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "StateSyncAgentTask::activate () "
                             "returned %d, errno = %d\n", 
                             result, 
                             errno),
                            -1);
        }

      if (! RMOptions::instance ()->parse_args (argc, argv))
        {  
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Replication Manager options "
                             "are incorrect.\n"),
                            -1);
        }
        
      ReplicationManager_i *rm_i = 0;
      ACE_NEW_RETURN (rm_i, 
                      ReplicationManager_i (orb.in (), 
                                            RMOptions::instance()->hertz(), 
                                            RMOptions::instance()->proactive(),
					    RMOptions::instance()->static_mode ()),
					    1);

      PortableServer::ServantBase_var owner_transfer (rm_i);

      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (rm_i->object_id ());

      poa->activate_object_with_id (oid.in (), rm_i);

      CORBA::Object_var rm_object = poa->id_to_reference (oid.in ());
      ReplicationManager_var rm = ReplicationManager::_narrow (rm_object.in ());

      CORBA::String_var ior =
        orb->object_to_string (rm.in ());
      
//      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));
      
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          
          if (output_file == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot open output file "
                                 "for writing IOR: %s",
                                 ior_output_file),
                                1);
            }
            
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }
      
      ReplicationManager_var primary_rm;

      if (RMOptions::instance ()->use_naming_service ())
        {
          TAO_Naming_Client naming_client;
          naming_client.init (orb.in ());
          
          CosNaming::Name rm_name;
          rm_name.length (1UL);
          rm_name[0UL].id = "ReplicationManager";

          if (AppOptions::instance ()->role () == PRIMARY)
            {
              try
                {
                  naming_client->bind (rm_name, rm.in ());
                }
              catch (CosNaming::NamingContext::AlreadyBound &)
                {
                  naming_client->rebind (rm_name, rm.in ());
                }
          
              primary_rm = ReplicationManager::_duplicate (rm.in ());

              //          ACE_DEBUG ((LM_INFO,
              //                      "ReplicationManager registered with Naming Service\n"));
            }
          else
            {
              CORBA::Object_var rm_obj = naming_client->resolve (rm_name);
              
              primary_rm = ReplicationManager::_narrow (rm_obj.in ());
            }
        }
      else
        {
          primary_rm = ReplicationManager::_duplicate (rm.in ());
        }

      // add reference for state synchronization of the RM itself
      rm_i->agent (sync_agent_thread.agent_ref ());

      // register its own StateSynchronizationAgent
      rm_i->register_state_synchronization_agent (
        AppOptions::instance ()->host_id ().c_str (),
	      AppOptions::instance ()->process_id ().c_str (),
	      sync_agent_thread.agent_ref ());

      sync_agent_thread.agent_ref ()->register_application (rm_i->object_id (),
                                                            rm.in ());

      RegistrationTask registration_task (
                         primary_rm.in (),
                         rm.in (),
                         orb.in ());

      if (registration_task.activate () == 0)
        {
          orb->run ();
        }

      // ACE_DEBUG ((LM_DEBUG, "(%P|%t) RM - event loop finished\n"));

      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("RM Process: caught exception:");
      return -1;
    }

  return 0;
}
