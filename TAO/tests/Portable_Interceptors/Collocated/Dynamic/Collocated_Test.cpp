#include "Server_Task.h"
#include "Client_Task.h"
#include "Echo_Collocated_ORBInitializer.h"
#include "interceptors.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"

struct Options
{
  ACE_TString output { ACE_TEXT ("test.ior") };
  ACE_TString input { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.output = get_opts.opt_arg ();
        break;
      case 'k':
        options.input = get_opts.opt_arg ();
        break;
      case '?':
      default:
        // Leave ORB options for ORB_init to process.
        return 0;
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  if (parse_args (argc, argv, options) == -1)
    return -1;

  char const server_orb[] = "server_orb";
  char const client_orb[] = "client_orb";

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      // The ORB initializer var assumes ownership of the ACE_NEW allocation.
      ACE_NEW_RETURN (temp_initializer,
                      Echo_Collocated_ORBInitializer,
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
      Server_Task server_task (options.output,
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

      Client_Task client_task (options.input,
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

      CORBA::ULong number_called =
        Echo_Server_Request_Interceptor::server_interceptor_check_;

      if (number_called != 10)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Server Side Interceptors not"
                      " called properly, called %d times, expected 10\n",
                      number_called));
        }

      if (Echo_Client_Request_Interceptor::client_interceptor_check_ != 10)
        ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR:Client Interceptors not called"
                  " properly\n"));

      corb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      // Ignore exceptions..
    }
  return 0;
}
