// $Id$
#include "RT_CORBA_Workers.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"


// The number of messages that is being processed
static size_t number_of_messages = 1000;

// The number of upcall threads
static size_t number_of_workers = 4;

// The size of the message
static size_t message_size = 100;

// Number of threads that are ready to go
static size_t ready_threads = 0;

// Number of IO threads
static size_t io_threads = 1;

// High resolution test timer
static ACE_High_Res_Timer test_timer;

// Debugging condition
static DEBUGGING_RANGE debug = DEBUG_NONE;

/*******************************************************************/
// Constructor for Synchronisers
Synchronisers::Synchronisers (void)
  : mutex_ (),
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

    if (ready_threads == (number_of_workers + io_threads))
      {
        // Reset the ready_threads so that we can wait at the end of
        // runs
        ready_threads = 0;

        if (debug)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Ready to signal start \n"));
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

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Ready to go.. \n"));
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

    if (ready_threads == (number_of_workers + io_threads))
      {
        // Reset the ready_threads so that we can wait at the end of
        // runs
        ready_threads = 0;

        // Start the timer
        test_timer.stop ();

        // Signal all the threads
        this->event_.signal ();

        if (debug)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Ended peacefully \n"));
          }

        // return to do our work;
        return 0;
      }


    // If we are not the last thread, let go off the lock
  }

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Going to wait .. \n"));
    }

  // Wait blisfully till we are woken up
  this->event_.wait ();

  return 0;
}

/*******************************************************************/

Worker_Task::Worker_Task (Message_Queue *mq,
                          Synchronisers &synch)
  : ACE_Task<ACE_MT_SYNCH> (0, mq),
  synch_ (synch),
  messages_processed_ (0)
{
}

int
Worker_Task::svc (void)
{
  // Start synchronization
  (void) this->synch_.start_synchronization ();

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

      // Get the flag in the message blok
      ACE_Message_Block::Message_Flags flag =
        mb->flags ();

      // The stop flag
      int stop_flag = 0;

      // Check for the stop flag
      if (ACE_BIT_ENABLED (flag,
                           Synchronisers::MB_STOP_FLAG))
        {
          if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Got stop message after [%d] messages \n",
                        this->messages_processed_));
          stop_flag = 1;
        }
      // Release the message block
      mb->release ();

      // Counter.
      ++this->messages_processed_;

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) dequeued its %d message\n",
                      this->messages_processed_));
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

      // If the stop_flag is set just break and wait..
      if (stop_flag)
        break;
    }

  (void) this->synch_.end_synchronization ();
  return 0;
}

int
Worker_Task::processed (void)
{
  return this->messages_processed_;
}

/*******************************************************************/

IO_Task::IO_Task (Message_Queue *mq,
                  Synchronisers &synch)
  : ACE_Task<ACE_MT_SYNCH> (0, mq),
    synch_ (synch)
{
}

int
IO_Task::svc (void)
{
  // Synchronise threads
  (void) this->synch_.start_synchronization ();


  for (size_t i = 0;
       i < (number_of_messages - number_of_workers);
       ++i)
    {
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) IO thread ->  overall message %d\n",
                      i));
        }

      // Make a message block
      ACE_Message_Block *message_block = 0;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (),
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

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Sending close messages \n"));
    }

  // Stop messages
  for (size_t i = 0;
       i < number_of_workers;
       ++i)
    {
      // Make a message block
      ACE_Message_Block *message_block = 0;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (),
                          -1);

      // Set the stop flag
      message_block->set_flags (Synchronisers::MB_STOP_FLAG);

      int result = this->putq (message_block);

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "IO::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
        }
    }


  (void) this->synch_.end_synchronization ();
  return 0;
}


/*******************************************************************/

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("m:s:t:d:i:"));
  int c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'm':
          number_of_messages = ACE_OS::atoi (get_opt.optarg);
          break;
        case 't':
          number_of_workers = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'd':
          debug = ACE_static_cast (DEBUGGING_RANGE, ACE_OS::atoi (get_opt.optarg));
          break;
        case 's':
          message_size = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'i':
          io_threads = ACE_OS::atoi (get_opt.optarg);
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


/*******************************************************************/
int
main (int argc, ACE_TCHAR *argv[])
{
  int result = parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  ACE_High_Res_Timer::calibrate ();

  // Create the message queue
  Message_Queue message_queue;

  // Create the Synchronisers
  Synchronisers synch;

  // Workers.
  Worker_Task **workers = 0;
  ACE_NEW_RETURN (workers,
                  Worker_Task *[number_of_workers],
                  -1);


  int priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);


  long flags = THR_SCHED_FIFO | THR_SCOPE_PROCESS;

  size_t i = 0;
  // Create and activate the worker threads
  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_NEW_RETURN (workers[i],
                      Worker_Task (&message_queue, synch),
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

  // Workers.
  IO_Task **io_task = 0;
  ACE_NEW_RETURN (io_task,
                  IO_Task *[io_threads],
                  -1);


  // Create and activate the worker threads
  for (i = 0; i < io_threads; ++i)
    {
      ACE_NEW_RETURN (io_task[i],
                      IO_Task (&message_queue, synch),
                      -1);

      // Activate the workers.
      result = io_task[i]->activate (flags,
                                     1,
                                     1,
                                     priority);

      if (result != 0)
        return result;
    }


  // Wait for all threads to terminate.
  result = ACE_Thread_Manager::instance ()->wait ();


  ACE_hrtime_t elapsed_time = 0;

  test_timer.elapsed_time (elapsed_time);

  double elapsed_time_per_invocation =
    (double) elapsed_time / number_of_messages;

  /* ACE_DEBUG ((LM_DEBUG,
     "(%P|%t) Throughput is [%f] \n",
     elapsed_time_per_invocation)); */

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Throughput is [%f] \n",
              1000000000/ elapsed_time_per_invocation));

  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Message process for thread [%d] is [%d] \n",
                  i, workers[i]->processed ()));
      delete workers[i];
    }
  delete[] workers;

  for (i = 0; i < io_threads; ++i)
    {

      delete io_task[i];
    }
  delete[] io_task;

  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >;
template class ACE_Lock_Adapter<ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else /*ACE_HAS_THREADS*/

int
main (int, char *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Not supported in single threaded builds \n"));

  return 0;
}


#endif /*ACE_HAS_THREADS*/
