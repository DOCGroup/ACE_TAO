// $Id$

#include "ace/Synch.h"
#include "Cubit_Client.h"
#include "Cubit_Server.h"

#define THE_IOR_FILE "theior"

ACE_RCSID(IDL_Cubit, collocation_test, "$Id$")

#define ACE_THREAD_MANAGER ACE_Thread_Manager::instance ()

void *
svr_worker (void *arg)
{
  Cubit_Server cubit_server;
  ACE_Barrier *barrier = (ACE_Barrier *) arg;
  char *fake[] = {"server", "-o", THE_IOR_FILE };

  ACE_TRY_NEW_ENV
    {
      if (cubit_server.init (3, fake, ACE_TRY_ENV) == -1)
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
      ACE_UNUSED_ARG (sysex);
      ACE_TRY_ENV.print_exception ("System Exception");
      return (void *) 1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      ACE_TRY_ENV.print_exception ("User Exception");
      return (void *) 1;
    }
  ACE_ENDTRY;
  return 0;
}


int
main (int argc, char **argv)
{
  int test_collocation = 0;

  if (ACE_OS::strstr (argv[0], "noncollocation_test") == 0)
    // Check if we are running the test with collocation optimization enabled.
    {
      test_collocation = 1;
      ACE_DEBUG ((LM_DEBUG, "%s: Using collocation optimization\n", argv[0]));
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s: Not using collocation optimization\n", argv[0]));

  Cubit_Client cubit_client (test_collocation, 1);
  // We want to test collocation, so create
  // cubit_client with parameter 1 set.  Make sure
  // the server shuts itself down afterward.

  CORBA::Environment env;
  ACE_Barrier barrier (2);

  int retv = 1;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Cubit: Collocation test \n\n"));

  ACE_THREAD_MANAGER->spawn (ACE_reinterpret_cast (ACE_THR_FUNC, &svr_worker),
                             &barrier);
  barrier.wait ();
  ACE_OS::sleep (1);

  if (cubit_client.init (argc, argv, THE_IOR_FILE) == -1)
    return 1;
  else
    retv = cubit_client.run ();

  ACE_THREAD_MANAGER->wait ();
  return retv;
}
