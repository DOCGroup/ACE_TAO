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

#include "ace/Profile_Timer.h"
#include "ace/Handle_Set.h"
#include "ace/Containers.h"
#include "test_config.h"

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
      size_t handle = size_t (ACE_OS::rand () % ACE_Handle_Set::MAXSIZE);

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
  (ACE_HANDLE) 255,
  ACE_INVALID_HANDLE
};

static void
test_boundaries (void)
{
  ACE_Handle_Set handle_set;
  ACE_Unbounded_Queue<ACE_HANDLE> queue;
  ACE_HANDLE handle;

  // First test an empty set.

  ACE_Handle_Set_Iterator i1 (handle_set);

  while ((handle = i1 ()) != ACE_INVALID_HANDLE)
    ACE_ASSERT (!"this shouldn't get called since the set is empty!\n");

  // Insert the vector of HANDLEs into the set.

  for (int i = 0;
       handle_vector[i] != ACE_INVALID_HANDLE;
       i++)
    {
      handle_set.set_bit (handle_vector[i]);
      queue.enqueue_tail (handle_vector[i]);
    }

  int count = 0;

  ACE_Handle_Set_Iterator i2 (handle_set); 

  while ((handle = i2 ()) != ACE_INVALID_HANDLE)
    {
      ACE_HANDLE temp;
      queue.dequeue_head (temp);
      ACE_ASSERT (handle == temp);
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
  ACE_HANDLE handle;
  size_t count = 0;

  timer.start ();

  for (i = 0; i < max_iterations; i++)
    {
      ACE_Handle_Set_Iterator iter (handle_set);

      // Only iterate up to <handle_set.max_set ()>.
      while ((handle = iter ()) != ACE_INVALID_HANDLE)
	count++;
    }

  timer.stop ();

  ACE_ASSERT (count == max_handles * max_iterations);

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
	      "real time = %f secs, user time = %f secs, system time = %f secs\n",
	      et.real_time,
	      et.user_time,
	      et.system_time));

  ACE_DEBUG ((LM_DEBUG,
	      "time per each of the %d calls = %f usecs\n",
	      count,
	      (et.real_time / double (count)) * 1000000));
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Handle_Set_Test");

  int count = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_Handle_Set::MAXSIZE;
  size_t max_handles = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_Handle_Set::MAXSIZE;
  size_t max_iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : ACE_MAX_ITERATIONS;

  test_duplicates (count);
  test_boundaries ();
  test_performance (max_handles, max_iterations);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Unbounded_Queue<int>;
template class ACE_Unbounded_Queue_Iterator<int>;
template class ACE_Node<int>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
