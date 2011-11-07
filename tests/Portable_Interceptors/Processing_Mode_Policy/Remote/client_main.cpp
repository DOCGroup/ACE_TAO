//$Id$
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"
#include "Echo_Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "client_interceptor.h"

const ACE_TCHAR *input = ACE_TEXT("file://test.ior");

ACE_TString proc_mode_str;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:k:"));
  int c;

  proc_mode_str.set (ACE_TEXT("LOCAL_AND_REMOTE"));

  while ((c = get_opts ()) != -1)
  {
    switch (c)
      {
      case 'p':
        proc_mode_str.set (get_opts.opt_arg ());
        break;
      case 'k':
        input = get_opts.opt_arg ();
        break;
      case '?':
      default:
       return 0;
      }
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

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      // Set up our default expectations.
      CORBA::ULong expected_interceptor_check = 12;

      PortableInterceptor::ProcessingMode proc_mode =
                                   PortableInterceptor::LOCAL_AND_REMOTE;

      if (proc_mode_str == ACE_TEXT("LOCAL_ONLY"))
        {
          proc_mode = PortableInterceptor::LOCAL_ONLY;
          ACE_DEBUG((LM_DEBUG,
                     "Using LOCAL_ONLY for client-side ProcessingMode.\n"));

          // Since all of the calls are remote, setting the client-side
          // ProcessingMode to LOCAL_ONLY will cause the client request
          // interceptor to not fire.  Thus, we expect 0 calls on the
          // client interceptor.
          expected_interceptor_check = 0;
        }
      else if (proc_mode_str == ACE_TEXT("REMOTE_ONLY"))
        {
          proc_mode = PortableInterceptor::REMOTE_ONLY;
          ACE_DEBUG((LM_DEBUG,
                     "Using REMOTE_ONLY for client-side ProcessingMode.\n"));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,
                     "Using LOCAL_AND_REMOTE for client-side ProcessingMode.\n"));
        }

      ACE_NEW_RETURN (temp_initializer,
                      Echo_Client_ORBInitializer (proc_mode),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      ACE_Argv_Type_Converter atc (argc, argv);
      CORBA::ORB_var orb =
        CORBA::ORB_init (atc.get_argc (),
                         atc.get_TCHAR_argv (),
                         "");

      Client_Task client_task (input,
                               orb.in (),
                               ACE_Thread_Manager::instance ());

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();

      CORBA::ULong number_called =
        Echo_Client_Request_Interceptor::client_interceptor_check_;

      if (number_called != expected_interceptor_check)
        {
          ACE_ERROR ((LM_ERROR,
                    "(%P|%t) ERROR:Client-Side Interceptors not called "
                    "properly, called %d times, expected %d.\n",
                    number_called, expected_interceptor_check));
          return 1;
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Some exception was caught in main().\n"));
      return 1;
    }
  return 0;
}
