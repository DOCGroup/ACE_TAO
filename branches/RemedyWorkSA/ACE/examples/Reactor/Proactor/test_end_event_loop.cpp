//=============================================================================
/**
 *  @file     test_end_event_loop.cpp
 *
 *  $Id$
 *
 *  This program tests the event loop mechanism of the
 *  Proactor. To end the event loop, threads that are blocked in
 *  waiting for completions are  woken up and the event loop comes
 *  to the end. This is tested in this program.
 *  Threads are doing <run_event_loop> with/without time_out
 *  values and the main thread calls <end_event_loop>.
 *  make
 *  ./test_end_event_loop
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_unistd.h"
#include "ace/Proactor.h"
#include "ace/Task.h"
#include "ace/WIN32_Proactor.h"
#include "ace/POSIX_Proactor.h"
#include "ace/OS_main.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
// This only works on Win32 platforms and on Unix platforms supporting
// POSIX aio calls.

/**
 * @class My_Task:
 *
 * @brief Contains thread functions which execute event loops. Each
 * thread waits for a different signal.
 */
class My_Task: public ACE_Task <ACE_NULL_SYNCH>
{
public:
  // Constructor.
  My_Task (void)
    : time_flag_ (0)
    {}

  /// Destructor.
  virtual ~My_Task (void) {}

  //FUZZ: disable check_for_lack_ACE_OS
  // If time_flag is zero do the eventloop indefinitely, otherwise do
  // it for finite amount of time (13secs!!!).
  int open (void *timed_event_loop)
    {
  //FUZZ: enble check_for_lack_ACE_OS

      // Set the local variable.
      if (timed_event_loop == 0)
        this->time_flag_ = 0;
      else
        this->time_flag_ = 1;

      // Spawn the threads.
      if (this->activate (THR_NEW_LWP, 5) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%N:%l:%p\n",
                           "My_Task:open: <activate> failed"),
                          -1);

      return 0;
    }

  // Thread function.
  int svc (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t):Starting svc routine\n"));

      if (this->time_flag_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t):Going to do *timed* <run_event_loop>\n"));

          ACE_Time_Value run_time (13);

          if (ACE_Proactor::instance ()->run_event_loop (run_time) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t):%p.\n",
                               "<Proactor::run_event_loop> failed"),
                              -1);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t):Going to do *indefinite* <run_event_loop>\n"));

          if (ACE_Proactor::instance ()->run_event_loop () == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t):%p.\n",
                               "<Proactor::run_event_loop> failed"),
                              -1);
        }
      return 0;
    };

private:
  /// If zero, indefinite event loop, otherwise timed event loop.
  int time_flag_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_DEBUG ((LM_DEBUG,
              "(%P | %t):Test starts\n"));

  // Let us get the singleton proactor created here. This is very
  // important. This will mask the signal used in the Proactor masked
  // for the main thread (and all the threads).
  ACE_Proactor *proactor = ACE_Proactor::instance ();
  ACE_UNUSED_ARG (proactor);

  My_Task task1, task2;

  // Test the indefinite run event loop.
  if (task1.open (0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):Failed to <open> the task\n"),
                      1);

  // Test the indefinite run event loop. Just pass a non-zero.
  if (task2.open ((void *)&task2) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):Failed to <open> the task\n"),
                      1);

  // Give a gap.
  ACE_OS::sleep (3);

  // End the event loop.
  if (ACE_Proactor::instance ()->end_event_loop () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):Failed to <end_event_loop>\n"),
                      1);

  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P | %t):Test ends\n"));
  return 0;
}

#else /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example cannot work with AIOCB_Proactor.\n"));
  return 1;
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

