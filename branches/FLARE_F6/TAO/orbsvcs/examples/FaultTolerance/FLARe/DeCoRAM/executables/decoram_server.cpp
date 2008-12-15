// $Id$

#include <sstream>
#include "orbsvcs/orbsvcs/LWFT/AppSideReg.h"
#include "orbsvcs/orbsvcs/LWFT/AppOptions.h"
#include "orbsvcs/orbsvcs/LWFT/StateSyncAgentTask.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Server_Init.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManagerC.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "Worker_i.h"
#include "Name_Helper_T.h"

long priority = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv,
                        "p:h" //client options
                        );
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        priority = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'h':
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\t-p <priority> (defaults to %d)\n",
                           priority),
                           -1);
      }

  return 0;
}


int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (CORBA::is_nil (orb.in ()))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     ACE_TEXT ("ORB could not be initialized.\n")),
			    -1);
	}

      ACE_DEBUG ((LM_TRACE, ACE_TEXT ("ORB initialized.\n")));

      AppOptions::instance ()->parse_args (argc, argv);

      AppSideReg proc_reg (orb.in ());

      int result = proc_reg.activate ();
      
      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppSideReg::activate () returned %d\n",
                             result),
                            -1);
        }
#ifdef USE_STATESYNCHRONIZATION   
      StateSynchronizationAgent_i* ssa_servant =
	      new StateSynchronizationAgent_i (
	        orb.in (),
		AppOptions::instance ()->host_id (),
		AppOptions::instance ()->process_id (),
		!(AppOptions::instance ()->use_dds ()));

      PortableServer::ServantBase_var owner_transfer (ssa_servant);

      // create task for state synchronization agent
      StateSyncAgentTask sync_agent_thread (orb.in (),
					    ssa_servant);

      result = sync_agent_thread.activate ();
      
      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "StateSyncAgentTask::activate () "
                             "returned %d, errno = %d\n", 
                             result, 
                             errno),
                            -1);
        }
#endif
      ReplicationManager_var rm;
      TAO_Naming_Client naming_client;
      naming_client.init (orb.in ());
      
      CosNaming::NamingContextExt_var ns =
        CosNaming::NamingContextExt::_narrow (naming_client.get_context ());

      CORBA::Object_var rm_obj =  ns->resolve_str ("ReplicationManager");
      
      if (CORBA::is_nil (rm_obj.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "ServerTask: Null RM objref from Naming Service\n"));
        }
      else
        {
          rm = ReplicationManager::_narrow (rm_obj.in ());
          
          ACE_DEBUG ((LM_TRACE,
                      "ServerTask: RM resolved from Naming Service\n"));
        }

            if (CORBA::is_nil (rm.in ()))
	{
	  ACE_DEBUG ((LM_ERROR, "ServerTask: could not narrow RM reference."));
	}
      else
	{
          // get reference of the state synchronization agent
          CORBA::Object_var obj =
            orb->resolve_initial_references ("RootPOA");

          PortableServer::POA_var root_poa =
            PortableServer::POA::_narrow (obj.in ());
#ifdef USE_STATESYNCHRONIZATION
          obj = root_poa->servant_to_reference (ssa_servant);

          StateSynchronizationAgent_var agent =
            StateSynchronizationAgent::_narrow (obj.in ());

          if (CORBA::is_nil (agent.in ()))
            {
              ACE_DEBUG ((LM_ERROR, "ServerTask: Could not get object "
                          "reference to StateSynchronizationAgent.\n"));
              return 1;
            }

	  // ***************************************************
	  // register replication agent with the replication manager

	  rm->register_state_synchronization_agent (
            AppOptions::instance ()->host_id ().c_str (),
	    AppOptions::instance ()->process_id ().c_str (),
	    agent.in ());
#else
	  StateSynchronizationAgent_var agent;
#endif
          CORBA::PolicyList policies (1); 
          policies.length (2);

          CORBA::Object_var rtobj = orb->resolve_initial_references ("RTORB");

          RTCORBA::RTORB_var rtorb = RTCORBA::RTORB::_narrow (rtobj.in ());

          policies[0] = rtorb->create_priority_model_policy (RTCORBA::SERVER_DECLARED, 
							     priority);
          policies[1] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);

          // Create a POA with the correct policies 
          PortableServer::POA_var worker_poa = 
            root_poa->create_POA ("Worker_POA",
                                  PortableServer::POAManager::_nil (),
                                  policies);

          Worker_i *servant =
            new Worker_i (orb.in (),
                          root_poa.in (),
                          AppOptions::instance ()->app_id (),
                          agent.in ());

          PortableServer::ServantBase_var safe_servant (servant);
          ACE_UNUSED_ARG (safe_servant);

          PortableServer::ObjectId_var oid =
            PortableServer::string_to_ObjectId (AppOptions::instance ()->app_id ().c_str ());

          worker_poa->activate_object_with_id (oid.in (), servant);

          CORBA::Object_var servant_object =
            root_poa->id_to_reference (oid.in ());

          DeCoRAM::Worker_var worker = DeCoRAM::Worker::_narrow (servant_object.in ());

	  Name_Helper_T <DeCoRAM::Worker> nh (orb.in ());
	  
	  nh.bind ("/FLARe/" + AppOptions::instance ()->app_id (),
		   worker.in ());

          rm->register_application (
            AppOptions::instance ()->app_id ().c_str (),
            AppOptions::instance ()->load (),
            AppOptions::instance ()->host_id ().c_str (),
            AppOptions::instance ()->process_id ().c_str (),
            AppOptions::instance ()->role (),
            worker.in ());

#ifdef USE_STATESYNCHRONIZATION
          agent->register_application (AppOptions::instance ()->app_id ().c_str (),
                                       worker.in ());	  
#endif

	  orb->run ();
        } // end else
    }
  catch (Name_Helper_Exception & ex)
    {
      ACE_DEBUG ((LM_ERROR, "Name_Helper_Exception: %s", ex.what ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server: exception caught:");
      return -1;
    }
  
  return 0;
}
