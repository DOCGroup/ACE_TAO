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

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

const int SHMSZ = 27;
static TCHAR shm_key[] = ACE_TEMP_FILE_NAME __TEXT ("XXXXXX");

static void *
client (void *)
{
  ACE_OS::sleep (3);
  char *t = ACE_ALPHABET;
  ACE_Shared_Memory_MM shm_client (shm_key);
  char *shm = (char *) shm_client.malloc ();

  for (char *s = shm; *s != '\0'; s++)
    {
      ACE_ASSERT (*t == s[0]);
      t++;
    }
  *shm = '*';
#if !defined (ACE_WIN32)
  ACE_OS::exit (0);
#endif
  return 0;
}

static void *
server (void *)
{
  ACE_Shared_Memory_MM shm_server (shm_key, SHMSZ);

  char *shm = (char *) shm_server.malloc ();

  if (! shm)
  {
   ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n%a"), ASYS_TEXT ("ACE_Shared_Memory_MM::malloc () failed in server")));
   exit (-1);
  }

  char *s = shm;
  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    *s++ = *c;

  *s = '\0';

  // Perform a busy wait (ugh)
  while (*shm != '*')
    ACE_OS::sleep (1);

  if (shm_server.remove () == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n"), ASYS_TEXT ("remove")));

  ACE_OS::unlink (shm_key);
  return 0;
}

static void
spawn (void)
{
#if !defined (ACE_LACKS_FORK)
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n%a"), ASYS_TEXT ("fork failed")));
      exit (-1);
    case 0:
      client (0);
    default:
      server (0);
    }
#elif defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (client),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n%a"), ASYS_TEXT ("thread create failed")));

  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (server),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n%a"), ASYS_TEXT ("thread create failed")));
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_ERROR ((LM_ERROR, ASYS_TEXT ("only one thread may be run in a process on this platform\n%a"), 1));
#endif /* ACE_HAS_THREADS */
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("MM_Shared_Memory_Test"));

  if (ACE_OS::mktemp (shm_key) == 0 || (ACE_OS::unlink (shm_key) == -1 && errno == EPERM))
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("%p\n"), shm_key), 1);

  spawn ();

  ACE_END_TEST;
  return 0;
}

