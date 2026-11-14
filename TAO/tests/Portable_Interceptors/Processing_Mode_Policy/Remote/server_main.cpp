#include "Server_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"
#include "Echo_Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "server_interceptor.h"

struct Options
{
  ACE_TString output { ACE_TEXT ("test.ior") };
  ACE_TString proc_mode { ACE_TEXT ("LOCAL_AND_REMOTE") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
      case 'p':
        options.proc_mode = get_opts.opt_arg ();
        break;
      case 'o':
        options.output = get_opts.opt_arg ();
        break;
      case '?':
      default:
        return 0;
      }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  if (parse_args (argc, argv, options) != 0)
    return -1;

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      // Set up our default expectations.
      CORBA::ULong expected_interceptor_check (12);

      PortableInterceptor::ProcessingMode proc_mode (
        PortableInterceptor::LOCAL_AND_REMOTE);

      if (options.proc_mode == ACE_TEXT ("LOCAL_ONLY"))
        {
          proc_mode = PortableInterceptor::LOCAL_ONLY;
          ACE_DEBUG ((LM_DEBUG,
                     "Using LOCAL_ONLY for server-side ProcessingMode.\n"));

          // Since all of the calls are remote, setting the server-side
          // ProcessingMode to LOCAL_ONLY will cause the server request
          // interceptor to not fire.  Thus, we expect 0 calls on the
          // server interceptor.
          expected_interceptor_check = 0;
        }
      else if (options.proc_mode == ACE_TEXT ("REMOTE_ONLY"))
        {
          proc_mode = PortableInterceptor::REMOTE_ONLY;
          ACE_DEBUG ((LM_DEBUG,
                     "Using REMOTE_ONLY for server-side ProcessingMode.\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Using LOCAL_AND_REMOTE for server-side ProcessingMode.\n"));
        }

      ACE_NEW_RETURN (temp_initializer,
                      Echo_Server_ORBInitializer (proc_mode),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_Manual_Event me;
      Server_Task server_task (options.output.c_str (),
                               orb.in (),
                               me,
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();

      CORBA::ULong number_called (
        Echo_Server_Request_Interceptor::server_interceptor_check_);

      if (number_called != expected_interceptor_check)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Server-Side Interceptors not "
                      "called properly, called %d times, expected %d.\n",
                      number_called, expected_interceptor_check));
          return 1;
        }
    }
  catch (CORBA::Exception const &)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Some exception was caught in main().\n"));
      return 1;
    }
  return 0;
}
