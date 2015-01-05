// $Id$

//============================================================================
//
// = LIBRARY
//     tests
//
// = FILENAME
//     Auto_IncDec_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the Auto Increment/Decrement Class in
//     ACE.
//
// = AUTHOR
//     Edan Ayal <EdanA@cti2.com>
//
//============================================================================

#include "test_config.h"
#include "ace/Auto_IncDec_T.h"
#include "ace/Thread_Manager.h"
#include "ace/Atomic_Op.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_THREADS)

// Default number of threads.
static size_t n_threads = 15;

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> INTERLOCKED_INT;
static INTERLOCKED_INT current_threads_in_first_section;
static INTERLOCKED_INT current_threads_in_second_section;

static void *
worker (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) worker starting\n")));

  { // First section.
    ACE_Auto_IncDec<INTERLOCKED_INT> threads_in_section_auto_inc_dec
      (current_threads_in_first_section);

    // Wait according to the number of threads...
    ACE_Time_Value pause (current_threads_in_first_section.value (),
                          0);
    ACE_OS::sleep (pause);
  }

  { // Second section.
    ACE_Auto_IncDec<INTERLOCKED_INT> threads_in_section_auto_inc_dec
      (current_threads_in_second_section);

    // Wait according to the number of threads inside the previous
    // section...
      ACE_Time_Value pause (current_threads_in_first_section.value (),
                            0);
    ACE_OS::sleep (pause);
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) worker exiting\n")));
  return 0;
}

#endif /* ACE_HAS_THREADS */

// Spawn off threads.

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Auto_IncDec_Test"));

#if defined (ACE_HAS_THREADS)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) main thread starting\n")));

  current_threads_in_first_section = 0;
  current_threads_in_second_section = 0;

  if (ACE_Thread_Manager::instance ()->spawn_n
      (n_threads,
       ACE_THR_FUNC (worker),
       0,
       THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n")),
                      -1);
  // Make sure at least one thread is started...
  ACE_Thread::yield ();

  while (ACE_Thread_Manager::instance ()->count_threads ())
    {
      // wait according to the number of threads...
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT (" %d in first section, %d in second section, %d total\n"),
                  current_threads_in_first_section.value (),
                  current_threads_in_second_section.value (),
                  ACE_Thread_Manager::instance ()->count_threads ()));

      ACE_Time_Value pause (1, 0);
      ACE_OS::sleep (pause);
    }

  ACE_Thread_Manager::instance ()->wait ();

  ACE_ASSERT (current_threads_in_first_section.value () == 0
              && current_threads_in_second_section.value () == 0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) exiting main thread\n")));
#else
  int counter = 0;
  {
    ACE_Auto_IncDec<int> Auto_IncDec1 (counter);
    ACE_ASSERT (counter == 1);

    ACE_Auto_IncDec<int> Auto_IncDec2 (counter);
    ACE_ASSERT (counter == 2);

    {
      ACE_ASSERT (counter == 2);
      ACE_Auto_IncDec<int> Auto_IncDec3 (counter);
      ACE_ASSERT (counter == 3);
    }

    ACE_ASSERT (counter == 2);
  }

  ACE_ASSERT (counter == 0);

#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}
