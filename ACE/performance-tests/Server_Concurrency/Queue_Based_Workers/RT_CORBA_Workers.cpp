#include "RT_CORBA_Workers.h"

#if defined (ACE_HAS_THREADS)

#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Lock_Adapter_T.h"
#include <cerrno>
#include <limits>

// The number of messages that is being processed
static size_t number_of_messages {100};

// The number of upcall threads
static size_t number_of_workers {2};

// The size of the message
static size_t message_size {100};

// Number of threads that are ready to go
static size_t ready_threads {};

// Number of input and output threads
static size_t io_threads {2}; // 1 for output and 1 for input

// High resolution test timer
static ACE_High_Res_Timer test_timer;

// Debugging condition
static DEBUGGING_RANGE debug {DEBUGGING_RANGE::DEBUG_NONE};

// Data block used by the message blocks
// Keep its base reference until all queued messages have been released.
ACE_Data_Block *data_block {};

static bool
parse_size (ACE_TCHAR const *argument, size_t &value)
{
  ACE_TCHAR *end {};
  errno = 0;
  long const parsed = ACE_OS::strtol (argument, &end, 10);
  if (argument == end || *end != ACE_TEXT ('\0') || errno == ERANGE || parsed < 0
      || static_cast<unsigned long> (parsed) > std::numeric_limits<size_t>::max ())
    {
      return false;
    }

  value = static_cast<size_t> (parsed);
  return true;
}

Synchronisers::Synchronisers ()
  : mutex_ (),
    event_ ()
{
}


int
Synchronisers::start_synchronization ()
{
  // Hold the lock and increment the global variable to indicate
  // number of ready threads
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

    ++ready_threads;

    if (ready_threads == (number_of_workers + io_threads))
      {
        // Reset the ready_threads so that we can wait at the end of
        // runs
        ready_threads = 0;

        if (debug != DEBUGGING_RANGE::DEBUG_NONE)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Ready to signal start\n"));
          }
        // Start the timer
        test_timer.start ();

        // Signal all the threads
        this->event_.signal ();

        // return to do our work;
        return 0;
      }

    // If we are not the last thread, let go off the lock
  }

  if (debug != DEBUGGING_RANGE::DEBUG_NONE)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Ready to wait () on event..\n"));
      //FUZZ: enable check_for_lack_ACE_OS
    }

  // Wait blisfully till we are woken up
  this->event_.wait ();

  return 0;
}

int
Synchronisers::end_synchronization ()
{
  // Hold the lock and increment the global variable to indicate
  // number of ready threads
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

    ++ready_threads;

    if (ready_threads == (number_of_workers + io_threads))
      {
        // Reset the ready_threads so that we can wait at the end of
        // runs
        ready_threads = 0;

        // Start the timer
        test_timer.stop ();

        // Signal all the threads
        this->event_.signal ();

        if (debug != DEBUGGING_RANGE::DEBUG_NONE)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Ended peacefully\n"));
          }

        // return to do our work;
        return 0;
      }


    // If we are not the last thread, let go off the lock
  }

  if (debug != DEBUGGING_RANGE::DEBUG_NONE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Going to wait ..\n"));
    }

  // Wait blisfully till we are woken up
  this->event_.wait ();

  return 0;
}

/*******************************************************************/

Worker_Task::Worker_Task (Message_Queue *mq,
                          Synchronisers &synch)
  : ACE_Task<ACE_MT_SYNCH> (nullptr, mq),
  synch_ (synch),
  messages_processed_ (0)
{
}

int
Worker_Task::svc ()
{
  // Start synchronization
  (void) this->synch_.start_synchronization ();

  for (;;)
    {
      ACE_Message_Block *mb {};
      int const getq_result = this->getq (mb);
      if (getq_result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Worker_Task::svc (%t) -> %p\n",
                             "getq error"),
                            -1);
        }

      // Get the flag in the message blok
      ACE_Message_Block::Message_Flags const flag =
        mb->self_flags ();

      // Check for the stop flag
      bool const stop_flag = ACE_BIT_ENABLED (flag,
                                              Synchronisers::MB_STOP_FLAG);
      if (stop_flag)
        {
          if (debug != DEBUGGING_RANGE::DEBUG_NONE)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) saw flag after [%B] messages\n",
                          this->messages_processed_));
            }

        }
      // Release the message block
      ACE_Message_Block_Unique_Ptr input_owner {mb};
      input_owner.reset ();

      // Counter.
      ++this->messages_processed_;

      if (debug != DEBUGGING_RANGE::DEBUG_NONE)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) dequeued my %B message\n",
                      this->messages_processed_));
        }

      //
      // Process message here.
      //

      for (size_t j = 0; j < message_size; ++j)
        {
          // Eat a little CPU
          /* takes about 40.2 usecs on a 167 MHz Ultra2 */
          u_long const n = 11UL;
          ACE::is_prime (n, 2, n / 2);
        }

      // Make a message block for writing onto output queue
      ACE_Message_Block *message_block {};
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (data_block),
                      -1);
      ACE_Message_Block_Unique_Ptr output_owner {message_block};

      // Put this message block into the next queue or the output
      // queue
      int const put_next_result = this->put_next (output_owner.get ());

      if (put_next_result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Input::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
        }
      output_owner.release ();

      // If the stop_flag is set just break and wait..
      if (stop_flag)
        {
          if (debug != DEBUGGING_RANGE::DEBUG_NONE)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Got stop message after [%B] messages\n",
                        this->messages_processed_));

          break;
        }
    }

  (void) this->synch_.end_synchronization ();
  return 0;
}

size_t
Worker_Task::processed () const
{
  return this->messages_processed_;
}

/*******************************************************************/

Input_Task::Input_Task (Message_Queue *mq,
                        Synchronisers &synch)
  : ACE_Task<ACE_MT_SYNCH> (nullptr, mq),
    synch_ (synch)
{
}

int
Input_Task::svc ()
{
  // Synchronise threads
  (void) this->synch_.start_synchronization ();


  size_t i {};
  for (i = 0;
       i < (number_of_messages - number_of_workers);
       ++i)
    {
      // Make a message block
      ACE_Message_Block *message_block {};
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (data_block),
                      -1);

      ACE_Message_Block_Unique_Ptr message_owner {message_block};
      int const result = this->putq (message_owner.get ());

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Input::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
        }
      message_owner.release ();
      if (debug != DEBUGGING_RANGE::DEBUG_NONE)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) Input thread ->  Sent [%B] messages\n",
                      i));
        }
    }

  if (debug != DEBUGGING_RANGE::DEBUG_NONE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Sending close messages\n"));
    }


  // Stop messages
  for (i = 0;
       i < number_of_workers;
       ++i)
    {
      // Make a message block
      ACE_Message_Block *message_block {};
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (data_block),
                      -1);
      ACE_Message_Block_Unique_Ptr message_owner {message_block};

      // Set the stop flag in the message block and not in the datablock
      message_block->set_self_flags (Synchronisers::MB_STOP_FLAG);

      int const result = this->putq (message_owner.get ());

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Input::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
        }
      message_owner.release ();
    }


  (void) this->synch_.end_synchronization ();
  return 0;
}

/*******************************************************************/

Output_Task::Output_Task (Message_Queue *mq,
                          Synchronisers &synch)
  : ACE_Task<ACE_MT_SYNCH> (nullptr, mq),
    synch_ (synch)
{
}

int
Output_Task::svc ()
{
  // Synchronise threads
  (void) this->synch_.start_synchronization ();

  for (size_t i = 0;
       i < number_of_messages;
       ++i)
    {
      // Get the message block from queue
      ACE_Message_Block *mb {};
      int const result = this->getq (mb);

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Output::svc (%t) -> %p\n",
                             "getq error"),
                            -1);
        }

      ACE_Message_Block_Unique_Ptr message_owner {mb};

      if (debug != DEBUGGING_RANGE::DEBUG_NONE)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) Output thread ->  received [%B] message\n",
                      i));
        }
    }

  (void) this->synch_.end_synchronization ();
  return 0;
}

int
Output_Task::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  /*  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Sticking message into "
                  " output queue\n"));
                  }*/
  return this->putq (mb);
}

/*******************************************************************/

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("m:s:w:t:d:"));
  int c {};

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'm':
          if (!parse_size (get_opt.opt_arg (), number_of_messages)
              || number_of_messages == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR, "Message count must be positive\n"), -1);
            }
          break;
        case 'w':
        case 't':
          if (!parse_size (get_opt.opt_arg (), number_of_workers)
              || number_of_workers == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR, "Worker count must be positive\n"), -1);
            }
          break;
        case 'd':
          {
            size_t parsed_debug {};
            if (!parse_size (get_opt.opt_arg (), parsed_debug)
                || parsed_debug > static_cast<size_t> (DEBUGGING_RANGE::DEBUG))
              {
                ACE_ERROR_RETURN ((LM_ERROR, "Invalid debug level\n"), -1);
              }
            debug = static_cast<DEBUGGING_RANGE> (parsed_debug);
          }
          break;
        case 's':
          if (!parse_size (get_opt.opt_arg (), message_size))
            {
              ACE_ERROR_RETURN ((LM_ERROR, "Invalid message size\n"), -1);
            }
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s\n"
                             "\t[-m number of messages]\n"
                             "\t[-s message size]\n"
                             "\t[-w number of workers (or legacy -t)]\n"
                             "\t[-d debug]\n",
                             argv[0]),
                            -1);
        }
    }

  return 0;
}


/*******************************************************************/
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  if (number_of_messages < number_of_workers)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Message count must be at least the worker count\n"),
                        -1);
    }

  if (number_of_messages > std::numeric_limits<size_t>::max () / 2)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Message count is too large\n"), -1);
    }

  ACE_High_Res_Timer::calibrate ();

  // Create the message queue
  Message_Queue input_message_queue;
  Message_Queue output_message_queue;

  // Create the datablocks. IF we use the default Message Blocks Ctor,
  // it is going to do an extra allocation for the data block
  ACE_NEW_RETURN (data_block,
                  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >,
                  -1);

  // Increment the reference count so that we can share the
  // datablock. This is donw twice the number of messages for the
  // input and output queues.
  size_t i {};

  for (i = 0; i < 2 * number_of_messages; ++i)
    {
      data_block->duplicate ();
    }

  // Create the Synchronisers
  Synchronisers synch;

  // Workers.
  // Keep the task array manual: workers are activated into a fixed-size
  // synchronization barrier and can still reference their task on failures.
  Worker_Task **workers {};
  ACE_NEW_RETURN (workers,
                  Worker_Task *[number_of_workers],
                  -1);

  // Input Task
  Input_Task input_task (&input_message_queue,
                         synch);

  // Output Task
  Output_Task output_task (&output_message_queue,
                           synch);
  int priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);


  long flags = THR_SCHED_FIFO | THR_SCOPE_PROCESS;

  // Create and activate the worker threads
  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_NEW_RETURN (workers[i],
                      Worker_Task (&input_message_queue, synch),
                      -1);

      workers[i]->next (&output_task);

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


  // Activate the input and output threads
  result = input_task.activate (flags,
                                1,
                                1,
                                priority);

  if (result != 0)
    return result;


  // Activate the workers.
  result = output_task.activate (flags,
                                 1,
                                 1,
                                 priority);

  if (result != 0)
    return result;


  // Wait for all threads to terminate.
  result = ACE_Thread_Manager::instance ()->wait ();


  ACE_hrtime_t elapsed_time {};

  test_timer.elapsed_time (elapsed_time);

# if !defined (ACE_WIN32)
  double const elapsed_time_per_invocation =
    static_cast<double> (elapsed_time) / number_of_messages;

  ACE_DEBUG ((LM_DEBUG,
     "(%P|%t) Throughput is [%f]\n",
     elapsed_time_per_invocation));

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Throughput is [%f]\n",
              1000000000/ elapsed_time_per_invocation));

#endif /*ACE_WIN32 */
  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Message process for thread [%B] is [%B]\n",
                  i, workers[i]->processed ()));
      delete workers[i];
    }
  delete[] workers;

  if (result == 0)
    {
      delete data_block;
      data_block = nullptr;
    }

  return result;
}

#else /*ACE_HAS_THREADS*/

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Not supported in single threaded builds\n"));

  return 0;
}


#endif /*ACE_HAS_THREADS*/
