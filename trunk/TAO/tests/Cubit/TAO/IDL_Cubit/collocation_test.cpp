// $Id$

#include "client_i.h"
#include "server_i.h"

#define ACE_THREAD_MANAGER ACE_Thread_Manager::instance ()

void *
svr_worker (void *arg)
{
  Cubit_Server cubit_server;
  char *fake[] = {"server"};

  TAO_TRY
    {
      if (cubit_server.init (1, fake, TAO_TRY_ENV) == -1)
        return (void *) 1;
      else cubit_server.run (TAO_TRY_ENV);
        TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return (void *) 1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return (void *) 1;
    }
  TAO_ENDTRY;
  return 0;
}


int
main (int argc, char **argv)
{
  Cubit_Client cubit_client;
  CORBA::Environment env;

  int retv = 1;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Cubit: Collocation test \n\n"));

  ACE_THREAD_MANAGER->spawn (svr_worker);
  ACE_OS::sleep (5);

  if (cubit_client.init (argc, argv) == -1)
    return 1;
  else
    retv = cubit_client.run (1);

  ACE_THREAD_MANAGER->wait ();
  return retv;
}
