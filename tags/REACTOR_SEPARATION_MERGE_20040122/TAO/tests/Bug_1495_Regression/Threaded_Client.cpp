/**
 * $Id$
 *
 * @file Threaded_Client.cpp
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * This program spawns two threads:
 *    1.)  A "server" thread using Server_Task that acts as a server meant to
 *         recieve forwarded requests.
 *    2.)  A "client" thread using Client_Task that acts as a client that sends
 *         a get_thread_id request that is forwarded by a remote server to
 *         the server in thread (1).
 *
 *
 * The test passes if the thread id of the thread that services the get_thread_id
 * request is the same as the thread that makes the request.
 *
 */

#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Manual_Event.h"

const char *ior_input_file = "file://test.ior";
const char *ior_output_file = "thr_server.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'i':
          ior_input_file = get_opts.opt_arg ();
          break;
        case 'o':
          ior_output_file = get_opts.opt_arg ();
          break;
        case '?':
        default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "usage:  %s "
                               "-i alternate_remote_ior "
                               "-o alternate_local_ior "
                               "\n",
                               argv [0]),
                              -1);
      }
  return 0;
}


int
main (int argc, char *argv[])
{
  // Parse command line
  if (parse_args (argc, argv) == -1)
    {
      return -1;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_Argv_Type_Converter main_args_s (argc, argv);

      CORBA::ORB_var sorb =
        CORBA::ORB_init (main_args_s.get_argc (),
                         main_args_s.get_TCHAR_argv (),
                         "Server_ORB"
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Manual_Event me;

      Server_Task server_task (ior_output_file,
                               sorb.in (),
                               me,
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_JOINABLE, 1, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating the server task."));
          return -1;
        }

      // Wait for the server task to activate.
      me.wait ();

      ACE_Argv_Type_Converter main_args_c (argc, argv);

      CORBA::ORB_var corb =
        CORBA::ORB_init (main_args_c.get_argc (),
                         main_args_c.get_TCHAR_argv (),
                         "Client_ORB"
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      {
        Client_Task client_task (ior_input_file,
                                 corb.in (),
                                 ACE_Thread_Manager::instance ());

        if (client_task.activate (THR_JOINABLE, 1, 1) == -1)
          {
            ACE_ERROR ((LM_ERROR, "Error activating client thread.\n"));
            return -1;
          }

        ACE_Thread_Manager::instance ()->wait ();
      }

      corb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // ignore exceptions
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "Threaded client ready.\n"));

  return 0;
}
