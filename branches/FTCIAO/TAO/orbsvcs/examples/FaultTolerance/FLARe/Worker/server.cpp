// $Id$

#include <sstream>
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "Worker_i.h"

#include "orbsvcs/orbsvcs/LWFT/AppOptions.h"
#include "orbsvcs/orbsvcs/LWFT/AppSideMonitor_Thread.h"
#include "orbsvcs/orbsvcs/LWFT/StateSyncAgentTask.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManagerC.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Server_Init.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Client_Init.h"
#include "orbsvcs/Naming/Naming_Client.h"

std::string ior_output = "test1.ior";
long invocations = 0;

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
	invocations = atoi (get_opts.opt_arg ());
	break;
      case 'p':
	ior_output = get_opts.opt_arg ();
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <iorfile1> "
                           "-i <number of invocation before failure> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%C> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Worker_i *server_impl,
               const ACE_TCHAR *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (AppOptions::instance ()->app_id ().c_str ());

  poa->activate_object_with_id (id,
                                server_impl);

  CORBA::Object_var server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in ());

  // Print ior to the file.
  if (filename != 0)
    {
      FILE *output_file= ACE_OS::fopen (filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
	ReplicationManager_ptr rm,
	StateSynchronizationAgent_ptr agent);

  int svc (void);

  CORBA::ORB_var orb_;
  
  ReplicationManager_var rm_;
  StateSynchronizationAgent_ptr agent_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
	    ReplicationManager_ptr rm,
	    StateSynchronizationAgent_ptr agent)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb)),
    rm_ (ReplicationManager::_duplicate (rm)),
    agent_ (StateSynchronizationAgent::_duplicate (agent))
{
}

int
Task::svc (void)
{
  try
    {
      // RootPOA.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Create child POA with SERVER_DECLARED PriorityModelPolicy,
      // and MULTIPLE_ID id uniqueness policy (so we can use one
      // servant to create several objects).
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);

      poa_policy_list[0] =
	root_poa->create_id_assignment_policy (PortableServer::USER_ID);
      
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list);

      // Servant.
      Worker_i server_impl (this->orb_.in (),
			    child_poa.in (),
			    AppOptions::instance ()->app_id (),
			    agent_,
			    invocations);

      int result = create_object (child_poa.in (), 
                                  orb_.in (), 
                                  &server_impl,
                                  ior_output.c_str ());

      CORBA::Object_var obj = child_poa->servant_to_reference (&server_impl);

      if (result == -1)
        return -1;

      // Activate POA manager.
      poa_manager->activate ();

      rm_->register_application (
			 AppOptions::instance ()->app_id ().c_str (), 
			 AppOptions::instance ()->load (),
			 AppOptions::instance ()->host_id ().c_str (),
			 AppOptions::instance ()->process_id ().c_str (),
			 AppOptions::instance ()->role (),
			 obj.in ());

      ReplicatedApplication_var app = ReplicatedApplication::_narrow (obj.in ());

      agent_->register_application (AppOptions::instance ()->app_id ().c_str (),
                                    app.in ());

      // Start ORB event loop.
      this->orb_->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in Server_Declared test server:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // ORB.
      LWFT_Client_Init client_initializer;
      CORBA::ORB_var orb =
        client_initializer.init (argc, argv);

      AppOptions::instance ()->parse_args (argc, argv);
      AppOptions::instance ()->orb (CORBA::ORB::_duplicate (orb.in ()));

      int result = AppSideMonitor_Thread::instance ()->activate ();

      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppSideReg::activate () returned %d\n",
                             result),
                            -1);
        }

      // create task for state synchronization agent
      result = StateSyncAgentTask::instance ()->activate ();
      
      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "StateSyncAgentTask::activate () "
                             "returned %d, errno = %d\n", 
                             result, 
                             errno),
                            -1);
        }

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
	}

      rm->register_state_synchronization_agent (
            AppOptions::instance ()->host_id ().c_str (),
	    AppOptions::instance ()->process_id ().c_str (),
	    StateSyncAgentTask::instance ()->agent_ref ());

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in (),
		 rm.in (),
		 StateSyncAgentTask::instance ()->agent_ref ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result = task.activate (flags);

      // Wait for task to exit.
      result = thread_manager.wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught: ");
      return -1;
    }

  return 0;
}
