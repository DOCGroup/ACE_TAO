#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Manual_Event.h"
#include <string>

static int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &output,
            std::basic_string<ACE_TCHAR> &input,
            Client_Task::SyncMode &sync_mode,
            std::string &server_orb,
            std::string &client_orb)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:o:nm:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        output = get_opts.opt_arg ();
        break;
      case 'k':
        input = get_opts.opt_arg ();
        break;
      case 'n':
        server_orb = "server_orb";
        client_orb = "client_orb";
        break;
      case 'm':
        {
          ACE_TCHAR const *mode = get_opts.opt_arg ();
          if (mode == nullptr)
            return 0;

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - mode set to <%s> for collocated oneway test\n"),
                      mode));
          if (ACE_OS::strcmp (ACE_TEXT ("none"), mode) == 0)
            sync_mode = Client_Task::SyncMode::none;
          else if (ACE_OS::strcmp (ACE_TEXT ("transport"), mode) == 0)
            sync_mode = Client_Task::SyncMode::transport;
          else if (ACE_OS::strcmp (ACE_TEXT ("server"), mode) == 0)
            sync_mode = Client_Task::SyncMode::server;
          else if (ACE_OS::strcmp (ACE_TEXT ("target"), mode) == 0)
            sync_mode = Client_Task::SyncMode::target;
        }
        break;
      case '?':
      default:
        return 0;
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  std::basic_string<ACE_TCHAR> output {ACE_TEXT ("test.ior")};
  std::basic_string<ACE_TCHAR> input {ACE_TEXT ("file://test.ior")};
  Client_Task::SyncMode sync_mode {Client_Task::SyncMode::none};
  std::string server_orb;
  std::string client_orb;
  if (parse_args (argc, argv, output, input, sync_mode, server_orb, client_orb) == -1)
    return -1;

  try
    {
      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var const sorb {
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         server_orb.c_str ())};

      ACE_Manual_Event me;
      Server_Task server_task (output.c_str (),
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
      CORBA::ORB_var const corb {
        CORBA::ORB_init (catc.get_argc (),
                         catc.get_TCHAR_argv (),
                         client_orb.c_str ())};

      Client_Task client_task (input.c_str (),
                               corb.in (),
                               sync_mode,
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
