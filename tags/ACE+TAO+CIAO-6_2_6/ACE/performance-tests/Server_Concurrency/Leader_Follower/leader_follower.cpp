// $Id$

#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"
#include "ace/ACE.h"
#include "ace/Task_T.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"
#include "../Latency_Stats.h"

static size_t number_of_messages = 100;
static size_t message_size = 100;
static size_t number_of_threads = 10;
static size_t burst_size = 10;
static size_t timeout_between_bursts = 1;

static size_t leader_available = 0;
static size_t messages_in_this_burst = 0;
static size_t total_messages_consumed = 0;
static size_t burst = 1;

static ACE_hrtime_t start_of_burst;

enum DEBUGGING_RANGE
{
  DEBUG_NONE = 0,
  DEFAULT = 1,
  PRINT_INDIVIDUAL_LATENCY = 2
};

static DEBUGGING_RANGE debug = DEBUG_NONE;

typedef ACE_Task<ACE_SYNCH> TASK;

class Leader_Follower_Task : public TASK
{
public:
  Leader_Follower_Task (ACE_SYNCH_MUTEX &mutex,
                        ACE_SYNCH_CONDITION &condition);
  int svc (void);

  size_t messages_consumed_;
  ACE_SYNCH_MUTEX &mutex_;
  ACE_SYNCH_CONDITION &condition_;

  Latency_Stats latency_stats_;
  Throughput_Stats throughput_stats_;
};

Leader_Follower_Task::Leader_Follower_Task (ACE_SYNCH_MUTEX &mutex,
                                            ACE_SYNCH_CONDITION &condition)
  : messages_consumed_ (0),
    mutex_ (mutex),
    condition_ (condition)
{
}

int
Leader_Follower_Task::svc (void)
{
  for (;;)
    {
      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

        // Wait until there is no leader.
        while (leader_available)
          {
            int result = this->condition_.wait ();
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
      if (number_of_messages == 0)
        {
          exit_loop = 1;
        }
      else
        {
          if (messages_in_this_burst == burst_size)
            {
              ++burst;
              messages_in_this_burst = 0;
              ACE_Time_Value tv (0, timeout_between_bursts);
              ACE_OS::sleep (tv);
            }

          if (messages_in_this_burst == 0)
            {
              start_of_burst = ACE_OS::gethrtime ();
            }

          --number_of_messages;

          // Burst counter.
          ++messages_in_this_burst;

          // Global counter.
          ++total_messages_consumed;

          // Local counter.
          ++this->messages_consumed_;

          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) burst %d: message %d; overall message %d; message for this thread %d\n",
                          burst,
                          messages_in_this_burst,
                          total_messages_consumed,
                          this->messages_consumed_));
            }
        }

      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

        // I am no longer the leader.
        leader_available = 0;

        // Wake up a follower.
        this->condition_.signal ();
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

          //
          // Record stats for this message.
          //
          ACE_hrtime_t latency_from_start_of_burst =
            ACE_OS::gethrtime () - start_of_burst;
          this->latency_stats_.sample (latency_from_start_of_burst);

          this->throughput_stats_.sample ();

          if (debug >= PRINT_INDIVIDUAL_LATENCY)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) latency from start of burst: %Q\n",
                          latency_from_start_of_burst));
            }
        }
    }

  return 0;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("m:s:w:b:t:d:"));
  int c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'm':
          number_of_messages = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 's':
          message_size = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'w':
          number_of_threads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'b':
          burst_size = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 't':
          timeout_between_bursts = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          debug = static_cast<DEBUGGING_RANGE> (ACE_OS::atoi (get_opt.opt_arg ()));
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

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  move_to_rt_class ();
  ACE_High_Res_Timer::calibrate ();

  ACE_SYNCH_MUTEX mutex;
  ACE_SYNCH_CONDITION condition (mutex);

  // Leader Followers.
  Leader_Follower_Task **leader_followers = 0;
  ACE_NEW_RETURN (leader_followers,
                  Leader_Follower_Task *[number_of_threads],
                  -1);

  ACE_Profile_Timer timer;
  timer.start ();

  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO) +
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  long flags = THR_BOUND | THR_SCHED_FIFO;

  // Create and activate them.
  size_t i = 0;
  for (i = 0; i < number_of_threads; ++i)
    {
      ACE_NEW_RETURN (leader_followers[i],
                      Leader_Follower_Task (mutex,
                                            condition),
                      -1);

      // Activate the leader_followers.
      result = leader_followers[i]->activate (flags,
                                              1,
                                              1,
                                              priority);
      if (result != 0)
        {
          flags = THR_BOUND;
          priority = ACE_Sched_Params::priority_min (ACE_SCHED_OTHER,
                                                     ACE_SCOPE_THREAD);
          result = leader_followers[i]->activate (flags,
                                                  1,
                                                  1,
                                                  priority);
          if (result != 0)
            {
              return result;
            }
        }
    }

  // Wait for all threads to terminate.
  result = ACE_Thread_Manager::instance ()->wait ();

  timer.stop ();
  ACE_Rusage rusage;
  timer.elapsed_rusage (rusage);

  Latency_Stats latency;
  Throughput_Stats throughput;
  for (i = 0; i < number_of_threads; ++i)
    {
      latency.accumulate (leader_followers[i]->latency_stats_);
      throughput.accumulate (leader_followers[i]->throughput_stats_);
      ACE_DEBUG ((LM_DEBUG, "Thread[%d]: ", i));
      leader_followers[i]->throughput_stats_.dump_results (ACE_TEXT(""), ACE_TEXT(""));
    }

  ACE_DEBUG ((LM_DEBUG, "\nTotals for latency:\n"));
  latency.dump_results (argv[0], ACE_TEXT("latency"));

  ACE_DEBUG ((LM_DEBUG, "\nTotals for throughput:\n"));
  throughput.dump_results (argv[0], ACE_TEXT("throughput"));

#if defined(ACE_HAS_PRUSAGE_T)
  ACE_DEBUG ((LM_DEBUG, "\n(%t) Context switches %d/%d\n",
              rusage.pr_vctx,
              rusage.pr_ictx));
#endif /* ACE_HAS_PRUSAGE_T */

  for (i = 0; i < number_of_threads; ++i)
    {
      delete leader_followers[i];
    }
  delete[] leader_followers;

  return result;
}
