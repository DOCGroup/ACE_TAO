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
//     This is a simple test of <ACE_Shared_Memory_MM>.  The test
//     forks two processes or spawns two threads (depending upon the
//     platform) and then executes child and parent allowing them to
//     exchange data using shared memory. No user input is required as
//     far as command line arguments are concerned.
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu> 
//    and Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Shared_Memory_MM.h"
#include "ace/Synch.h"
#include "ace/Thread_Manager.h"

ACE_RCSID(tests, MM_Shared_Memory_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

const int SHMSZ = 27;
static TCHAR shm_key[] = ACE_TEMP_FILE_NAME ACE_TEXT ("XXXXXX");

// Synchronize the start of the parent and the child.
static ACE_Process_Semaphore *process_synchronizer = 0;

static void *
child (void * = 0)
{
  int result;
#if !defined (ACE_LACKS_FORK)
  // Wait for the parent process to 
  result = process_synchronizer->acquire ();
  ACE_ASSERT (result != -1);
#endif /* !defined (ACE_LACKS_FORK) */

  char *t = ACE_ALPHABET;
  ACE_Shared_Memory_MM shm_child;

  result = shm_child.open (shm_key);
  ACE_ASSERT (result != -1);

  char *shm = (char *) shm_child.malloc ();

  ACE_ASSERT (shm != 0);

  for (char *s = shm; *s != '\0'; s++)
    {
      ACE_ASSERT (*t == s[0]);
      t++;
    }

  // Indicate to the parent that we're done.
  *shm = '*';

  return 0;
}

static void *
parent (void * = 0)
{
  int result;
  ACE_Shared_Memory_MM shm_parent;

  result = shm_parent.open (shm_key, SHMSZ);
  ACE_ASSERT (result != -1);

  char *shm = (char *) shm_parent.malloc ();

  ACE_ASSERT (shm != 0);

  char *s = shm;

  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    *s++ = *c;

  *s = '\0';

#if !defined (ACE_LACKS_FORK)
  // Allow the child process to proceed.
  result = process_synchronizer->release ();
  ACE_ASSERT (result != -1);
#endif /* !defined (ACE_LACKS_FORK) */

  // Perform a "busy wait" until the child sets the character to '*'.
  while (*shm != '*')
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("(%P) spinning in parent!\n")));

  result = shm_parent.remove ();
  ACE_ASSERT (result != -1);

  ACE_OS::unlink (shm_key);
  return 0;
}

static int
spawn (void)
{
#if !defined (ACE_LACKS_FORK)
  // Create the synchronizer before spawning the child process, to
  // avoid race condition between the creation in the parent and use
  // in the child.
  ACE_NEW_RETURN (process_synchronizer,
                  ACE_Process_Semaphore (0), // Locked by default...
                  -1);

  switch (ACE_OS::fork (ASYS_TEXT ("child")))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%P|%t) %p\n"),
                         ASYS_TEXT ("fork failed")),
                        1);
      /* NOTREACHED */
    case 0:
      parent ();
      delete process_synchronizer;
      break;
      /* NOTREACHED */
    default:
      child ();
      break;
      /* NOTREACHED */
    }
#elif defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (child),
       (void *) 0,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) %p\n"),
                       ASYS_TEXT ("thread create failed")),
                      1);
  else if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (parent),
       (void *) 0,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) %p\n"),
                       ASYS_TEXT ("thread create failed")),
                      1);
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_ERROR_RETURN ((LM_ERROR,
                     ASYS_TEXT ("only one thread may be run in a process on this platform\n")),
                    1);
#endif /* ACE_HAS_THREADS */
  return 0;
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
