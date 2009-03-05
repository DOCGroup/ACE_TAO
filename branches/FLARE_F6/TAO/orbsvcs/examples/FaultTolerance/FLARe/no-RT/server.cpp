// $Id$

#include <sstream>
#include <memory>

#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Barrier.h"

#include "tao/ORBInitializer_Registry.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "orbsvcs/orbsvcs/LWFT/AppOptions.h"
#include "orbsvcs/orbsvcs/LWFT/AppSideMonitor_Thread.h"
#include "orbsvcs/orbsvcs/LWFT/StateSyncAgentTask.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Server_Init.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Client_Init.h"

#include "test_i.h"
#include "ServerTask.h"

int
parse_args (int argc, char *argv[], ServerOptions & options)
{
  ACE_Get_Opt get_opts (argc, argv, "b:hf:l:s:a:c:nr:");
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'b':
          options.bands_file = get_opts.opt_arg ();
          break;
        case 'f':
          options.lanes_file = get_opts.opt_arg ();
          break;
        case 'l':
          options.number_of_lanes =
            ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 's':
          options.static_threads =
            ACE_OS::atoi (get_opts.opt_arg ());
          break;   
        case 'a':
          options.stop =
            ACE_OS::atoi (get_opts.opt_arg());
          break;
        case 'c':
	  options.number_of_servants =
	    ACE_OS::atoi (get_opts.opt_arg ());
	  break;
        case 'n':
          options.use_ns = true;
          break;
        case 'r':
          options.rm_ior_file = get_opts.opt_arg ();
          break;

        case 'h':
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s\n"
                             "\t-a stop"
                             "\t-b <bands file> (defaults to %s)\n"
			     "\t-c <number of servants> (defaults to %d)\n"
                             "\t-h <help: shows options menu>\n"
                             "\t-f <lanes file> (defaults to %s)\n"
                             "\t-l <number of lanes> (defaults to %d)\n"
                             "\t-s <static threads> (defaults to %d)\n"
                             "\t-n use naming service to resolve ReplicationManager"
                             "\t-r <ior file for ReplicationManager> (defaults to %d)"
                             "\n",
                             argv [0],
                             options.bands_file,
			     options.number_of_servants,
                             options.lanes_file,
                             options.number_of_lanes,
                             options.static_threads,
                             options.rm_ior_file),
                            -1);
	        break;
        }
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

      if (CORBA::is_nil (orb.in ()))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     ACE_TEXT ("ORB could not be initialized.\n")),
			    -1);
	}

      ACE_DEBUG ((LM_TRACE, ACE_TEXT ("ORB initialized.\n")));

      AppOptions::instance ()->parse_args (argc, argv);
      AppOptions::instance ()->orb (orb.in ());

      int result = AppSideMonitor_Thread::instance ()->activate ();
      
      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppMonitor_Thread::activate () returned %d\n",
                             result),
                            -1);
        }
   
      ServerOptions server_options;

      result = parse_args (argc, argv, server_options);

      if (result != 0)
        {
          return result;
        }

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

#if (SCHEDULING_TYPE_IN_SVC_CONF_HAS_BEEN_SET_CORRECTLY)
      if (!check_supported_priorities (orb.in ()))
	{
	  return -1;
	}
#endif
      
      // Create task.
      ServerTask task (server_options,
                       orb.in (),
                       StateSyncAgentTask::instance ()->agent_ref ());

      // Activate task.
      result = task.activate (THR_NEW_LWP | THR_JOINABLE);

      if (result != 0)
	      {
	        ACE_ERROR_RETURN ((LM_ERROR,
	                           "ServerTask::activate () returned %d\n",
	                           result),
	                          -1);
	      }

      // Wait for task to exit.
      result = ACE_Thread_Manager::instance ()->wait ();
      ACE_ASSERT (result != -1);
    }
#ifdef FLARE_USES_DDS
  catch (const DDSFailure & failure)
    {
      std::cerr << "DDS communication failed: " << failure.description () 
		<< std::endl;
    }
#endif /* FLARE_USES_DDS */
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server: exception caught:");
      return -1;
    }

  return 0;
}
