//$Id$
#include "Server_Task.h"
#include "Client_Task.h"
#include "ace/Get_Opt.h"

const char *output = "test.ior";
const char *input = "file://test.ior";
static int named_orbs = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:o:n");
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
        named_orbs = 1;
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

  ACE_Manual_Event me;
  Server_Task server_task (output,
                           named_orbs,
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

  Client_Task client_task (input,
                           named_orbs,
                           argc,
                           argv,
                           ACE_Thread_Manager::instance ());

  if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                            1,
                            1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
    }

  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}
