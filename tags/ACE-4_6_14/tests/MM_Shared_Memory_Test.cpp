// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Shared_Memory_MM_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of ACE_Shared_Memory_MM. The test forks
//     two processes or spawns two threads (depending upon the
//     platform) and then executes client and server allowing them to
//     exchange data using shared memory. No user input is required as
//     far as command line arguments are concerned.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Shared_Memory_MM.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"

ACE_RCSID(tests, MM_Shared_Memory_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

const int SHMSZ = 27;
static TCHAR shm_key[] = ACE_TEMP_FILE_NAME ACE_TEXT ("XXXXXX");

static void *
client (void * = 0)
{
  ACE_OS::sleep (ACE_DEFAULT_TIMEOUT);
  char *t = ACE_ALPHABET;
  ACE_Shared_Memory_MM shm_client;

  if (shm_client.open (shm_key) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n%a"),
                ASYS_TEXT ("ACE_Shared_Memory_MM::open() failed in client"),
                1));
    /* NOTREACHED */

  char *shm = (char *) shm_client.malloc ();

  if (shm == 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n%a"),
                ASYS_TEXT ("ACE_Shared_Memory_MM::malloc () failed in client"),
                1));
    /* NOTREACHED */

  for (char *s = shm; *s != '\0'; s++)
    {
      ACE_ASSERT (*t == s[0]);
      t++;
    }

  *shm = '*';

  return 0;
}

static void *
server (void * = 0)
{
  ACE_Shared_Memory_MM shm_server;

  if (shm_server.open (shm_key, SHMSZ) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n%a"),
                ASYS_TEXT ("ACE_Shared_Memory_MM::open () failed in server"),
                1));
    /* NOTREACHED */

  char *shm = (char *) shm_server.malloc ();

  if (shm == 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n%a"),
                ASYS_TEXT ("ACE_Shared_Memory_MM::malloc () failed in server"),
                1));
    /* NOTREACHED */

  char *s = shm;

  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    *s++ = *c;

  *s = '\0';

  // Perform a busy wait (ugh).
  while (*shm != '*')
    ACE_OS::sleep (1);

  if (shm_server.remove () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n"),
                ASYS_TEXT ("remove")));

  ACE_OS::unlink (shm_key);
  return 0;
}

static void
spawn (void)
{
#if !defined (ACE_LACKS_FORK)
  switch (ACE_OS::fork (ASYS_TEXT ("child")))
    {
    case -1:
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) %p\n%a"),
                  ASYS_TEXT ("fork failed"),
                  1));
      /* NOTREACHED */
    case 0:
      server ();
      break;
    default:
      client ();
      break;
    }
#elif defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (client),
       (void *) 0,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n%a"),
                ASYS_TEXT ("thread create failed"),
                1));

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (server),
       (void *) 0,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%P|%t) %p\n%a"),
                ASYS_TEXT ("thread create failed"),
               1));
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("only one thread may be run in a process on this platform\n%a"), 1));
#endif /* ACE_HAS_THREADS */
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("MM_Shared_Memory_Test"));

  if (ACE_OS::mktemp (shm_key) == 0
      || (ACE_OS::unlink (shm_key) == -1
          && errno == EPERM))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) %p\n"),
                       shm_key),
                      1);
  spawn ();

  ACE_END_TEST;
  return 0;
}
