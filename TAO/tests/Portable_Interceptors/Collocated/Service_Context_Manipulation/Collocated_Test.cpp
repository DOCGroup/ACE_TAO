//$Id$

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

const ACE_TCHAR *output = ACE_TEXT ("test.ior");
const ACE_TCHAR *input = ACE_TEXT ("file://test.ior");
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  server_orb.set ("server_orb");
  client_orb.set ("client_orb");

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer;

      ACE_NEW_RETURN (temp_initializer,
                      Collocated_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var sorb =
        CORBA::ORB_init (argc, argv, server_orb.c_str ());

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

      CORBA::ORB_var corb =
        CORBA::ORB_init (argc, argv, client_orb.c_str ());

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
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions..
    }

  return 0;
}
