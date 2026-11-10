#include "TestImpl.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Argv_Type_Converter.h"
#include "tao/IORTable/IORTable.h"
#include "Server_Task.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("server.ior") };
  try
    {
      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var sorb (
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv ()));

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;
      {
        ACE_Manual_Event me;

        Server_Task server_task (ACE_TString (ior_output_file),
                                 sorb.in (),
                                 me,
                                 ACE_Thread_Manager::instance ());

        if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 1, 1) == -1)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error activating server task\n")));
          }

        // Wait for the server thread to do some processing
        me.wait ();

        ACE_Thread_Manager::instance ()->wait ();
      }

      sorb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
