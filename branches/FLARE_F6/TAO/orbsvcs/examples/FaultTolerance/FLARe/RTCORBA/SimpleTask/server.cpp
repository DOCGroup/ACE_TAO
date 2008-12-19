// $Id$

#include <sstream>
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "check_supported_priorities.cpp"
#include "Worker_i.h"

#include "orbsvcs/orbsvcs/LWFT/AppSideReg.h"
#include "orbsvcs/orbsvcs/LWFT/AppOptions.h"
#include "orbsvcs/orbsvcs/LWFT/StateSyncAgentTask.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManagerC.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Server_Init.h"
#include "orbsvcs/Naming/Naming_Client.h"


std::string ior_output = "test1.ior";
CORBA::Short poa_priority = 20;
CORBA::Short object_priority = -1;
long invocations = 0;
bool kill_me = false;

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:i:b:k"));
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	kill_me = true;
	break;
      case 'i':
	invocations = atoi (get_opts.opt_arg ());
	break;
      case 'b':
        result = ::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                           "%hd",
                           &object_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        break;
      case 'p':
	ior_output = get_opts.opt_arg ();
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <iorfile1> "
                           "-o <iorfile2> "
                           "-a <poa_priority> "
                           "-b <object_priority> "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (poa_priority < 0
      || object_priority < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Valid poa and object priorities must be"
                       " specified.\nSee README file for more info\n"),
                      -1);

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
create_object (RTPortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Worker_i *server_impl,
               CORBA::Short priority,
               const ACE_TCHAR *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (AppOptions::instance ()->app_id ().c_str ());

  if (priority > -1)
    poa->activate_object_with_id_and_priority (id,
					       server_impl,
					       priority);
  else
    poa->activate_object_with_id (id,
				  server_impl);


  CORBA::Object_var server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in ());

  //  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

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
    agent_ (agent)
{
}

int
Task::svc (void)
{
  try
    {
      // RTORB.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ());
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      // RootPOA.
      object =
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
      poa_policy_list.length (2);
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              poa_priority);
      poa_policy_list[1] =
	root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list);

      RTPortableServer::POA_var rt_poa =
        RTPortableServer::POA::_narrow (child_poa.in ());
      if (check_for_nil (rt_poa.in (), "RTPOA") == -1)
        return -1;

      // Servant.
      Worker_i server_impl (this->orb_.in (),
			    child_poa.in (),
			    AppOptions::instance ()->app_id (),
			    agent_,
			    invocations);

      // Create object 1 (it will inherit POA's priority).
      int result;
      //      ACE_DEBUG ((LM_DEBUG, "\nActivated object one as "));
      result = create_object (rt_poa.in (), this->orb_.in (), &server_impl,
                              object_priority, ior_output.c_str ());

      CORBA::Object_var obj = rt_poa->servant_to_reference (&server_impl);

      if (result == -1)
        return -1;

      // Activate POA manager.
      poa_manager->activate ();

      //      std::cout << "register apps." << std::endl;

      rm_->register_application (
			 AppOptions::instance ()->app_id ().c_str (), 
			 AppOptions::instance ()->load (),
			 AppOptions::instance ()->host_id ().c_str (),
			 AppOptions::instance ()->process_id ().c_str (),
			 AppOptions::instance ()->role (),
			 obj.in ());

      std::cout << "Worker orb->run ()" << std::endl;

      if (kill_me)
	{
	  std::ostringstream os;
	  os << "kill-" << ACE_OS::getpid ();
	  std::ofstream file (os.str ().c_str ());
	  file << 0;
	  file.close ();
	}

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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      AppOptions::instance ()->parse_args (argc, argv);

#if 0
      std::cout << "Parameters:" << std::endl;
      for (int i=0;i < argc;++i)
	std::cout << argv[i] << std::endl;
#endif
      AppSideReg proc_reg (orb.in ());

      int result = proc_reg.activate ();

      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppSideReg::activate () returned %d\n",
                             result),
                            -1);
        }

      //      std::cout << "Before StateSyncAgent." << std::endl;

      StateSynchronizationAgent_i* ssa_servant =
	      new StateSynchronizationAgent_i (
	        orb.in (),
		AppOptions::instance ()->host_id (),
		AppOptions::instance ()->process_id (),
		!(AppOptions::instance ()->use_dds ()));

      PortableServer::ServantBase_var owner_transfer (ssa_servant);
      // ACE_DEBUG ((LM_TRACE, ACE_TEXT ("StateSynchronizationAgent created.\n")));

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

      //      std::cout << "Before resolving NS." << std::endl;

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
	  
	  //	  ACE_DEBUG ((LM_TRACE,
	  //		      "ServerTask: RM resolved from Naming Service\n"));
	}

      rm->register_state_synchronization_agent (
            AppOptions::instance ()->host_id ().c_str (),
	    AppOptions::instance ()->process_id ().c_str (),
	    sync_agent_thread.agent_ref ());

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in (),
		 rm,
		 sync_agent_thread.agent_ref ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      //      std::cout << "Start Task" << std::endl;

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
