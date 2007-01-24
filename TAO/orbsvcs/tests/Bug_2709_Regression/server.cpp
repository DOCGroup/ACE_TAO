//
// $Id$
//

#include "TestImpl.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Argv_Type_Converter.h"
#include "tao/IORTable/IORTable.h"
#include "Server_Task.h"

ACE_RCSID (Hello,
           server,
           "server.cpp,v 1.6 2003/11/01 11:15:11 dhinton Exp")

const char *ior_output_file = "server.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_Argv_Type_Converter satc (argc, argv);
      CORBA::ORB_var sorb =
        CORBA::ORB_init (satc.get_argc (),
                         satc.get_TCHAR_argv ()
                         ACE_ENV_ARG_PARAMETER);

      {
        ACE_Manual_Event me;

        Server_Task server_task (ior_output_file,
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

        ACE_Thread_Manager::instance ()->wait ();
      }

      sorb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
