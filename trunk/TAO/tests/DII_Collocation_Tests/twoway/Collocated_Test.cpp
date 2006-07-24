//$Id$

// This include is needed for static builds to initialize DII_Arguments_Converter
// service object.
//#include "tao/DynamicInterface/DII_Arguments_Converter_Impl.h"

#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"

const char *output = "server.ior";
const char *input = "file://server.ior";
const char *simple_test_output = "simple_test.ior";
const char *simple_test_input = "file://simple_test.ior";
// static int named_orbs = 0;
ACE_CString server_orb;
ACE_CString client_orb;

CORBA::Boolean debug = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:o:nd");
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
      case 'n':
        // named_orbs = 1;
        server_orb.set ("server_orb");
        client_orb.set ("client_orb");
        break;
      case 'd':
        debug = 1;
        break;
      case '?':
      default:
        // This is a hack but that is okay!
        return 0;
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  if (parse_args (argc,
                  argv) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         server_orb.c_str ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Manual_Event me;
      Server_Task server_task (output,
                               simple_test_output,
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
                         client_orb.c_str ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Client_Task client_task (input,
                               simple_test_input,
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
      sorb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (server_orb != client_orb)
        {
          corb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      CORBA::ULong errors = client_task.error_count () + server_task.error_count ();
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
  ACE_CATCHANY
    {
      // Ignore exceptions..
    }
  ACE_ENDTRY;
  return 0;
}
