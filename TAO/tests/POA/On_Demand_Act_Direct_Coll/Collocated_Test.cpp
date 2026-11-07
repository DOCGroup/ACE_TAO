#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"

struct Options
{
  ACE_TCHAR const *output {ACE_TEXT("test.ior")};
  ACE_TCHAR const *input {ACE_TEXT("file://test.ior")};
  CORBA::Boolean exception_occurred {};
  ACE_CString server_orb;
  ACE_CString client_orb;
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o:td:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.output = get_opts.opt_arg ();
        break;
      case 'k':
        options.input = get_opts.opt_arg ();
        break;
      case 't':
        // no exception expected
        options.exception_occurred = false;
        break;
      case 'd':
        // exception expected
        options.exception_occurred = true;
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  if (parse_args (argc, argv, options) == -1)
    return -1;

  try
    {
      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var const sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         options.server_orb.c_str ());

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
      CORBA::ORB_var const corb =
        CORBA::ORB_init (catc.get_argc (),
                         catc.get_TCHAR_argv (),
                         options.client_orb.c_str ());

      Client_Task client_task (options.input,
                               corb.in (),
                               ACE_Thread_Manager::instance (),
                               options.exception_occurred);

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
    }
  catch (CORBA::Exception const &)
    {
      // Ignore exceptions.
    }
  return 0;
}
