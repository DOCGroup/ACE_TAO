// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Recursive_Mutex_Test.cpp
//
// = DESCRIPTION
//      This test program verifies the functionality of the ACE_OS
//      implementation of recursive mutexes on Win32 and Posix
//      pthreads.
//
// = AUTHOR
//    Prashant Jain and Doug C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"
#include "ace/Synch.h"

ACE_RCSID(tests, Recursive_Mutex_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

// Total number of iterations.
#if defined (CHORUS)
  // Chorus can't handle 100 iterations:
  //   [amThrd.C:154]: Failed to allocate an AmActor
  static size_t n_iterations = 45;
#else  /* ! CHORUS */
  static size_t n_iterations = 100;
#endif /* ! CHORUS */
static size_t n_threads = ACE_MAX_THREADS;

static void
recursive_worker (size_t nesting_level,
                  ACE_Recursive_Thread_Mutex *rm)
{
  if (nesting_level < n_iterations)
    {
      ACE_ASSERT (rm->acquire () == 0);
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t) = acquired, nesting = %d, thread id = %u\n"),
                  rm->get_nesting_level (),
                  rm->get_thread_id ()));

      recursive_worker (nesting_level + 1,
                        rm);

      ACE_ASSERT (rm->release () == 0);
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t) = released, nesting = %d, thread id = %u\n"),
                  rm->get_nesting_level (),
                  rm->get_thread_id ()));
    }
}

static void *
worker (void *arg)
{
  ACE_Recursive_Thread_Mutex *rm =
    ACE_reinterpret_cast (ACE_Recursive_Thread_Mutex *, 
                          arg);
  recursive_worker (0, rm);
  return 0;
}

#endif /* ACE_HAS_THREADS */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Recursive_Mutex_Test"));

#if defined (ACE_HAS_THREADS)
  ACE_Recursive_Thread_Mutex rm;
  ACE_Thread_Manager::instance ()->spawn_n (n_threads,
                                            ACE_THR_FUNC (worker),
                                            (void *) &rm);
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("ACE doesn't support recursive process mutexes on this platform\n")));
#endif /* ACE_WIN32 */
  ACE_END_TEST;
  return 0;
}
