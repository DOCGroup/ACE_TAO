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

  int argc = 3;
  CORBA::String_var argv0 = CORBA::string_dup ("server");
  CORBA::String_var argv1 = CORBA::string_dup ("-o");
  CORBA::String_var argv2 = CORBA::string_dup (THE_IOR_FILE);
  //CORBA::String_var argv3 = CORBA::string_dup ("-ORBCollocationStrategy");
  //CORBA::String_var argv4 = CORBA::string_dup ("direct");
  char* argv[] = { argv0.inout (), argv1.inout (), argv2.inout () };

  ACE_TRY_NEW_ENV
    {
      if (cubit_server.init (argc, argv,
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
  int test_collocation = 0;

  if (ACE_OS::strstr (argv[0], "noncollocation_test") == 0)
    // Check if we are running the test with collocation optimization enabled.
    {
      test_collocation = 1;
      ACE_DEBUG ((LM_DEBUG, "%s: Using collocation optimization\n", argv[0]));
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s: Not using collocation optimization\n", argv[0]));

  ACE_Barrier barrier (2);

  int retv = 1;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Cubit: Collocation test \n\n"));

  ACE_THREAD_MANAGER->spawn (ACE_reinterpret_cast (ACE_THR_FUNC, &svr_worker),
                             &barrier);
  barrier.wait ();
  ACE_OS::sleep (1);

    Cubit_Client cubit_client (test_collocation, 1);
    // We want to test collocation, so create
    // cubit_client with parameter 1 set.  Make sure
    // the server shuts itself down afterward.

    if (cubit_client.init (argc, argv, THE_IOR_FILE) == -1)
      return 1;
    else
      retv = cubit_client.run ();

  ACE_THREAD_MANAGER->wait ();
  return retv;
}
