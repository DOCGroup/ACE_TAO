// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Thread_Bounded_Packet_Relay.cpp
//
// = DESCRIPTION
//    Method definitions for the threaded-bounded packet relay class.
//
// = AUTHORS
//    Chris Gill           <cdgill@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu>
//
//    Based on the Timer Queue Test example written by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"
#include "Thread_Bounded_Packet_Relay.h"

typedef Thread_Bounded_Packet_Relay_Driver::COMMAND CMD;

ACE_RCSID(Bounded_Packet_Relay, Thread_Bounded_Packet_Relay, "$Id$")

// Constructor.

Text_Input_Device_Wrapper::Text_Input_Device_Wrapper (ACE_Thread_Manager *input_task_mgr,
                                                      size_t read_length, 
                                                      const char* text)
  : Input_Device_Wrapper_Base (input_task_mgr),
    read_length_ (read_length),
    text_ (text),
    index_ (0)
{
}

// Destructor.

Text_Input_Device_Wrapper::~Text_Input_Device_Wrapper (void)
{
}

// Creates a new message block, carrying data
// read from the underlying input device.

ACE_Message_Block *
Text_Input_Device_Wrapper::create_input_message (void)
{

  // Construct a new message block to send.
  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb, 
                  ACE_Message_Block (read_length_), 
                  0);

  // Zero out a "read" buffer to hold data.
  char read_buf [BUFSIZ];
  ACE_OS::memset (read_buf, 0, BUFSIZ);

  // Loop through the text, filling in data to copy into the read
  // buffer (leaving room for a terminating zero).
  for (size_t i = 0; i < read_length_ - 1; ++i)
    {
      read_buf [i] = text_ [index_];
      index_ = (index_ + 1) % ACE_OS::strlen (text_);
    }

  // Copy buf into the Message_Block and update the wr_ptr ().
  if (mb->copy (read_buf, read_length_) < 0) 
    {
      delete mb;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "read buffer copy failed"),
                        0);
    }
  return mb;
}

// Constructor.

Text_Output_Device_Wrapper::Text_Output_Device_Wrapper (int logging)
  : logging_ (logging)
{
}

// Consume and possibly print out the passed message.

int 
Text_Output_Device_Wrapper::write_output_message (void *message)
{
  if (message)
    {
      if (logging_)
	  {
          ACE_DEBUG ((LM_DEBUG, "%s", 
	      ACE_static_cast (ACE_Message_Block *, message)->
		    rd_ptr ()));
	  }

      delete ACE_static_cast (ACE_Message_Block *, message);
	  return 0;
    }

  ACE_ERROR_RETURN ((LM_ERROR,
                     "Text_Output_Device_Wrapper::"
                     "write_output_message: null argument"), -1);
}

// Modifies device settings based on passed pointer to a u_long turns
// logging on if u_long is non-zero, off if u_long is zero, and does
// nothing if the pointer is null.

int 
Text_Output_Device_Wrapper::modify_device_settings (void *logging)
{
  if (logging)
    logging_ = *ACE_static_cast (int *, logging);
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "null logging level pointer"),
                      -1);
  return 0;
}

// Constructor.

User_Input_Task::User_Input_Task (Bounded_Packet_Relay<ACE_MT_SYNCH> *relay,
                                  Thread_Timer_Queue *queue,
                                  Thread_Bounded_Packet_Relay_Driver &tbprd)
  : ACE_Task_Base (ACE_Thread_Manager::instance ()),
    relay_ (relay),
    queue_ (queue),
    usecs_ (ACE_ONE_SECOND_IN_USECS),
    driver_ (tbprd)
{
}

// Runs the main event loop.

int 
User_Input_Task::svc (void)
{
  for (;;)
    // Call back to the driver's implementation of how to read and
    // parse input.
    if (this->driver_.get_next_request () == -1)
      break;

  // We are done.
  this->relay_->end_transmission (Bounded_Packet_Relay_Base::CANCELLED);
  this->queue_->deactivate ();
  ACE_DEBUG ((LM_DEBUG,
              "terminating user input thread\n"));
  return 0;
} 

// Sets the number of packets for the next transmission.

int 
User_Input_Task::set_packet_count (void *argument)
{
  if (argument)
    {
      driver_.packet_count (*ACE_static_cast (int *, argument));
      return 0;
    }
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_packet_count: null argument"), 
                    -1);
}

// Sets the input device packet arrival period (usecs) for the next
// transmission.

int 
User_Input_Task::set_arrival_period (void *argument)
{
  if (argument)
    {
      driver_.arrival_period (*ACE_static_cast (int *, argument));
      return 0;
    }
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_arrival_period: null argument"), 
                    -1);
}

// Sets the period between output device sends (usecs) for the next
// transmission.

int 
User_Input_Task::set_send_period (void *argument)
{
  if (argument)
    {
      driver_.send_period (*ACE_static_cast (int *, argument));
      return 0;
    }
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_send_period: null argument"), 
                    -1);
}

// Sets a limit on the transmission duration (usecs).

int 
User_Input_Task::set_duration_limit (void *argument)
{
  if (argument)
    {
      driver_.duration_limit (*ACE_static_cast (int *, argument));
      return 0;
    }
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_duration_limit: null argument"), 
                    -1);
}

// Sets logging level (0 or 1) for output device for the next
// transmission.

int 
User_Input_Task::set_logging_level (void *argument)
{
  if (argument)
    {
      driver_.logging_level (*ACE_static_cast (int *, argument));
      return 0;
    }  
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_logging_level: null argument"), 
                    -1);
}

// Runs the next transmission (if one is not in progress).

int 
User_Input_Task::run_transmission (void *)
{
  if (relay_)
    {
      switch (relay_->start_transmission (driver_.packet_count (),
                                          driver_.arrival_period (),
                                          driver_.logging_level ()))
        {
          case 1:
            ACE_DEBUG ((LM_DEBUG, 
                       "\nRun transmission: "
                       "transmission already in progress\n"));
            return 0;
            /* NOTREACHED */
          case 0:
            {
              ACE_Time_Value now = ACE_OS::gettimeofday ();
              ACE_Time_Value send_every (0, driver_.send_period ());
              ACE_Time_Value send_at (send_every + now);

              Send_Handler *send_handler;
              ACE_NEW_RETURN (send_handler, 
                              Send_Handler (driver_.packet_count (), 
                                            send_every,
                                            *relay_,
                                            *queue_), 
                              -1);
              if (queue_->schedule (send_handler, 0, send_at) < 0)
                ACE_ERROR_RETURN ((LM_ERROR, 
                                   "User_Input_Task::run_transmission: "
                                   "failed to schedule send handler"), 
                                  -1);
              if (driver_.duration_limit ())
                {
                  ACE_Time_Value terminate_at (0, driver_.duration_limit ());
                  terminate_at += now;

                  Termination_Handler *termination_handler;

                  ACE_NEW_RETURN (termination_handler, 
                                  Termination_Handler (*relay_,
                                                       *queue_), 
                                  -1);
                  if (queue_->schedule (termination_handler, 
                                        0, terminate_at) < 0)
                    ACE_ERROR_RETURN ((LM_ERROR, 
                                       "User_Input_Task::run_transmission: "
                                       "failed to schedule termination "
                                       "handler"), 
                                      -1);
                }
              return 0;
            }
            /* NOTREACHED */
          default:
            return -1;
            /* NOTREACHED */
        }  
    } 
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::run_transmission: "
                     "relay not instantiated"), 
                    -1);
}

// Ends the current transmission (if one is in progress).

int 
User_Input_Task::end_transmission (void *)
{
  if (relay_)
    {
      switch (relay_->end_transmission (Bounded_Packet_Relay_Base::CANCELLED))
        {
          case 1:
            ACE_DEBUG ((LM_DEBUG, 
                        "\nEnd transmission: "
                        "no transmission in progress\n"));
            return 0;
            /* NOTREACHED */
          case 0: 
            // Cancel any remaining timers.
            this->clear_all_timers ();
            return 0;
            /* NOTREACHED */
          default:
            return -1;
            /* NOTREACHED */
        }
    }
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::end_transmission: "
                     "relay not instantiated"), 
                    -1);
}

// Reports statistics for the previous transmission 
// (if one is not in progress).

int 
User_Input_Task::report_stats (void *)
{
  if (relay_)
    {
      switch (relay_->report_statistics ())
        {
          case 1:
            ACE_DEBUG ((LM_DEBUG, 
                        "\nRun transmission: "
                        "\transmission already in progress\n"));
             /* fall through to next case */

          case 0:
            return 0;
             /* NOTREACHED */

          default:
            return -1;
             /* NOTREACHED */
        }
    }
  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::report_stats: "
                     "relay not instantiated"), 
                    -1);
}

// Shut down the task.

int 
User_Input_Task::shutdown (void *)
{
#if !defined (ACE_LACKS_PTHREAD_CANCEL)
  // Cancel the thread timer queue task "preemptively."
  ACE_Thread::cancel (this->queue_->thr_id ());
#else
  // Cancel the thread timer queue task "voluntarily."
  this->queue_->deactivate ();
#endif /* ACE_LACKS_PTHREAD_CANCEL */

  // -1 indicates we are shutting down the application.
  return -1;
}

// Helper method: clears all timers.

int
User_Input_Task::clear_all_timers (void)
{
  // loop through the timers in the queue, cancelling each one
  for (ACE_Timer_Node_T <ACE_Event_Handler *> *node;
       (node = queue_->timer_queue ().get_first ()) != 0;
       )
    queue_->cancel (node->get_timer_id (), 0);

  return 0;
}

// Constructor.

BPR_Handler_Base::BPR_Handler_Base (Bounded_Packet_Relay<ACE_MT_SYNCH> &relay,
                                    Thread_Timer_Queue &queue)
  : relay_ (relay),
    queue_ (queue)
{
}

// Destructor.

BPR_Handler_Base::~BPR_Handler_Base (void)
{
}

// Helper method: clears all timers.

int
BPR_Handler_Base::clear_all_timers (void)
{
  // Loop through the timers in the queue, cancelling each one.

  for (ACE_Timer_Node_T <ACE_Event_Handler *> *node;
       (node = queue_.timer_queue ().get_first ()) != 0;
       )
    queue_.cancel (node->get_timer_id (), 0);

  return 0;
}

// Constructor.

Send_Handler::Send_Handler (u_long send_count, 
                            const ACE_Time_Value &duration,
                            Bounded_Packet_Relay<ACE_MT_SYNCH> &relay,
                            Thread_Timer_Queue &queue)
  : BPR_Handler_Base (relay, queue),
    send_count_ (send_count),
    duration_ (duration)
{
}

// Destructor.

Send_Handler::~Send_Handler (void)
{
}

// Call back hook.

int 
Send_Handler::handle_timeout (const ACE_Time_Value &current_time,
			      const void *arg)
{
  switch (relay_.send_input ())
    {
      case 0:
        // Decrement count of packets to relay.
        --send_count_;
        /* Fall through to next case. */
      case 1:
        if (send_count_ > 0)
          {
            // Re-register the handler for a new timeout.
            if (queue_.schedule (this,
                                 0, 
                                 duration_ + ACE_OS::gettimeofday ()) < 0)
              ACE_ERROR_RETURN ((LM_ERROR, 
                                 "Send_Handler::handle_timeout: "
                                 "failed to reschedule send handler"), 
                                -1);
            return 0;
          }
        else
          {
            // All packets are sent, time to cancel any other timers,
            // end the transmission, and go away.
            this->clear_all_timers ();
            relay_.end_transmission (Bounded_Packet_Relay_Base::COMPLETED);
            delete this;
            return 0;
          }
        /* NOTREACHED */
      default:
        return -1;
    }
}

// Cancellation hook.

int 
Send_Handler::cancelled (void)
{
  delete this;
  return 0;
}

// Constructor.

Termination_Handler::Termination_Handler (Bounded_Packet_Relay<ACE_MT_SYNCH> &relay,
                                          Thread_Timer_Queue &queue)
  : BPR_Handler_Base (relay, queue)
{
}

// Destructor.

Termination_Handler::~Termination_Handler (void)
{
}

// Call back hook.

int 
Termination_Handler::handle_timeout (const ACE_Time_Value &current_time,
                                     const void *arg)
{
  // Transmission timed out, so cancel any other
  // timers, end the transmission, and go away.
  this->clear_all_timers ();
  relay_.end_transmission (Bounded_Packet_Relay_Base::TIMED_OUT);
  delete this;
  return 0;
}

// Cancellation hook

int 
Termination_Handler::cancelled (void)
{
  delete this;
  return 0;
}

// Constructor.

Thread_Bounded_Packet_Relay_Driver::Thread_Bounded_Packet_Relay_Driver (Bounded_Packet_Relay<ACE_MT_SYNCH> *relay)
  : input_task_ (relay, &timer_queue_, *this)
{
}

// Destructor.

Thread_Bounded_Packet_Relay_Driver::~Thread_Bounded_Packet_Relay_Driver (void)
{
}

// Display the user menu.

int 
Thread_Bounded_Packet_Relay_Driver::display_menu (void)
{
  static char menu[] =
    "\n\n  Options:\n"
    "  ----------------------------------------------------------------------\n"
    "  1 <number of packets to relay in one transmission = %d>\n"
    "     min = 1 packet.\n"
    "  2 <input packet arrival period (in usec) = %d>\n"
    "     min = 1.\n"
    "  3 <output packet transmission period (in usec) = %d>\n"
    "     min = 1.\n"
    "  4 <limit on duration of transmission (in usec) = %d>\n"
    "     min = 1, no limit = 0.\n"
    "  5 <logging level = %d>\n"
    "     no logging = 0, logging = non-zero.\n"
    "  ----------------------------------------------------------------------\n"
    "  6 - run a transmission using the current settings\n"
    "  7 - cancel transmission (if there is one running)\n"
    "  8 - report statistics from the most recent transmission\n"
    "  9 - quit the program\n"
    "  ----------------------------------------------------------------------\n";

  ACE_DEBUG ((LM_DEBUG, 
	          menu, 
	          this->packet_count (),
              this->arrival_period (),
              this->send_period (),
              this->duration_limit (),
              this->logging_level ()));

  return 0;
}

// Initialize the driver.

int 
Thread_Bounded_Packet_Relay_Driver::init (void)
{
  // Initialize the <Command> objects with their corresponding
  // methods from <User_Input_Task>.
  ACE_NEW_RETURN (packet_count_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::set_packet_count),
                  -1);
  ACE_NEW_RETURN (arrival_period_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::set_arrival_period),
                  -1);
  ACE_NEW_RETURN (transmit_period_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::set_send_period),
                  -1);
  ACE_NEW_RETURN (duration_limit_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::set_duration_limit),
                  -1);
  ACE_NEW_RETURN (logging_level_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::set_logging_level),
                  -1);
  ACE_NEW_RETURN (run_transmission_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::run_transmission),
                  -1);
  ACE_NEW_RETURN (cancel_transmission_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::end_transmission),
                  -1);
  ACE_NEW_RETURN (report_stats_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::report_stats),
                  -1);
  ACE_NEW_RETURN (shutdown_cmd_,
                  CMD (input_task_,
                       &User_Input_Task::shutdown),
                  -1);
  if (this->input_task_.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "cannot activate input task"),
                      -1);
  else if (this->timer_queue_.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "cannot activate timer queue"),
                      -1);
  else if (ACE_Thread_Manager::instance ()->wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "wait on Thread_Manager failed"),
                      -1);
  return 0;
}

// Run the driver

int 
Thread_Bounded_Packet_Relay_Driver::run (void)
{
  this->init ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Thread_Timer_Queue_Adapter<Timer_Heap>;
template class Bounded_Packet_Relay_Driver<Thread_Timer_Queue>;
template class Command<User_Input_Task, User_Input_Task::ACTION>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Thread_Timer_Queue_Adapter<Timer_Heap>
#pragma instantiate Bounded_Packet_Relay_Driver<Thread_Timer_Queue>
#pragma instantiate Command<User_Input_Task, User_Input_Task::ACTION>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
// These templates will specialized in libACE.* if the platforms does
// not define ACE_MT_SAFE.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Thread_Condition<ACE_Thread_Mutex>;
template class ACE_Condition<ACE_Thread_Mutex>;
template class ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>;
template class ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>;
template class ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>;
template class ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>;
template class ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Thread_Condition<ACE_Thread_Mutex>
#pragma instantiate ACE_Condition<ACE_Thread_Mutex>
#pragma instantiate ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>
#pragma instantiate ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>
#pragma instantiate ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>
#pragma instantiate ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>
#pragma instantiate ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_MT_SAFE */
