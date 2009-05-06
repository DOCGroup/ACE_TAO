// $Id$

#include <sstream>
#include "ace/Get_Opt.h"

#include "orbsvcs/orbsvcs/LWFT/AppOptions.h"
#include "orbsvcs/orbsvcs/LWFT/AppSideMonitor_Thread.h"
#include "orbsvcs/orbsvcs/LWFT/StateSyncAgentTask.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManagerC.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Server_Init.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Client_Init.h"
#include "orbsvcs/Naming/Naming_Client.h"

#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tests/RTCORBA/common_args.cpp"
#include "tests/RTCORBA/check_supported_priorities.cpp"
#include "ace/High_Res_Timer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_stdio.h"
#include "Worker_i.h"

static int stop = 0;
std::string ior_output = "test1.ior";
long invocations = 0;

static CORBA::ULong static_threads = 1;
static CORBA::ULong dynamic_threads = 0;
static CORBA::ULong number_of_lanes = 0;
static RTCORBA::Priority default_thread_priority = 0;
static RTCORBA::Priority pool_priority = ACE_INT16_MIN;
static const char *bands_file = "empty-file";
static const char *lanes_file = "empty-file";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv,
                        ACE_TEXT ("b:f:l:n:s:a:p:i:")); 
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        bands_file = get_opts.opt_arg ();
        break;

      case 'f':
        pool_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        lanes_file = get_opts.opt_arg ();
        break;

      case 'n':
        number_of_lanes = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        static_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'a':
        stop = ACE_OS::atoi (get_opts.opt_arg ()); 
        break;

      case 'i':
        invocations = ACE_OS::atoi (get_opts.opt_arg ()); 
        break;

      case 'p':
        ior_output = ACE_OS::atoi (get_opts.opt_arg ()); 
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-b <bands file> (defaults to %s)\n"
                           "\t-f <pool priority> (defaults to %d)\n"
                           "\t-l <lanes file> (defaults to %s)\n"
                           "\t-n <number of lanes> (defaults to %d)\n"
                           "\t-p <ior file> (defaults to %s)\n"
                           "\t-s <static threads> (defaults to %d)\n"
                           "\n",
                           argv [0],
                           bands_file,
                           default_thread_priority,
                           lanes_file,
                           number_of_lanes,
                           ior_output.c_str (),
                           static_threads),
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
write_ior_to_file (const char *ior_file,
                   CORBA::ORB_ptr orb,
                   CORBA::Object_ptr object)
{
  CORBA::String_var ior =
    orb->object_to_string (object);

  FILE *output_file =
    ACE_OS::fopen (ior_file,
                   "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       ior_file),
                      -1);

  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());

  ACE_OS::fclose (output_file);

  return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Worker_i *server_impl,
               const ACE_TCHAR *filename)
{
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (AppOptions::instance ()->app_id ().c_str ());

  poa->activate_object_with_id (id,
                                server_impl);

  CORBA::Object_var server =
    poa->id_to_reference (id.in ());

  CORBA::String_var ior =
    orb->object_to_string (server.in ());

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
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      default_thread_priority =
        // current->the_priority ();
        get_implicit_thread_CORBA_priority (this->orb_.in ());

      int result = 0;
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      CORBA::PolicyList policies;

      CORBA::Boolean allow_borrowing = 0;
      if (number_of_lanes != 0)
        {
          get_auto_priority_lanes_and_bands (number_of_lanes,
                                             rt_orb.in (),
                                             stacksize,
                                             static_threads,
                                             dynamic_threads,
                                             allow_request_buffering,
                                             max_buffered_requests,
                                             max_request_buffer_size,
                                             allow_borrowing,
                                             policies,
                                             1);
        }
      else if (ACE_OS::strcmp (lanes_file, "empty-file") != 0)
        {
          result =
            get_priority_lanes ("server",
                                lanes_file,
                                rt_orb.in (),
                                stacksize,
                                static_threads,
                                dynamic_threads,
                                allow_request_buffering,
                                max_buffered_requests,
                                max_request_buffer_size,
                                allow_borrowing,
                                policies,
                                1);

          if (result != 0)
            return result;

          result =
            get_priority_bands ("server",
                                bands_file,
                                rt_orb.in (),
                                policies,
                                1);

          if (result != 0)
            return result;
        }
      else
        {
          if (pool_priority == ACE_INT16_MIN)
            pool_priority =
              default_thread_priority;

          RTCORBA::ThreadpoolId threadpool_id =
            rt_orb->create_threadpool (stacksize,
                                       static_threads,
                                       dynamic_threads,
                                       pool_priority,
                                       allow_request_buffering,
                                       max_buffered_requests,
                                       max_request_buffer_size);

          policies.length (policies.length () + 1);
          policies[policies.length () - 1] =
            rt_orb->create_threadpool_policy (threadpool_id);

          if (ACE_OS::strcmp (bands_file, "empty-file") != 0)
            {
              result =
                get_priority_bands ("server",
                                    bands_file,
                                    rt_orb.in (),
                                    policies,
                                    1);

              if (result != 0)
                return result;
            }
        }

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              default_thread_priority);

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var poa =
        root_poa->create_POA ("RT POA",
                              poa_manager.in (),
                              policies);

      Worker_i server_impl (this->orb_.in (),
                            root_poa.in (),
                            AppOptions::instance ()->app_id (),
                            agent_,
                            invocations);

      result = create_object (root_poa.in (),
                              orb_.in (),
                              &server_impl,
                              ior_output.c_str ());

      CORBA::Object_var obj = root_poa->servant_to_reference (&server_impl);

      if (result == -1)
        return -1;

      poa_manager->activate ();

      rm_->register_application (
                         AppOptions::instance ()->app_id ().c_str (),
                         AppOptions::instance ()->load (),
                         AppOptions::instance ()->host_id ().c_str (),
                         AppOptions::instance ()->process_id ().c_str (),
                         AppOptions::instance ()->role (),
                         obj.in ());

      ReplicatedApplication_var app =
        ReplicatedApplication::_narrow (obj.in ());

      agent_->register_application (
         AppOptions::instance ()->app_id ().c_str (),
         app.in ());

      this->orb_->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      LWFT_Client_Init client_initializer;
      CORBA::ORB_var orb = client_initializer.init (argc, argv);

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
      
      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in ());

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in (), rm.in (), 
                 StateSyncAgentTask::instance ()->agent_ref ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
