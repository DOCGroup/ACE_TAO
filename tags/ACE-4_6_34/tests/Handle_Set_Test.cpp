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
//    Prashant Jain
//
// ============================================================================

#include "test_config.h"
#include "ace/Profile_Timer.h"
#include "ace/Handle_Set.h"
#include "ace/Containers.h"

ACE_RCSID(tests, Handle_Set_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static void
test_duplicates (size_t count)
{
  size_t duplicates = 0;
  size_t sets = 0;
  size_t clears = 0;

  ACE_Handle_Set handle_set;

  ACE_OS::srand (ACE_OS::time (0L));

  for (size_t i = 0; i < count; i++)
    {
      size_t handle = ACE_static_cast(size_t, ACE_OS::rand () % ACE_Handle_Set::MAXSIZE);

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

  ACE_ASSERT (count == sets + clears);
  ACE_ASSERT (handle_set.num_set () + duplicates == sets);
}

// This is the vector of handles to test.  These numbers are chosen to
// test for boundaries conditions.
static ACE_HANDLE handle_vector[] =
{
  (ACE_HANDLE) 0,
  (ACE_HANDLE) 1,
  (ACE_HANDLE) 32,
  (ACE_HANDLE) 63,
  (ACE_HANDLE) 64,
  (ACE_HANDLE) 65,
  (ACE_HANDLE) 127,
  (ACE_HANDLE) 128,
  (ACE_HANDLE) 129,
#if defined(ACE_DEFAULT_REACTOR_SIZE) && ACE_DEFAULT_REACTOR_SIZE >= 255
  (ACE_HANDLE) 255,
#endif
  ACE_INVALID_HANDLE
};

static void
test_boundaries (void)
{
  ACE_Handle_Set handle_set;
  ACE_Unbounded_Set<ACE_HANDLE> set;
  ACE_HANDLE handle;

  // First test an empty set.

  ACE_Handle_Set_Iterator i1 (handle_set);

  while ((handle = i1 ()) != ACE_INVALID_HANDLE)
    {
      ACE_ASSERT (ASYS_TEXT ("this shouldn't get called since the set is empty!\n") == 0);
    }

  // Insert the vector of HANDLEs into the set.

  for (int i = 0;
       handle_vector[i] != ACE_INVALID_HANDLE;
       i++)
    {
      handle_set.set_bit (handle_vector[i]);
      set.insert (handle_vector[i]);
    }

  int count = 0;

  ACE_Handle_Set_Iterator i2 (handle_set);
  while ((handle = i2 ()) != ACE_INVALID_HANDLE)
    {
      int done = set.remove(handle);
      ACE_ASSERT(done == 0);
      count++;
    }

  ACE_ASSERT (count == handle_set.num_set ());
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

  ACE_ASSERT (count == max_handles * max_iterations);

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

#if defined (ACE_LACKS_FLOATING_POINT)
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("real time = %u usecs\n"),
              et.real_time));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("time per each of the %d calls = %u usecs\n"),
              count,
              et.real_time / count));
#else  /* ! ACE_LACKS_FLOATING_POINT */
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("real time = %f secs, user time = %f secs, system time = %f secs\n"),
              et.real_time,
              et.user_time,
              et.system_time));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("time per each of the %d calls = %f usecs\n"),
              count,
              et.real_time / double (count) * 1000000));
#endif /* ! ACE_LACKS_FLOATING_POINT */
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Handle_Set_Test"));

  int count = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_Handle_Set::MAXSIZE;
  size_t max_handles = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_Handle_Set::MAXSIZE;
  size_t max_iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : ACE_MAX_ITERATIONS;

  test_duplicates (count);
  test_boundaries ();
  test_performance (max_handles, max_iterations);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// The ACE_Node<ACE_INT32> instantation is in ace/Stats.cpp.
template class ACE_Unbounded_Set<ACE_HANDLE>;
template class ACE_Unbounded_Set_Iterator<ACE_HANDLE>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// The ACE_Node<ACE_INT32> instantation is in ace/Stats.cpp.
#pragma instantiate ACE_Unbounded_Set<ACE_HANDLE>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_HANDLE>
# if defined(_CRAYMPP)
// MPP Cray ACE_HANDLE is 64-bit, defined as int, but ACE_INT32 is short
// so instantiation in ace/Stats.cpp isn't used in this case
#pragma instantiate ACE_Node<ACE_HANDLE>
# endif
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

