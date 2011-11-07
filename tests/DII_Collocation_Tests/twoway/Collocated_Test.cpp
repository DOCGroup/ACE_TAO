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

const ACE_TCHAR *output = ACE_TEXT("server.ior");
const ACE_TCHAR *input = ACE_TEXT("file://server.ior");
const ACE_TCHAR *simple_test_output = ACE_TEXT("simple_test.ior");
const ACE_TCHAR *simple_test_input = ACE_TEXT("file://simple_test.ior");
// static int named_orbs = 0;
ACE_CString server_orb;
ACE_CString client_orb;

CORBA::Boolean debug = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o:nd"));
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
      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         server_orb.c_str ());

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
                         client_orb.c_str ());

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
      sorb->destroy ();
      if (server_orb != client_orb)
        {
          corb->destroy ();
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
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions..
    }
  return 0;
}
