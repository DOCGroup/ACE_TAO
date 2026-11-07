#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"
#include "Echo_Collocated_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "interceptors.h"

struct Options
{
  ACE_TString output { ACE_TEXT ("test.ior") };
  ACE_TString input { ACE_TEXT ("file://test.ior") };
  ACE_TString server_proc_mode { ACE_TEXT ("LOCAL_AND_REMOTE") };
  ACE_TString client_proc_mode { ACE_TEXT ("LOCAL_AND_REMOTE") };
};

ACE_TCHAR const server_orb[] = ACE_TEXT ("server_orb");
ACE_TCHAR const client_orb[] = ACE_TEXT ("client_orb");

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("s:c:k:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
      {
      case 's':
        options.server_proc_mode = get_opts.opt_arg ();
        break;
      case 'c':
        options.client_proc_mode = get_opts.opt_arg ();
        break;
      case 'o':
        options.output = get_opts.opt_arg ();
        break;
      case 'k':
        options.input = get_opts.opt_arg ();
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
      CORBA::ULong expected_server_interceptor_check (10);
      CORBA::ULong expected_client_interceptor_check (10);

      PortableInterceptor::ProcessingMode client_proc_mode (
        PortableInterceptor::LOCAL_AND_REMOTE);

      if (options.client_proc_mode == ACE_TEXT ("LOCAL_ONLY"))
        {
          client_proc_mode = PortableInterceptor::LOCAL_ONLY;
          ACE_DEBUG ((LM_DEBUG,
                      "Using LOCAL_ONLY for client-side ProcessingMode.\n"));
        }
      else if (options.client_proc_mode == ACE_TEXT ("REMOTE_ONLY"))
        {
          client_proc_mode = PortableInterceptor::REMOTE_ONLY;
          ACE_DEBUG ((LM_DEBUG,
                      "Using REMOTE_ONLY for client-side ProcessingMode.\n"));

          // Since all of the calls are collocated, setting the client-side
          // ProcessingMode to REMOTE_ONLY will cause the client request
          // interceptor to not fire.  Thus, we expect 0 calls on the
          // client interceptor.
          expected_client_interceptor_check = 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Using LOCAL_AND_REMOTE for client-side ProcessingMode.\n"));
        }

      PortableInterceptor::ProcessingMode server_proc_mode (
        PortableInterceptor::LOCAL_AND_REMOTE);

      if (options.server_proc_mode == ACE_TEXT ("LOCAL_ONLY"))
        {
          server_proc_mode = PortableInterceptor::LOCAL_ONLY;
          ACE_DEBUG ((LM_DEBUG,
                     "Using LOCAL_ONLY for server-side ProcessingMode.\n"));
        }
      else if (options.server_proc_mode == ACE_TEXT ("REMOTE_ONLY"))
        {
          server_proc_mode = PortableInterceptor::REMOTE_ONLY;
          ACE_DEBUG ((LM_DEBUG,
                     "Using REMOTE_ONLY for server-side ProcessingMode.\n"));

          // Since all of the calls are collocated, setting the server-side
          // ProcessingMode to REMOTE_ONLY will cause the server request
          // interceptor to not fire.  Thus, we expect 0 calls on the
          // server interceptor.
          expected_server_interceptor_check = 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Using LOCAL_AND_REMOTE for server-side ProcessingMode.\n"));
        }

      ACE_NEW_RETURN (temp_initializer,
                      Echo_Collocated_ORBInitializer (server_proc_mode,
                                                      client_proc_mode),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         server_orb);

      ACE_Manual_Event me;
      Server_Task server_task (options.output.c_str (),
                               sorb.in (),
                               me,
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      // Wait for the server thread to do some processing
      me.wait ();

      ACE_Argv_Type_Converter catc (argc, argv);
      CORBA::ORB_var corb =
        CORBA::ORB_init (catc.get_argc (),
                         catc.get_TCHAR_argv (),
                         client_orb);

      Client_Task client_task (options.input.c_str (),
                               corb.in (),
                               ACE_Thread_Manager::instance ());

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();

      sorb->destroy ();

      bool failed_check {};

      CORBA::ULong number_called (
        Echo_Server_Request_Interceptor::server_interceptor_check_);

      if (number_called != expected_server_interceptor_check)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Server-Side Interceptors not "
                      "called properly, called %d times, expected %d.\n",
                      number_called, expected_server_interceptor_check));
          failed_check = true;
        }

      number_called =
        Echo_Client_Request_Interceptor::client_interceptor_check_;

      if (number_called != expected_client_interceptor_check)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Client-Side Interceptors not called "
                      "properly, called %d times, expected %d.\n",
                      number_called, expected_client_interceptor_check));
          failed_check = true;
        }

      if (failed_check)
        {
          return 1;
        }

      corb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Some exception was caught in main().\n"));
      return 1;
    }
  return 0;
}
