// $Id$

#include "Cubit_Client.h"
#include "Cubit_Server.h"

#include "tao/Strategies/advanced_resource.h"

#include "ace/Barrier.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/Thread_Manager.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(IDL_Cubit, collocation_test, "$Id$")

#define  THE_IOR "theior"

struct Barriers
{
  Barriers (unsigned int init)
    : server_init_ (init),
      client_fini_ (init)
  {}

  ACE_Barrier server_init_;
  ACE_Barrier client_fini_;
};

static const char *server_cmd = 0;

void *
svr_worker (void *arg)
{
  Cubit_Server cubit_server;
  Barriers *thread_barrier = (Barriers *) arg;

  char cmd_line[BUFSIZ];
  ACE_OS::strcpy (cmd_line, "server ");
  if (server_cmd != 0)
    ACE_OS::strcat (cmd_line, server_cmd);
  ACE_OS::strcat (cmd_line, " -f " THE_IOR);
  ACE_ARGV args (cmd_line);

  try
    {
      int result = cubit_server.init (args.argc (),
                                      args.argv ());

      if (result == -1)
        return (void *) 1;

      thread_barrier->server_init_.wait ();
      cubit_server.run ();

      thread_barrier->client_fini_.wait ();
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return (void *) 1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return (void *) 1;
    }
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
        client_cmd = get_opts.opt_arg ();
        ACE_DEBUG ((LM_DEBUG, "Client argument: %s\n", client_cmd));
        break;
      case 's':
        server_cmd = get_opts.opt_arg ();
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
  ACE_OS::strcat (cmd_line, " -f " THE_IOR);
  ACE_ARGV args (cmd_line);

  Barriers thread_barrier (2);

  int retv = 1;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Cubit: Collocation test \n\n"));

  ACE_Thread_Manager tm;
  tm.spawn (reinterpret_cast<ACE_THR_FUNC> (&svr_worker),
            &thread_barrier);
  thread_barrier.server_init_.wait ();
  ACE_OS::sleep (1);

  Cubit_Client cubit_client (1);
  // Make sure the server shuts itself down afterward.

  if (cubit_client.init (args.argc (), args.argv ()) == -1)
    return 1;
  else
    retv = cubit_client.run ();

  thread_barrier.client_fini_.wait ();
  tm.wait ();

  ACE_OS::unlink (THE_IOR);
  return retv;
}
