// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Handle_Set_Test.cpp
//
// = DESCRIPTION
//      This test illustrates the use of ACE_Handle_Set to maintain a
//      set of handles. No command line arguments are needed to run
//      the test.
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Profile_Timer.h"
#include "ace/Handle_Set.h"
#include "ace/Containers.h"



static void
test_duplicates (size_t count)
{
  size_t duplicates = 0;
  size_t sets = 0;
  size_t clears = 0;

  ACE_Handle_Set handle_set;

  ACE_OS::srand ((u_int) ACE_OS::time (0L));

  for (size_t i = 0; i < count; i++)
    {
      size_t handle =
        static_cast<size_t> (ACE_OS::rand () % ACE_Handle_Set::MAXSIZE);

      if (ACE_ODD (handle))
        {
          if (handle_set.is_set ((ACE_HANDLE) handle))
            duplicates++;

          handle_set.set_bit ((ACE_HANDLE) handle);
          sets++;
        }
      else
        {
          if (handle_set.is_set ((ACE_HANDLE) handle))
            duplicates--;

          handle_set.clr_bit ((ACE_HANDLE) handle);
          clears++;
        }
    }

  ACE_TEST_ASSERT (count == sets + clears);
  ACE_TEST_ASSERT (handle_set.num_set () + duplicates == sets);
}

// This is the vector of handles to test.  These numbers are chosen to
// test for boundaries conditions.  Note that if
// <ACE_DEFAULT_SELECT_REACTOR_SIZE> is less than any of these
// <HANDLE> values, the logic in <test_boundaries> will simply ignore
// these values.
#if defined (ACE_WIN64)
  // The casts below are legit...
#  pragma warning(push)
#  pragma warning(disable : 4312)
#endif /* ACE_WIN64 */
static ACE_HANDLE handle_vector[] =
{
  (ACE_HANDLE) 0,
  (ACE_HANDLE) 1,
  (ACE_HANDLE) 31,
  (ACE_HANDLE) 32,
  (ACE_HANDLE) 63,
  (ACE_HANDLE) 64,
  (ACE_HANDLE) 65,
  (ACE_HANDLE) 127,
  (ACE_HANDLE) 128,
  (ACE_HANDLE) 129,
  (ACE_HANDLE) 254,
  (ACE_HANDLE) 255,
  (ACE_HANDLE) (ACE_DEFAULT_SELECT_REACTOR_SIZE - 1),
  (ACE_HANDLE) ACE_DEFAULT_SELECT_REACTOR_SIZE,
  ACE_INVALID_HANDLE
};
#if defined (ACE_WIN64)
#  pragma warning(pop)
#endif /* ACE_WIN64 */

static void
test_boundaries (void)
{
  ACE_Handle_Set handle_set;
  ACE_Unbounded_Set<ACE_HANDLE> set;
  ACE_HANDLE handle;

  // First test an empty set.

  for (ACE_Handle_Set_Iterator i1 (handle_set);
       (handle = i1 ()) != ACE_INVALID_HANDLE;
       )
    {
      ACE_TEST_ASSERT (0 ==
                  ACE_TEXT ("this shouldn't get called since ")
                  ACE_TEXT ("the set is empty!\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("ACE_DEFAULT_SELECT_REACTOR_SIZE %d\n"),
              ACE_DEFAULT_SELECT_REACTOR_SIZE));

  // Insert the vector of <ACE_HANDLE>s into the set.

#if defined (ACE_WIN64)
  // The casts below are legit...
#  pragma warning(push)
#  pragma warning(disable : 4312)
#endif /* ACE_WIN64 */
  for (int i = 0;
       handle_vector[i] != ACE_INVALID_HANDLE;
       i++)
    {
      if (handle_vector[i] < (ACE_HANDLE) ACE_DEFAULT_SELECT_REACTOR_SIZE)
        {
          handle_set.set_bit (handle_vector[i]);
          set.insert (handle_vector[i]);
        }
    }
#if defined (ACE_WIN64)
#  pragma warning(pop)
#endif /* ACE_WIN64 */

  int count = 0;

  for (ACE_Handle_Set_Iterator i2 (handle_set);
       (handle = i2 ()) != ACE_INVALID_HANDLE;
       )
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("obtained handle %d\n"),
                  handle));
      int done = set.remove (handle);
      ACE_TEST_ASSERT (done == 0);
      count++;
    }

  ACE_TEST_ASSERT (count == handle_set.num_set ());
}

static void
test_performance (size_t max_handles,
                  size_t max_iterations)
{
  ACE_Handle_Set handle_set;
  size_t i;

  for (i = 0; i < max_handles; i++)
    handle_set.set_bit ((ACE_HANDLE) i);

  ACE_Profile_Timer timer;
  size_t count = 0;

  timer.start ();

  for (i = 0; i < max_iterations; i++)
    {
      ACE_Handle_Set_Iterator iter (handle_set);

      // Only iterate up to <handle_set.max_set ()>.
      while (iter () != ACE_INVALID_HANDLE)
        count++;
    }

  timer.stop ();

  ACE_TEST_ASSERT (count == max_handles * max_iterations);

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("real time = %f secs, user time = %f secs, ")
                ACE_TEXT ("system time = %f secs\n"),
              et.real_time,
              et.user_time,
              et.system_time));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("time per each of the %d calls = %f usecs\n"),
              count,
              et.real_time / double (count) * 1000000));
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Handle_Set_Test"));

  int count = argc > 1
    ? ACE_OS::atoi (argv[1])
    : ACE_Handle_Set::MAXSIZE;
  size_t max_handles =
    argc > 2
    ? ACE_OS::atoi (argv[2])
    : ACE_Handle_Set::MAXSIZE;
  size_t max_iterations =
    argc > 3
    ? ACE_OS::atoi (argv[3])
    : ACE_MAX_ITERATIONS;

  test_duplicates (count);
  test_boundaries ();
  test_performance (max_handles,
                    max_iterations);
  ACE_END_TEST;
  return 0;
}

