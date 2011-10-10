//$Id$

#include "Server_Task.h"
#include "Client_Task.h"
#include "Echo_Collocated_ORBInitializer.h"
#include "interceptors.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"

const ACE_TCHAR *output = ACE_TEXT("test.ior");
const ACE_TCHAR *input = ACE_TEXT("file://test.ior");
ACE_CString server_orb;
ACE_CString client_orb;
int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        output = get_opts.opt_arg ();
        break;
      case 'k':
        input = get_opts.opt_arg ();
        break;
      case '?':
      default:
        // This is a hack but that is okay!
        return 0;
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc,
                  argv) == -1)
    return -1;

  server_orb.set ("server_orb");
  client_orb.set ("client_orb");

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

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
                         server_orb.c_str ());

      ACE_Manual_Event me;
      Server_Task server_task (output,
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
                         client_orb.c_str ());

      Client_Task client_task (input,
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
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions..
    }
  return 0;
}
