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
//    Chris Gill          <cdgill@cs.wustl.edu>
//
//    Based on the threaded timer queue driver by
//
//    Carlos O'Ryan       <coryan@cs.wustl.edu>
//    Douglas C. Schmidt  <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"
#include "Thread_Bounded_Packet_Relay.h"

ACE_RCSID(Bounded_Packet_Relay, Thread_Bounded_Packet_Relay, "$Id$")

// constructor

template <class RECEIVER, class ACTION>
Command<RECEIVER, ACTION>::Command (RECEIVER &recvr,
                                    ACTION action)
  : receiver_ (recvr),
    action_ (action)
{
}

// invokes an operation.

template <class RECEIVER, class ACTION> int
Command<RECEIVER, ACTION>::execute (void *arg)
{
  return (receiver_.*action_) (arg);
}

Text_Input_Device_Wrapper::Text_Input_Device_Wrapper (ACE_Thread_Manager *input_task_mgr,
                                                      size_t read_length, 
                                                      const char* text)
  : Input_Device_Wrapper_Base (input_task_mgr)
  // @@ Chris, your coding style here is not consistent with the rest of ACE.  this
  // should be 
  // : Input_Device.... (....),
  //   read_length_ (....),
  //   text_ (....), 
  //   index_ (0)
  // Please be follow this in the future.
  , read_length_ (read_length)
  , text_ (text)
  , index_ (0)
{
}
  // ctor

Text_Input_Device_Wrapper::~Text_Input_Device_Wrapper (void)
{
}
  // dtor

ACE_Message_Block *
Text_Input_Device_Wrapper::create_input_message (void)
{
  // @@ Chris, make sure to use the proper capitalization and
  // punctuation for these and all comments...

  // construct a new message block to send
  ACE_Message_Block *message;
  ACE_NEW_RETURN (message, 
                  ACE_Message_Block (read_length_), 
                  0);

  // zero out a "read" buffer to hold data
  char read_buf [BUFSIZ];
  ACE_OS::memset (read_buf, 0, BUFSIZ);

  // loop through the text, filling in data to copy into
  // the read buffer (leaving room for a terminating zero)
  for (size_t i = 0; i < read_length_ - 1; ++i)
    {
      read_buf [i] = text_ [index_];
      index_ = (index_ + 1) % ACE_OS::strlen (text_);
    }

  // Copy buf into the Message_Block and update the wr_ptr ().
  if (mb->copy (read_buf, read_length_) < 0) 
    {
      delete message;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "read buffer copy failed"),
                        0);
    }

  return message;
}

// creates a new message block, carrying data
// read from the underlying input device  

Text_Output_Driver_Wrapper::Text_Output_Driver_Wrapper (int logging)
  : logging_ (logging)
{
}
  // default ctor

int 
Text_Output_Driver_Wrapper::write_output_message (void *message)
{
  ACE_Message_Block *message;
}

// consume and possibly print out the passed message

int 
Text_Output_Driver_Wrapper::modify_device_settings (void *logging)
{
  // @@ Chris, your indentation throughout this file is not consistent
  // with ACE coding guidelines.  Please make sure you use the
  // indentation format that GNU emacs provide.  If you have any
  // questions, please let me know.
  if (logging)
  {
    logging_ = *ACE_static_cast (int *, logging);
  }
  else
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       "null logging level pointer"),
                      -1);
  }
}

// @@ Chris, you are not intenting your comments correctly.  The
// comments before each method should be flush against the left column
// and there should be only one blank line between the comments and
// the method.

  // modifies device settings based on passed pointer to a u_long
  // turns logging on if u_long is non-zero, off if u_long is zero,
  // and does nothing if the pointer is null.


User_Input_Task::User_Input_Task (Thread_Timer_Queue *queue,
                                  Thread_Bounded_Packet_Relay_Driver &tbprd)
  : ACE_Task_Base (ACE_Thread_Manager::instance ()),
    queue_ (queue),
    usecs_ (ACE_ONE_SECOND_IN_USECS),
    driver_ (tbprd)
{
}

  // ctor

int 
User_Input_Task::svc (void)
{
  for (;;)
    // call back to the driver's implementation on how to read and
    // parse input.
    if (this->driver_.get_next_request () == -1)
      break;

  // we are done.
  this->relay_->end_transmission (Bounded_Packet_Relay_Driver::CANCELLED);
  this->queue_->deactivate ();
  ACE_DEBUG ((LM_DEBUG,
              "terminating input thread\n"));
  return 0;
} // @@ Chris, you are putting the comments in the wrong place...
 // They should go BEFORE the method, not after it...  It's only in
 // the header file that they must go after the method...
  // This method runs the event loop in the new thread.

  // = Some helper methods.

int 
User_Input_Task::set_packet_count (void *argument)
{
  if (argument)
  {
    packet_count_ = *ACE_static_cast (int *, argument);
    return 0;
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_packet_count: null argument"), 
                    -1);
}
  // set the number of packets for the next transmission.

int 
User_Input_Task::set_arrival_period (void *argument)
{
  if (argument)
  {
    arrival_period_ = *ACE_static_cast (int *, argument);
    return 0;
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_arrival_period: null argument"), 
                    -1);
}
  // sets the input device packet arrival period (usecs)
  // for the next transmission.

int 
User_Input_Task::set_send_period (void *argument)
{
  if (argument)
  {
    send_period_ = *ACE_static_cast (int *, argument);
    return 0;
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_send_period: null argument"), 
                    -1);
}
  // sets the period between output device sends (usecs)
  // for the next transmission.

int 
User_Input_Task::set_duration_limit (void *argument)
{
  if (argument)
  {
    duration_limit_ = *ACE_static_cast (int *, argument);
    return 0;
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_duration_limit: null argument"), 
                    -1);
}
  // sets a limit on the transmission duration (usecs)

int 
User_Input_Task::set_logging_level (void *argument)
{
  if (argument)
  {
    logging_level_ = *ACE_static_cast (int *, argument);
    return 0;
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::set_logging_level: null argument"), 
                    -1);
}
  // sets logging level (0 or 1) for output device
  // for the next transmission

int 
User_Input_Task::run_transmission (void *argument)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (argument);

  if (relay_)
  {
    switch (relay_->start_transmission (packet_count_,
                                        arrival_period_,
                                        logging_level_))
    {
      case 1:
        // @@ Chris, please don't use ACE_OS::fprintf() for things
        // like this, use ACE_DEBUG or ACE_ERROR instead.
        ACE_OS::fprintf (ACE_STDERR, 
                         "\nRun transmission: "
                         "transmission already in progress\n");
        return 0;
        /* NOT REACHED */
      case 0:
      {
        ACE_Time_Value now = ACE_OS::gettimeofday ();
        ACE_Time_Value send_every (0, send_period_);
        ACE_Time_Value send_at (send_every + now);

        Send_Handler *send_handler;

        ACE_NEW_RETURN (send_handler, 
                        Send_Handler (packet_count_, 
                                      send_every,
                                      *relay_,
                                      *queue_), 
                        -1);

        if (queue_->schedule (send_handler, 0, send_at) < 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, 
                             "User_Input_Task::run_transmission: "
                             "failed to schedule send handler"), 
                            -1);
        }

        if (duration_limit_)
        {
          ACE_Time_Value terminate_at (0, duration_limit_);
          terminate_at += now;

          Termination_Handler *termination_handler;

          ACE_NEW_RETURN (termination_handler, 
                          Termination_Handler (*relay_,
                                               *queue_), 
                          -1);

          if (queue_->schedule (termination_handler, 0, terminate_at) < 0)
          {
            ACE_ERROR_RETURN ((LM_ERROR, 
                               "User_Input_Task::run_transmission: "
                               "failed to schedule termination handler"), 
                              -1);
          }
        }

        return 0;
      }
         /* not reached */

      default:
        return -1;
         /* not reached */
    }
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::run_transmission: "
                     "relay not instantiated"), 
                    -1);
}
  // runs the next transmission (if one is not in progress)

int 
User_Input_Task::end_transmission (void *argument)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (argument);

  if (relay_)
  {
    Bounded_Packet_Relay::Transmission_Status *status;

    status = 
      ACE_static_cast (Bounded_Packet_Relay::Transmission_Status *,
                       argument);

    if (status)
    {
      switch (relay_->end_transmission (*status)
      {
        case 1:
          ACE_OS::fprintf (ACE_STDERR, 
                           "\nEnd transmission: "
                           "no transmission in progress\n");
         return 0;
           /* not reached */

        case 0: 
          // cancel any remaining timers

          for (ACE_Timer_Node_T <ACE_Event_Handler *> *node;
               (node = queue_->timer_queue ().get_first ()) != 0;
               )
            queue->cancel (node->get_timer_id (), 0);

          return 0;
           /* not reached */
  
        default:
          return -1;
           /* not reached */
      }
    }

    ACE_ERROR_RETURN ((LM_ERROR, 
                       "User_Input_Task::end_transmission: "
                       "null argument"), 
                      -1);
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::end_transmission: "
                     "relay not instantiated"), 
                    -1);
}
  // ends the current transmission (if one is in progress)

int 
User_Input_Task::report_stats (void *argument)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (argument);

  if (relay_)
  {
    switch (relay_->report_statistics ())
    {
      case 1:
        ACE_OS::fprintf (ACE_STDERR, 
                         "\nRun transmission: "
                         "\transmission already in progress\n");
         /* fall through to next case */

      case 0:
        return 0;
         /* not reached */

      default:
        return -1;
         /* not reached */
    }
  }

  ACE_ERROR_RETURN ((LM_ERROR, 
                     "User_Input_Task::report_stats: "
                     "relay not instantiated"), 
                    -1);
}
  // reports statistics for the previous transmission 
  // (if one is not in progress)

int 
User_Input_Task::shutdown (void *argument)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (argument);

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
  // Shutdown task.

Send_Handler::Send_Handler (u_long send_count, 
                            const ACE_Time_Value &duration,
                            Bounded_Packet_Relay<ACE_Thread_Mutex> &relay,
                            Thread_Timer_Queue &queue)
  // @@ Chris, please fix the formatting here...
  : send_count_ (send_count)
  , duration_ (duration)
  , relay_ (relay)
  , queue_ (queue)
{
}
  // ctor

Send_Handler::~Send_Handler (void)
{
}
  // dtor

int 
Send_Handler::handle_timeout (const ACE_Time_Value &current_time,
			      const void *arg)
{
  switch (relay_->send_input ())
    {
    case 0:
      // decrement count of packets to relay
      --send_count_;
      /* fall through to next case */
    case 1:
      if (send_count_ > 0)
        {
          // re-register the handler for a new timeout
          if (queue_->schedule (this, 0, 
                                duration_ + ACE_OS::gettimeofday ()) < 0)
            ACE_ERROR_RETURN ((LM_ERROR, 
                               "Send_Handler::handle_timeout: "
                               "failed to reschedule send handler"), 
                              -1);
          return 0;
        }
      else
        {
          // @@ Chris, I think you repeat this code a number of times.
          // Can you please abstract it out into a helper method and
          // call it, rather than writing it redundantly?

          // all packets are sent, time to cancel any other
          // timers, end the transmission, and go away
          for (ACE_Timer_Node_T <ACE_Event_Handler *> *node;
               (node = queue_->timer_queue ().get_first ()) != 0;
               )
            queue->cancel (node->get_timer_id (), 0);

          relay_->end_transmission (Bounded_Packet_Relay::COMPLETED);
          delete this;
          return 0;
        }
      /* NOT REACHED */
    default:
      return -1;
    }
}
  // Call back hook.

int 
Send_Handler::cancelled (void)
{
  delete this;
  return 0;
}
  // Cancellation hook

Termination_Handler::Termination_Handler (Bounded_Packet_Relay<ACE_Thread_Mutex> &relay,
                                          Thread_Timer_Queue &queue)
  : relay_ (relay)
  , queue_ (queue)
{
}
  // ctor

Termination_Handler::~Termination_Handler (void)
{
}
  // dtor

int 
Termination_Handler::handle_timeout (const ACE_Time_Value &current_time,
                                     const void *arg)
{
  // transmission timed out, cancel any other
  // timers, end the transmission, and go away
  for (ACE_Timer_Node_T <ACE_Event_Handler *> *node;
       (node = queue_->timer_queue ().get_first ()) != 0;
       )
    queue->cancel (node->get_timer_id (), 0);

  relay_->end_transmission (Bounded_Packet_Relay::TIMED_OUT);
  delete this;
  return 0;
}
  // Call back hook.

int 
Termination_Handler::cancelled (void)
{
  delete this;
  return 0;
}
  // Cancellation hook

Thread_Bounded_Packet_Relay_Driver::Thread_Bounded_Packet_Relay_Driver (void)
  : input_task_ (&timer_queue_, *this)
{
}
// ctor

Thread_Bounded_Packet_Relay_Driver::~Thread_Bounded_Packet_Relay_Driver (void)
{
}
// dtor

int 
Thread_Bounded_Packet_Relay_Driver::display_menu (void)
{
  static char menu[] =
    "\n\n  Options:\n"
    "  -------\n"
    "  1 <number of packets to relay in one transmission>\n"
    "     min = 1 packet, default = 1000.\n"
    "  2 <input packet arrival period (in usec)>\n"
    "     min = 1, default = 500.\n"
    "  3 <output packet transmission period (in usec)>\n"
    "     min = 1, default = 1000.\n"
    "  4 <limit on duration of transmission (in usec)>\n"
    "     min = 1, default = 1500000, no limit = 0.\n"
    "  5 <logging level>\n"
    "     no logging = 0 (default), logging = 1.\n"
    "  6 - run a transmission using the current settings\n"
    "  7 - cancel transmission (if there is one running)\n"
    "  8 - report statistics from the most recent transmission\n"
    "  9 - quit the program\n";

  // @@ Chris, don't use ACE_OS::printf...
  ACE_OS::fprintf (ACE_STDERR, "%s", menu);

  return 0;
}
// display the user menu

int 
Thread_Bounded_Packet_Relay_Driver::init (void);
{
  // @@ Chris, I suspect that putting a typedef here may break some
  // compilers.  Can you please move it to outside of the method?
  typedef Command<Input_Task, Input_Task::ACTION> CMD;

  // initialize the <Command> objects with their corresponding
  // methods from <Input_Task>
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
// initialize the driver

int 
Thread_Bounded_Packet_Relay_Driver::run (void)
{
  this->init ();
  return 0;
}
// run the driver

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Thread_Timer_Queue_Adapter<Timer_Heap>;
template class Bounded_Packet_Relay_Driver<Thread_Timer_Queue,
                                       Input_Task,
                                       Input_Task::ACTION>;
template class Command<Input_Task, Input_Task::ACTION>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Thread_Timer_Queue_Adapter<Timer_Heap>
#pragma instantiate Bounded_Packet_Relay_Driver<Thread_Timer_Queue, \
                                           Input_Task, \
                                           Input_Task::ACTION>
#pragma instantiate Command<Input_Task, Input_Task::ACTION>
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
