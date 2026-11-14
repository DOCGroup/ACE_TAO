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
            std::string &server_orb,
            std::string &client_orb)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:o:n"));
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
        // named_orbs = 1;
        server_orb = "server_orb";
        client_orb = "client_orb";
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
  std::string server_orb;
  std::string client_orb;
  if (parse_args (argc, argv, output, input, server_orb, client_orb) == -1)
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
      if (server_orb != client_orb)
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
