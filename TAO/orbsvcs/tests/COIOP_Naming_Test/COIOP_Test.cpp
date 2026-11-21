#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Manual_Event.h"
#include "tao/Strategies/advanced_resource.h"
#include "NamingTask.h"
#include <string>

struct Test_Options
{
  std::string server_orb {};
  std::string client_orb {};
  bool expected_success {};
};

static void
parse_args (int argc, ACE_TCHAR *argv[], Test_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("sk:o:n"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        options.expected_success = true;
        break;
      case 'n':
        options.server_orb = "server_orb";
        options.client_orb = "client_orb";
        break;
      case 'k':
      case 'o':
        get_opts.opt_arg ();
        break;
      case '?':
      default:
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
      // Start the Naming Service tasks
      NamingTask name_service ("NamingORBA", argc, argv);
      name_service.activate ();
      // Wait for the Naming Service initialized.
      name_service.waitInit ();

      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var const sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         options.server_orb.c_str ());

      ACE_Manual_Event me;
      Server_Task server_task (name_service.root (),
                               sorb.in (),
                               me,
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error activating server task\n")));
        }

      // Wait for the server thread to do some processing
      me.wait ();

      ACE_Argv_Type_Converter catc (argc, argv);
      CORBA::ORB_var const corb =
        CORBA::ORB_init (catc.get_argc (),
                         catc.get_TCHAR_argv (),
                         options.client_orb.c_str ());

      Client_Task client_task (name_service.root (),
                               ACE_Thread_Manager::instance (),
                               options.expected_success);

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                1,
                                1) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error activating client task\n")));
        }

      client_task.wait ();
      server_task.wait ();
      name_service.end ();

      // Now that all threads have completed we can destroy the ORB
      sorb->destroy ();
      if (options.server_orb != options.client_orb)
        {
          corb->destroy ();
        }
    }
  catch (CORBA::Exception const &)
    {
      // Ignore exceptions..
    }
  return 0;
}
