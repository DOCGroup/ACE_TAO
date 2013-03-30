
//=============================================================================
/**
 *  @file    test_early_timeouts.cpp
 *
 *  $Id$
 *
 *  On some platforms, select() returns before the time value
 *  specified.  This tests counts the number of times this happens
 *  and the max early timeout.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "ace/Handle_Set.h"
#include "ace/Pipe.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/Truncate.h"


int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Mumber of seconds this test should run
  int runtime_in_seconds = 10;

  // Iterations
  int iterations = runtime_in_seconds * 10;

  // 100 millisecond timeout
  ACE_Time_Value timeout (0, 100000);

  // Time before starting select
  ACE_Time_Value starting_time_of_day;

  // Time before starting select
  ACE_Time_Value ending_time_of_day;

  // Number of times the timer expired early
  int no_of_early_timers = 0;

  // Maximum early timeout
  ACE_Time_Value maximum_early_timeout;

  //
  // Dummy handle and handle set
  // Note that some OS do not like "empty selects"
  //

  // Dummy handle set
  ACE_Handle_Set dummy_handle_set;

  // Dummy pipe
  ACE_Pipe dummy_pipe;
  int result = dummy_pipe.open ();
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result); // To avoid compile warning with ACE_NDEBUG.

  for (int i = 1; i <= iterations; i++)
    {
      // Add dummy handle to dummy set
      dummy_handle_set.set_bit (dummy_pipe.read_handle ());

      // Note the time before select
      starting_time_of_day = ACE_OS::gettimeofday ();

      // Wait for timeout
      result = ACE_OS::select (ACE_Utils::truncate_cast<int> ((intptr_t)dummy_pipe.read_handle ()), dummy_handle_set, 0, 0, &timeout);
      ACE_ASSERT (result == 0);

      // Note the time after select
      ending_time_of_day = ACE_OS::gettimeofday ();

      // Expected ending time
      ACE_Time_Value expected_ending_time_of_day =
        starting_time_of_day + timeout;

      // If the timer expired early
      if (ending_time_of_day < expected_ending_time_of_day)
        {
          // How early
          ACE_Time_Value early_timeout = expected_ending_time_of_day - ending_time_of_day;

          // Increment number of early timers
          no_of_early_timers++;

          // Check max early timeout
          if (early_timeout > maximum_early_timeout)
            {
              maximum_early_timeout = early_timeout;
            }
        }
    }

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              "There were %d early timers out of %d calls to select() (%f%%)\n"
              "The max early timeout was: %dsec %dusec\n",
              no_of_early_timers,
              iterations,
              float (no_of_early_timers) / iterations * 100,
              maximum_early_timeout.sec (),
              maximum_early_timeout.usec ()));
  //FUZZ: enable check_for_lack_ACE_OS

  return 0;
}
