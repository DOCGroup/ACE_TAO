// $Id$

#include "ace/Synch.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "Cubit_Client.h"
#include "Cubit_Server.h"

ACE_RCSID(IDL_Cubit, collocation_test, "$Id$")

static const char *server_cmd = 0;

void *
svr_worker (void *arg)
{
  Cubit_Server cubit_server;
  ACE_Barrier *barrier = (ACE_Barrier *) arg;

  char cmd_line[1024];
  ACE_OS::strcpy (cmd_line, "server ");
  if (server_cmd != 0)
    ACE_OS::strcat (cmd_line, server_cmd);
  ACE_OS::strcat (cmd_line, " -f theior");
  ACE_ARGV args (cmd_line);

  ACE_TRY_NEW_ENV
    {
      if (cubit_server.init (args.argc (),
                             args.argv (),
                             ACE_TRY_ENV) == -1)
        return (void *) 1;
      else
        {
          barrier->wait ();
          cubit_server.run (ACE_TRY_ENV);
        }
        ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return (void *) 1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return (void *) 1;
    }
  ACE_ENDTRY;
  return 0;
}


int
main (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "s:c:");
  int c = -1;
  const char *client_cmd = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        client_cmd = get_opts.optarg;
        ACE_DEBUG ((LM_DEBUG, "Client argument: %s\n", client_cmd));
        break;
      case 's':
        server_cmd = get_opts.optarg;
        ACE_DEBUG ((LM_DEBUG, "Server argument: %s\n", server_cmd));
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: collocation_test -s \"server opts\" -c \"client opts\""),
                          -1);
      }

  char cmd_line[1024];
  ACE_OS::strcpy (cmd_line, "client ");
  if (client_cmd != 0)
    ACE_OS::strcat (cmd_line, client_cmd);
  ACE_OS::strcat (cmd_line, " -f theior");
  ACE_ARGV args (cmd_line);

  ACE_Barrier barrier (2);

  int retv = 1;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Cubit: Collocation test \n\n"));

  ACE_Thread_Manager tm;
  tm.spawn (ACE_reinterpret_cast (ACE_THR_FUNC, &svr_worker),
            &barrier);
  barrier.wait ();
  ACE_OS::sleep (1);

  Cubit_Client cubit_client (1);
  // Make sure the server shuts itself down afterward.

  if (cubit_client.init (args.argc (), args.argv ()) == -1)
    return 1;
  else
    retv = cubit_client.run ();

  tm.wait ();
  return retv;
}
