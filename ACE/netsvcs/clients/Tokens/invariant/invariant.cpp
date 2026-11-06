//=============================================================================
/**
 *  @file    invariant.cpp
 *
 *  @author Tim Harrison
 */
//=============================================================================

#include "ace/Thread_Manager.h"
#include "ace/Token_Invariants.h"

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREADS_LIBRARY)

using ACE_TOKEN_INVARIANTS = ACE_Token_Invariant_Manager;

static constexpr ACE_TCHAR const *rwname = ACE_TEXT ("reader/writer");
static constexpr ACE_TCHAR const *mutexname = ACE_TEXT ("mutex");

static ACE_THR_FUNC_RETURN
run_reader_writer (void *)
{
  for (int iteration {}; iteration < 50; ++iteration)
    {
      int acquisitions {};
      for (; acquisitions < 5; ++acquisitions)
        {
          if (ACE_TOKEN_INVARIANTS::instance ()->reader_acquired (rwname) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "reader acquire violated invariant.\n"), 0);

          ACE_DEBUG ((LM_DEBUG, "(%t) rlock acquired.\n"));
        }

      ACE_TOKEN_INVARIANTS::instance ()->rwlock_releasing (rwname);

      if (ACE_TOKEN_INVARIANTS::instance ()->reader_acquired (rwname) == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "reader renew violated invariant.\n"), 0);

      ACE_DEBUG ((LM_DEBUG, "(%t) rlock renewed.\n"));

      for (; acquisitions > 0; --acquisitions)
        {
          ACE_TOKEN_INVARIANTS::instance ()->rwlock_releasing (rwname);
          ACE_DEBUG ((LM_DEBUG, "(%t) r-released.\n"));
        }

      if (ACE_TOKEN_INVARIANTS::instance ()->writer_acquired (rwname) == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "writer acquire violated invariant.\n"), 0);

      ACE_DEBUG ((LM_DEBUG, "\t\t(%t) wlock acquired.\n"));

      ACE_TOKEN_INVARIANTS::instance ()->rwlock_releasing (rwname);

      if (ACE_TOKEN_INVARIANTS::instance ()->writer_acquired (rwname) == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "writer renew violated invariant.\n"), 0);

      ACE_DEBUG ((LM_DEBUG, "(%t) rlock renewed.\n"));

      ACE_TOKEN_INVARIANTS::instance ()->rwlock_releasing (rwname);
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) thread exiting.\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static ACE_THR_FUNC_RETURN
run_mutex (void *)
{
  for (int iteration {}; iteration < 50; ++iteration)
    {
      if (ACE_TOKEN_INVARIANTS::instance ()->mutex_acquired (mutexname) == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "mutex acquire violated invariant.\n"), 0);

      ACE_DEBUG ((LM_DEBUG, "(%t) mutex acquired.\n"));

      ACE_TOKEN_INVARIANTS::instance ()->mutex_releasing (mutexname);

      if (ACE_TOKEN_INVARIANTS::instance ()->mutex_acquired (mutexname) == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "mutex renew violated invariant.\n"), 0);

      ACE_DEBUG ((LM_DEBUG, "(%t) mutex renewed.\n"));

      ACE_TOKEN_INVARIANTS::instance ()->mutex_releasing (mutexname);
      ACE_DEBUG ((LM_DEBUG, "(%t) mutex released.\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) thread exiting.\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static int
run_final_test ()
{
  ACE_DEBUG ((LM_DEBUG, "starting mutex tests 1 & 2\n"));

  // Mutex tests.
  if (ACE_TOKEN_INVARIANTS::instance ()->mutex_acquired (ACE_TEXT ("testing mutex")) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "mutex test 1 failed.\n"), 0);
  if (ACE_TOKEN_INVARIANTS::instance ()->mutex_acquired (ACE_TEXT ("testing mutex2")) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "mutex test 2 failed.\n"), 0);
  if (ACE_TOKEN_INVARIANTS::instance ()->mutex_acquired (ACE_TEXT ("testing mutex")) == 0)
    ACE_DEBUG ((LM_DEBUG, "mutex test 1 succeeded.\n"));
  else
    ACE_ERROR_RETURN ((LM_ERROR, "mutex test 1 failed..\n"), 0);

  if (ACE_TOKEN_INVARIANTS::instance ()->mutex_acquired (ACE_TEXT ("testing mutex2")) == 0)
    ACE_DEBUG ((LM_DEBUG, "mutex test 2 succeeded.\n"));
  else
    ACE_ERROR_RETURN ((LM_ERROR, "mutex test 2 failed..\n"), 0);

  // RW tests.
  ACE_DEBUG ((LM_DEBUG, "starting rwlock tests 1 & 2\n"));

  // Multiple readers.
  if (ACE_TOKEN_INVARIANTS::instance ()->reader_acquired (ACE_TEXT ("testing rwlock")) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "rwlock test 1 failed.\n"), 0);
  if (ACE_TOKEN_INVARIANTS::instance ()->reader_acquired (ACE_TEXT ("testing rwlock 2")) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "rwlock test 2 failed.\n"), 0);
  if (ACE_TOKEN_INVARIANTS::instance ()->reader_acquired (ACE_TEXT ("testing rwlock")) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "rwlock test 1 failed..\n"), 0);
  if (ACE_TOKEN_INVARIANTS::instance ()->reader_acquired (ACE_TEXT ("testing rwlock 2")) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "rwlock test 2 failed..\n"), 0);

  // Writer.
  if (ACE_TOKEN_INVARIANTS::instance ()->writer_acquired (ACE_TEXT ("testing rwlock")) == 0)
    ACE_DEBUG ((LM_ERROR, "rwlock test 1 succeded.\n"));
  else
    ACE_ERROR_RETURN ((LM_ERROR, "rwlock test 1 failed...\n"), 0);

  // Releasing reader.
  ACE_TOKEN_INVARIANTS::instance ()->rwlock_releasing (ACE_TEXT ("testing rwlock 2"));
  ACE_TOKEN_INVARIANTS::instance ()->rwlock_releasing (ACE_TEXT ("testing rwlock 2"));

  // Writer.
  if (ACE_TOKEN_INVARIANTS::instance ()->writer_acquired (ACE_TEXT ("testing rwlock 2")) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "rwlock test 2 failed....\n"), 0);

  // Reader.
  if (ACE_TOKEN_INVARIANTS::instance ()->reader_acquired (ACE_TEXT ("testing rwlock 2")) == 0)
    ACE_DEBUG ((LM_DEBUG, "rwlock test 2 succeeded.\n"));
  else
    ACE_ERROR_RETURN ((LM_ERROR, "rwlock test 2 failed.....\n"), 0);

  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Thread_Manager mgr;

  // Run reader/writer test
  if (mgr.spawn_n (2, run_reader_writer,
                   nullptr,
                   THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn failed"), -1);

  mgr.wait ();

  ACE_OS::sleep (2);

  // Run mutex test.
  if (mgr.spawn_n (2, run_mutex,
                   nullptr,
                   THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn failed"), -1);

  mgr.wait ();

  ACE_OS::sleep (2);

  run_final_test ();

  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
