// $Id$

#include "TestC.h"
#include "TestImpl.h"
#include "Client_Task.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "tao/ORB_Core.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "Server_Task.h"

//////////////////////////////////////////////////////////////////////////

ACE_TString collocated_ior_file(ACE_TEXT("collocated_server.ior"));
ACE_TString server_ior_file(ACE_TEXT("server.ior"));

//////////////////////////////////////////////////////////////////////////

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        server_ior_file = get_opts.opt_arg ();
        break;
      case 'o':
        collocated_ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
          "usage:  %s "
          "-s <server_iorfile>"
          "-o <collocated_iorfile>"
          "\n",
          argv [0]),
          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int failed = 0;

  ACE_CString server_orb;
  server_orb.set ("server_orb");
  ACE_CString client_orb;
  client_orb.set ("client_orb");

  try
    {
      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv (),
                         server_orb.c_str ());

      ACE_Argv_Type_Converter catc (argc, argv);
      CORBA::ORB_var corb =
        CORBA::ORB_init (catc.get_argc (),
                         catc.get_TCHAR_argv (),
                         client_orb.c_str ());

      {
        ACE_Manual_Event me;

        Server_Task server_task (collocated_ior_file.c_str(),
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

        Client_Task client_task (corb.in (),
          ACE_Thread_Manager::instance (),
          collocated_ior_file, server_ior_file);

        if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
          1,
          1) == -1)
          {
            ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
          }

        ACE_Thread_Manager::instance ()->wait ();
      }

      sorb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  if (failed)
    {
      ACE_DEBUG ((LM_ERROR, "Test failed.\n") );
    }

  return 0;
}
