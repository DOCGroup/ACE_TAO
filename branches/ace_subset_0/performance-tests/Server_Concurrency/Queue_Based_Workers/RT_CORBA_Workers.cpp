// $Id$
#include "RT_CORBA_Workers.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"


// The number of messages that is being processed
static size_t number_of_messages = 100;

// The number of upcall threads
static size_t number_of_workers = 2;

// The size of the message
static size_t message_size = 100;

// Number of threads that are ready to go
static size_t ready_threads = 0;

// Number of input and output threads
static size_t io_threads = 2; // 1 for output and 1 for input

// High resolution test timer
static ACE_High_Res_Timer test_timer;

// Debugging condition
static DEBUGGING_RANGE debug = DEBUG_NONE;

// Data block used by the message blocks
ACE_Data_Block *data_block = 0;

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
                  "(%P|%t) Ready to wait () on event.. \n"));
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
        mb->self_flags ();

      // The stop flag
      int stop_flag = 0;

      // Check for the stop flag
      if (ACE_BIT_ENABLED (flag,
                           Synchronisers::MB_STOP_FLAG))
        {
          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) saw flag after [%d] messages\n",
                          this->messages_processed_));
            }

          stop_flag = 1;
        }
      // Release the message block
      mb->release ();

      // Counter.
      ++this->messages_processed_;

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) dequeued my %d message\n",
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

      // Make a message block for writing onto output queue
      ACE_Message_Block *message_block = 0;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (data_block),
                      -1);

      // Put this message block into the next queue or the output
      // queue
      result = this->put_next (message_block);

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Input::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
                            }

      // If the stop_flag is set just break and wait..
      if (stop_flag)
        {
          if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) Got stop message after [%d] messages \n",
                        this->messages_processed_));

          break;
        }
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

Input_Task::Input_Task (Message_Queue *mq,
                        Synchronisers &synch)
  : ACE_Task<ACE_MT_SYNCH> (0, mq),
    synch_ (synch)
{
}

int
Input_Task::svc (void)
{
  // Synchronise threads
  (void) this->synch_.start_synchronization ();


  size_t i = 0;
  for (i = 0;
       i < (number_of_messages - number_of_workers);
       ++i)
    {
      // Make a message block
      ACE_Message_Block *message_block = 0;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (data_block),
                      -1);

      int result = this->putq (message_block);

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Input::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) Input thread ->  Sent [%d] messages\n",
                      i));
        }
    }

  if (debug)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Sending close messages \n"));
    }


  // Stop messages
  for (i = 0;
       i < number_of_workers;
       ++i)
    {
      // Make a message block
      ACE_Message_Block *message_block = 0;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (data_block),
                      -1);

      // Set the stop flag in the message block and not in the datablock
      message_block->set_self_flags (Synchronisers::MB_STOP_FLAG);

      int result = this->putq (message_block);

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Input::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
        }
    }


  (void) this->synch_.end_synchronization ();
  return 0;
}

/*******************************************************************/

Output_Task::Output_Task (Message_Queue *mq,
                          Synchronisers &synch)
  : ACE_Task<ACE_MT_SYNCH> (0, mq),
    synch_ (synch)
{
}

int
Output_Task::svc (void)
{
  // Synchronise threads
  (void) this->synch_.start_synchronization ();


  for (size_t i = 0;
       i < number_of_messages;
       ++i)
    {
      // Get the message block from queue
      ACE_Message_Block *mb = 0;
      int result = this->getq (mb);

      // delete the message block
      mb->release ();

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) Output thread ->  received [%d] message\n",
                      i));
                      }

      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Input::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
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
                  " output queue \n"));
                  }*/
  return this->putq (mb);
}

/*******************************************************************/

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("m:s:t:d:"));
  int c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'm':
          number_of_messages = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 't':
          number_of_workers = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          debug = ACE_static_cast (DEBUGGING_RANGE, ACE_OS::atoi (get_opt.opt_arg ()));
          break;
        case 's':
          message_size = ACE_OS::atoi (get_opt.opt_arg ());
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
  size_t i = 0;

  for (i = 0; i < 2*number_of_messages; ++i)
    {
      data_block->duplicate ();
    }

  // Create the Synchronisers
  Synchronisers synch;

  // Workers.
  Worker_Task **workers = 0;
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


  ACE_hrtime_t elapsed_time = 0;

  test_timer.elapsed_time (elapsed_time);

# if !defined (ACE_WIN32)
  double elapsed_time_per_invocation =
    (double) elapsed_time / number_of_messages;

  ACE_DEBUG ((LM_DEBUG,
     "(%P|%t) Throughput is [%f] \n",
     elapsed_time_per_invocation)); 

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Throughput is [%f] \n",
              1000000000/ elapsed_time_per_invocation));

#endif /*ACE_WIN32 */
  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Message process for thread [%d] is [%d] \n",
                  i, workers[i]->processed ()));
      delete workers[i];
    }
  delete[] workers;

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
