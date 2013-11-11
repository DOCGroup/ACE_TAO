// $Id$

#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"
#include "ace/ACE.h"
#include "ace/Task_T.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"
#include "ace/Lock_Adapter_T.h"
#include "../Latency_Stats.h"

static size_t number_of_messages = 100;
static size_t message_size = 100;
static size_t number_of_workers = 10;
static size_t burst_size = 10;
static size_t timeout_between_bursts = 1;

enum DEBUGGING_RANGE
{
  DEBUG_NONE = 0,
  DEFAULT = 1,
  PRINT_INDIVIDUAL_LATENCY = 2
};

static DEBUGGING_RANGE debug = DEBUG_NONE;

static ACE_Data_Block *data_block = 0;

class Message_Block : public ACE_Message_Block
{
public:
  Message_Block (ACE_Data_Block *data_block,
                 ACE_hrtime_t start_of_burst);

  ACE_hrtime_t start_of_burst_;
};

Message_Block::Message_Block (ACE_Data_Block *data_block,
                              ACE_hrtime_t start_of_burst)
  : ACE_Message_Block (data_block),
    start_of_burst_ (start_of_burst)
{
}

typedef ACE_Task<ACE_SYNCH> TASK;

class Worker_Task : public TASK
{
public:
  Worker_Task (ACE_Message_Queue<ACE_SYNCH> *mq);
  int svc (void);

  size_t messages_dequeued_;

  Latency_Stats latency_stats_;
  Throughput_Stats throughput_stats_;
};

class IO_Task : public TASK
{
public:
  IO_Task (ACE_Message_Queue<ACE_SYNCH> *mq);
  int svc (void);
};

Worker_Task::Worker_Task (ACE_Message_Queue<ACE_SYNCH> *mq)
  : TASK (0, mq),
    messages_dequeued_ (0)
{
}

int
Worker_Task::svc (void)
{
  for (;;)
    {
      ACE_Message_Block *mb = 0;
      int result = this->getq (mb);
      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Worker_Task::svc (%t) -> %p\n",
                             "getq error"),
                            -1);
        }

      ACE_Message_Block::ACE_Message_Type message_type =
        mb->msg_type ();

      // If STOP message, break loop and end the task.
      if (message_type == ACE_Message_Block::MB_STOP)
        {
          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) stop message dequeued after %d data messages\n",
                          this->messages_dequeued_));
            }

          mb->release ();
          break;
        }

      Message_Block *message_block =
        dynamic_cast<Message_Block *> (mb);

      ACE_hrtime_t start_of_burst_for_this_message_block =
        message_block->start_of_burst_;

      mb->release ();

      // Counter.
      ++this->messages_dequeued_;

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) dequeued its %d message\n",
                      this->messages_dequeued_));
        }

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
        ACE_OS::gethrtime () - start_of_burst_for_this_message_block;
      this->latency_stats_.sample (latency_from_start_of_burst);

      this->throughput_stats_.sample ();

      if (debug >= PRINT_INDIVIDUAL_LATENCY)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) latency from start of burst: %Q\n",
                      latency_from_start_of_burst));
        }
    }

  return 0;
}

IO_Task::IO_Task (ACE_Message_Queue<ACE_SYNCH> *mq)
  : TASK (0, mq)
{
}

int
IO_Task::svc (void)
{
  size_t i = 0;
  size_t messages_queued = 1;
  size_t burst = 1;

  // Data messages.
  while (number_of_messages > 0)
    {
      ACE_hrtime_t start_of_burst = ACE_OS::gethrtime ();

      for (i = 1;
           i <= burst_size && number_of_messages > 0;
           ++i, --number_of_messages, ++messages_queued)
        {
          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) IO thread -> burst %d: message %d; overall message %d\n",
                          burst,
                          i,
                          messages_queued));
            }

          Message_Block *message_block = 0;
          ACE_NEW_RETURN (message_block,
                          Message_Block (data_block,
                                         start_of_burst),
                          -1);

          int result = this->putq (message_block);
          if (result == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "IO::svc (%t) -> %p\n",
                                 "putq error"),
                                -1);
            }
        }

      ++burst;
      ACE_Time_Value tv (0, timeout_between_bursts);
      ACE_OS::sleep (tv);
    }

  // Terminate messages.
  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_Message_Block *message_block = 0;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block ((size_t)0,
                                         (int)ACE_Message_Block::MB_STOP),
                      -1);

      int result = this->putq (message_block);
      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "IO::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
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
          number_of_workers = ACE_OS::atoi (get_opt.opt_arg ());
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
                             "\t[-w number of workers]\n"
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

  size_t i = 0;

  ACE_NEW_RETURN (data_block,
                  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >,
                  -1);

  for (i = 0; i < number_of_messages; ++i)
    {
      data_block->duplicate ();
    }

  ACE_Message_Queue<ACE_SYNCH> message_queue;

  // Workers.
  Worker_Task **workers = 0;
  ACE_NEW_RETURN (workers,
                  Worker_Task *[number_of_workers],
                  -1);

  ACE_Profile_Timer timer;
  timer.start ();

  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO) +
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  // priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO, priority);

  long flags = THR_BOUND | THR_SCHED_FIFO;

  // Create and activate them.
  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_NEW_RETURN (workers[i],
                      Worker_Task (&message_queue),
                      -1);

      // Activate the workers.
      result = workers[i]->activate (flags,
                                     1,
                                     1,
                                     priority);
      if (result != 0)
        {
          flags = THR_BOUND;
          priority = ACE_Sched_Params::priority_min (ACE_SCHED_OTHER,
                                                     ACE_SCOPE_THREAD);
          result = workers[i]->activate (flags,
                                         1,
                                         1,
                                         priority);
          if (result != 0)
            {
              return result;
            }
        }
    }

  // IO Task.
  IO_Task io (&message_queue);

  // Activate the workers.
  priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO) +
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO, priority);

  flags = THR_BOUND | THR_SCHED_FIFO;

  result = io.activate (THR_BOUND);
  if (result != 0)
    {
      flags = THR_BOUND;
      priority = ACE_Sched_Params::priority_min (ACE_SCHED_OTHER,
                                                 ACE_SCOPE_THREAD);
      result = io.activate (flags,
                            1,
                            1,
                            priority);
      if (result != 0)
        {
          return result;
        }
    }

  // Wait for all threads to terminate.
  result = ACE_Thread_Manager::instance ()->wait ();

  timer.stop ();
  ACE_Rusage rusage;
  timer.elapsed_rusage (rusage);

  Latency_Stats latency;
  Throughput_Stats throughput;
  for (i = 0; i < number_of_workers; ++i)
    {
      latency.accumulate (workers[i]->latency_stats_);
      throughput.accumulate (workers[i]->throughput_stats_);
      ACE_DEBUG ((LM_DEBUG, "Thread[%d]: ", i));
      workers[i]->throughput_stats_.dump_results (ACE_TEXT(""), ACE_TEXT(""));
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

  for (i = 0; i < number_of_workers; ++i)
    {
      delete workers[i];
    }
  delete[] workers;
  delete data_block;

  return result;
}

