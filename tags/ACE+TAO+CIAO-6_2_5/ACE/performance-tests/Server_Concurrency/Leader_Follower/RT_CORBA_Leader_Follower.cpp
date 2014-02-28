// $Id$

#include "RT_CORBA_Leader_Follower.h"



#if defined (ACE_HAS_THREADS)

// We need the following only if we have threads enabled..
#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"

// Number of messages that are used in this experiment
static size_t number_of_messages = 1000;

// Number of messages that are used in this experiment
static size_t number_of_messages_left = 0;

// Number of threads used in this experiment
static size_t number_of_threads = 2;

// Global variable for the availability of the leader
static size_t leader_available = 0;

// Number of threads that are ready to go/dispatch
static size_t ready_threads = 0;

// Work in the upcall
static size_t message_size = 100;

// Debugging condition
static DEBUGGING_RANGE debug = DEBUG_NONE;

// Timer for the test
ACE_High_Res_Timer test_timer;




/*******************************************************************/
// Constructor for Synchronisers
Synchronisers::Synchronisers (void)
  : mutex_ (),
    condition_ (mutex_),
    event_ ()
{
}


int
Synchronisers::start_synchronization (void)
{
  // Hold the lock and increment the global variable to indicate
  // number of ready threads
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

    ready_threads ++;

    if (debug)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Ready to go..\n"));
      }

    if (ready_threads == number_of_threads)
      {

        // Reset the ready_threads so that we can wait at the end of
        // runs
        ready_threads = 0;

        // Start the timer
        test_timer.start ();

        // Signal all the threads
        this->event_.signal ();

        // return to do our work;
        return 0;
      }

    // If we are not the last thread, let go off the lock
  }

  // Wait blisfully till we are woken up
  this->event_.wait ();

  return 0;
}

int
Synchronisers::end_synchronization (void)
{
  // Hold the lock and increment the global variable to indicate
  // number of ready threads
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

    ready_threads ++;

    if (debug)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Ready to go..\n"));
      }

    if (ready_threads == number_of_threads)
      {

        // Reset the ready_threads so that we can wait at the end of
        // runs
        ready_threads = 0;

        // Start the timer
        test_timer.stop ();

        // Signal all the threads
        this->event_.signal ();

        // return to do our work;
        return 0;
      }

    // If we are not the last thread, let go off the lock
  }

  // Wait blisfully till we are woken up
  this->event_.wait ();

  return 0;
}


/*******************************************************************/

Leader_Follower_Task::Leader_Follower_Task (Synchronisers &synch)
  : messages_consumed_ (0),
    synch_ (synch)
{
}

int
Leader_Follower_Task::processed (void)
{
  return this->messages_consumed_;
}

int
Leader_Follower_Task::svc (void)
{
  (void) this->synch_.start_synchronization ();

  for (;;)
    {
      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->synch_.mutex_, -1);

        // Wait until there is no leader.
        while (leader_available)
          {
            int result = this->synch_.condition_.wait ();

            if (result == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Leader_Follower_Task::svc (%t) -> %p\n",
                                   "wait error"),
                                  -1);
              }
          }

        // I am the leader.
        leader_available = 1;

        //
        // We are letting go of the leader follower lock before going
        // in the event loop.
        //
      }

      //
      // It is ok to modify these shared variables without a lock
      // since we are the only leader.
      //

      int exit_loop = 0;
      if (number_of_messages_left == 0)
        {
          exit_loop = 1;
        }
      else
        {
          --number_of_messages_left;

          // Local counter.
          ++this->messages_consumed_;

          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) message for this thread %d\n",
                          this->messages_consumed_));
            }
        }

      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->synch_.mutex_, -1);

        // I am no longer the leader.
        leader_available = 0;

        // Wake up a follower.
        this->synch_.condition_.signal ();
      }

      if (exit_loop)
        {
          break;
        }
      else
        {
          //
          // Process message here.
          //
          for (size_t j = 0; j < message_size; ++j)
            {
              // Eat a little CPU
              /* takes about 40.2 usecs on a 167 MHz Ultra2 */
              u_long n = 11UL;
              ACE::is_prime (n, 2, n / 2);
            }

        }
    }

  (void) this->synch_.end_synchronization ();

  return 0;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("m:t:d:s:"));
  int c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'm':
          number_of_messages = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 't':
          number_of_threads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          debug = static_cast<DEBUGGING_RANGE> (ACE_OS::atoi (get_opt.opt_arg ()));
          break;
        case 's':
          message_size = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s\n"
                             "\t[-m number of messages]\n"
                             "\t[-s message size]\n"
                             "\t[-w number of threads]\n"
                             "\t[-b burst size]\n"
                             "\t[-t timeout between bursts]\n"
                             "\t[-d debug]\n",
                             argv[0]),
                            -1);
        }
    }

  return 0;
}

/*******************************************************************/

// Entry point

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = parse_args (argc, argv);

  if (result != 0)
    {
      return result;
    }

  Synchronisers synch_forms;

  ACE_High_Res_Timer::calibrate ();

  // Leader Followers.
  Leader_Follower_Task **leader_followers = 0;
  ACE_NEW_RETURN (leader_followers,
                  Leader_Follower_Task *[number_of_threads],
                  -1);

  int priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);

  long flags = THR_SCOPE_PROCESS;

  // Number of messages left = Number_Of_messages
  number_of_messages_left = number_of_messages;

  size_t i = 0;
  // Create and activate them.
  for (i = 0; i < number_of_threads; ++i)
    {
      ACE_NEW_RETURN (leader_followers[i],
                      Leader_Follower_Task (synch_forms),
                      -1);

      // Activate the leader_followers.
      result = leader_followers[i]->activate (flags,
                                              1,
                                              1,
                                              priority);
      if (result != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) - Activate failed  for RT class "
                      " - Using default priority for thread [%d]\n",
                      i));

          flags = THR_BOUND;
          priority = ACE_Sched_Params::priority_min (ACE_SCHED_OTHER,
                                                     ACE_SCOPE_THREAD);

          // Activate the leader_followers.
          result = leader_followers[i]->activate (flags,
                                                  1,
                                                  1,
                                                  priority);

          if (result != 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) - Failed again no hope\n"));

              return 0;
            }

        }
    }

  // Wait for all threads to terminate.
  result = ACE_Thread_Manager::instance ()->wait ();

  ACE_hrtime_t elapsed_time = 0;

  test_timer.elapsed_time (elapsed_time);

  double elapsed_time_per_invocation =
    static_cast<double> (
        ACE_UINT64_DBLCAST_ADAPTER (elapsed_time / number_of_messages)
      );

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Throughput is [%f]\n",
              1000000000/ elapsed_time_per_invocation));

  for (i = 0; i < number_of_threads; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Message consumed in thread [%d] is [%d]\n",
                  i, leader_followers[i]->processed ()));
      delete leader_followers[i];
    }

  delete[] leader_followers;

  return result;
}

#else /*if defined (ACE_HAS_THREADS)*/

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "(%p|%t) Cannot run in SIngle threaded mode\n"));

  return 0;
}
#endif /*ACE_HAS_THREADS*/
