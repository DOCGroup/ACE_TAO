#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Manual_Event.h"
#include <string>

struct Test_Options
{
  ACE_TString output {ACE_TEXT ("server.ior")};
  ACE_TString input {ACE_TEXT ("file://server.ior")};
  ACE_TString simple_test_output {ACE_TEXT ("simple_test.ior")};
  ACE_TString simple_test_input {ACE_TEXT ("file://simple_test.ior")};
  std::string server_orb;
  std::string client_orb;
};

bool debug {};

void
parse_args (int argc, ACE_TCHAR *argv[], Test_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o:nd"));
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
      case 'n':
        // named_orbs = 1;
        options.server_orb = "server_orb";
        options.client_orb = "client_orb";
        break;
      case 'd':
        debug = true;
        break;
      case '?':
      default:
        // Leave arguments not handled here for ORB_init.
        return;
      }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Test_Options options {};
  parse_args (argc, argv, options);

  try
    {
      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var const sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         options.server_orb.c_str ());

      ACE_Manual_Event me;
      Server_Task server_task (options.output.c_str (),
                               options.simple_test_output.c_str (),
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
      CORBA::ORB_var const corb =
        CORBA::ORB_init (catc.get_argc (),
                         catc.get_TCHAR_argv (),
                         options.client_orb.c_str ());

      Client_Task client_task (options.input.c_str (),
                               options.simple_test_input.c_str (),
                               corb.in (),
                               ACE_Thread_Manager::instance ());

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      // Wait for the client and server to finish
      ACE_Thread_Manager::instance ()->wait ();

      // Now that all threads have completed we can destroy the ORB
      sorb->destroy ();
      if (options.server_orb != options.client_orb)
        {
          corb->destroy ();
        }

      CORBA::ULong const errors = client_task.error_count () + server_task.error_count ();
      if (errors == 0)
        {
          ACE_DEBUG((LM_DEBUG, "(%P|%t) test passed\n"));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG, "(%P|%t) test failed - error_count=%u\n", errors));
          return 1;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("DII collocation test failed");
      return 1;
    }
  return 0;
}
