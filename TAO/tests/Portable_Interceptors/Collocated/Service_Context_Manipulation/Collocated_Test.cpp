#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"
#include "test_i.h"
#include "interceptors.h"
#include "Collocated_ORBInitializer.h"

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
                      Collocated_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var sorb =
        CORBA::ORB_init (argc, argv, server_orb);

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

      CORBA::ORB_var corb =
        CORBA::ORB_init (argc, argv, client_orb);

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
    }
  catch (CORBA::Exception const &)
    {
      // Ignore exceptions..
    }

  return 0;
}
