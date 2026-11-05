
//=============================================================================
/**
 *  @file    Thread_Bounded_Packet_Relay.cpp
 *
 *  Method definitions for the threaded-bounded packet relay class.
 *
 *  @author Chris Gill <cdgill@cs.wustl.edu> and Douglas C. Schmidt
 *          <d.schmidt@vanderbilt.edu>. Based on Timer Queue Test by Carlos O'Ryan
 *          <coryan@cs.wustl.edu>, Douglas C. Schmidt <d.schmidt@vanderbilt.edu>,
 *          and Sergio Flores-Gaitan <sergio@cs.wustl.edu>.
 */
//=============================================================================


#include "ace/OS_NS_sys_time.h"
#include "ace/Condition_T.h"
#include "ace/Null_Mutex.h"
#include <memory>
#include <new>

#include "Thread_Bounded_Packet_Relay.h"

using DRIVER_CMD = Thread_Bounded_Packet_Relay_Driver::MYCOMMAND;
using HANDLER_CMD = ACE_Command_Callback<BPR_Handler_Base, BPR_Handler_Base::ACTION>;
using SEND_HANDLER_CMD = ACE_Command_Callback<Send_Handler, Send_Handler::ACTION>;


// Constructor.

Text_Input_Device_Wrapper::Text_Input_Device_Wrapper (ACE_Thread_Manager *input_task_mgr,
                                                      std::string const &text,
                                                      int logging)
  : Input_Device_Wrapper_Base (input_task_mgr),
    text_ (text),
    logging_ (logging)
{
}

// Modifies device settings based on passed pointer to a u_long.

int
Text_Input_Device_Wrapper::modify_device_settings (void *logging)
{
  packet_count_ = 0;

  if (logging)
    logging_ = *static_cast<int *> (logging);
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Text_Input_Device_Wrapper::modify_device_settings: "
                       "null argument"),
                      -1);
  return 0;
}


// Creates a new message block, carrying data
// read from the underlying input device.

ACE_Message_Block *
Text_Input_Device_Wrapper::create_input_message ()
{
  if (text_.empty ())
    return nullptr;

  std::string read_buffer (text_.size () + 1, '\0');
  for (size_t i = 0; i < text_.size (); ++i)
    {
      read_buffer[i] = text_[index_];
      index_ = (index_ + 1) % text_.size ();
    }

  // Construct a new message block to send.
  std::unique_ptr<ACE_Message_Block> mb (
    new (std::nothrow) ACE_Message_Block (read_buffer.size ()));
  if (!mb)
    {
      errno = ENOMEM;
      return nullptr;
    }

  // Copy buf into the Message_Block and update the wr_ptr ().
  if (mb->copy (read_buffer.c_str (), read_buffer.size ()) < 0)
    {
      ACE_ERROR ((LM_ERROR, "read buffer copy failed"));
      return nullptr;
    }

  // log packet creation if logging is turned on
  if (logging_ & Text_Input_Device_Wrapper::LOG_MSGS_CREATED)
    {
      ++packet_count_;
      ACE_DEBUG ((LM_DEBUG, "input message %d created\n",
                  packet_count_));
    }

  return mb.release ();
}

// Constructor.

Text_Output_Device_Wrapper::Text_Output_Device_Wrapper (int logging)
  : logging_ (logging)
{
}

// Consume and possibly print out the passed message.

int
Text_Output_Device_Wrapper::write_output_message (ACE_Message_Block *message)
{
  std::unique_ptr<ACE_Message_Block> message_block (message);
  if (message_block)
    {
      ++packet_count_;

      if (logging_ & Text_Output_Device_Wrapper::LOG_MSGS_RCVD)
        ACE_DEBUG ((LM_DEBUG, "output message %d received\n",
                    packet_count_));

      if (logging_ & Text_Output_Device_Wrapper::PRINT_MSGS_RCVD)
        ACE_DEBUG ((LM_DEBUG, "output message %d:\n[%s]\n",
                    packet_count_,
                    message_block->rd_ptr ()));

      return 0;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     "Text_Output_Device_Wrapper::"
                     "write_output_message: null argument"), -1);
}

// Modifies device settings based on passed pointer to a u_long.

int
Text_Output_Device_Wrapper::modify_device_settings (void *logging)
{
  packet_count_ = 0;

  if (logging)
    logging_ = *static_cast<int *> (logging);
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Text_Output_Device_Wrapper::modify_device_settings: "
                       "null argument"),
                      -1);
  return 0;
}

// Constructor.

User_Input_Task::User_Input_Task (Bounded_Packet_Relay *relay,
                                  Thread_Timer_Queue *queue,
                                  Thread_Bounded_Packet_Relay_Driver &tbprd)
  : ACE_Task_Base (ACE_Thread_Manager::instance ()),
    relay_ (relay),
    queue_ (queue),
    driver_ (tbprd)
{
}

// Destructor.

User_Input_Task::~User_Input_Task ()
{
  this->clear_all_timers ();
}

// Runs the main event loop.

int
User_Input_Task::svc ()
{
  for (;;)
    // Call back to the driver's implementation of how to read and
    // parse input.
    if (this->driver_.get_next_request () == -1)
      break;

  // We are done.
  this->relay_->end_transmission (Bounded_Packet_Relay::CANCELLED);
  this->queue_->deactivate ();
  ACE_DEBUG ((LM_DEBUG,
              "terminating user input thread\n"));
  this->clear_all_timers ();
  return 0;
}

// Sets the number of packets for the next transmission.

int
User_Input_Task::set_packet_count (void *argument)
{
  if (argument)
    {
      driver_.packet_count (*static_cast<u_long const *> (argument));
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
      driver_.arrival_period (*static_cast<u_long const *> (argument));
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
      driver_.send_period (*static_cast<u_long const *> (argument));
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
      driver_.duration_limit (*static_cast<u_long const *> (argument));
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
      driver_.logging_level (*static_cast<int *> (argument));
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
                       " Transmission already in progress\n"));
            return 0;
            /* NOTREACHED */
          case 0:
            {
              ACE_Time_Value const now = ACE_OS::gettimeofday ();
              ACE_Time_Value const send_every (0, driver_.send_period ());
              ACE_Time_Value const send_at (send_every + now);

              std::unique_ptr<Send_Handler> send_handler (
                new (std::nothrow) Send_Handler (driver_.packet_count (),
                                                 send_every,
                                                 *relay_,
                                                 *queue_,
                                                 driver_));
              if (!send_handler)
                {
                  errno = ENOMEM;
                  return -1;
                }
              if (queue_->schedule (send_handler.get (), nullptr, send_at) < 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "User_Input_Task::run_transmission: "
                                     "failed to schedule send handler"),
                                    -1);
                }
              send_handler.release ();
              if (driver_.duration_limit ())
                {
                  ACE_Time_Value terminate_at (0, driver_.duration_limit ());
                  terminate_at += now;

                  std::unique_ptr<Termination_Handler> termination_handler (
                    new (std::nothrow) Termination_Handler (*relay_,
                                                            *queue_,
                                                            driver_));
                  if (! termination_handler)
                    {
                      errno = ENOMEM;
                      this->clear_all_timers ();
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "User_Input_Task::run_transmission: "
                                         "failed to allocate termination "
                                         "handler"),
                                        -1);
                    }
                  if (queue_->schedule (termination_handler.get (),
                                        nullptr, terminate_at) < 0)
                    {
                      this->clear_all_timers ();
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "User_Input_Task::run_transmission: "
                                         "failed to schedule termination "
                                         "handler"),
                                        -1);
                    }
                  termination_handler.release ();
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
      switch (relay_->end_transmission (Bounded_Packet_Relay::CANCELLED))
        {
          case 1:
            ACE_DEBUG ((LM_DEBUG,
                        "\nEnd transmission: "
                        "no transmission in progress\n"));
            ACE_FALLTHROUGH;
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
                        "\ntransmission already in progress\n"));
            return 0;
             /* NOTREACHED */

          case 0:
            this->clear_all_timers ();
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
  // Clear any outstanding timers.
  this->clear_all_timers ();

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
User_Input_Task::clear_all_timers ()
{
  // loop through the timers in the queue, cancelling each one
  for (ACE_Timer_Node_T <ACE_Event_Handler *> *node {};
       (node = queue_->timer_queue ()->get_first ()) != nullptr;
       )
    queue_->timer_queue ()->cancel (node->get_timer_id (), nullptr, 0);

  return 0;
}

// Constructor.

BPR_Handler_Base::BPR_Handler_Base (Bounded_Packet_Relay &relay,
                                    Thread_Timer_Queue &queue)
  : relay_ (relay),
    queue_ (queue)
{
}

// Helper method: clears all timers.

int
BPR_Handler_Base::clear_all_timers (void *)
{
  // Loop through the timers in the queue, cancelling each one.

  for (ACE_Timer_Node_T <ACE_Event_Handler *> *node {};
       (node = queue_.timer_queue ()->get_first ()) != nullptr;
       )
    queue_.timer_queue ()->cancel (node->get_timer_id (), nullptr, 0);
  //    queue_.cancel (node->get_timer_id (), 0);

  // Invoke the handler's (virtual) destructor
  delete this;

  return 0;
}

// Constructor.

Send_Handler::Send_Handler (u_long send_count,
                            ACE_Time_Value const &duration,
                            Bounded_Packet_Relay &relay,
                            Thread_Timer_Queue &queue,
                            Thread_Bounded_Packet_Relay_Driver &driver)
  : BPR_Handler_Base (relay, queue),
    send_count_ (send_count),
    duration_ (duration),
    driver_ (driver)
{
}

// Call back hook.

int
Send_Handler::handle_timeout (ACE_Time_Value const &,
                              void const *)
{
  switch (relay_.send_input ())
    {
      case 0:
        // Decrement count of packets to relay.
        --send_count_;
        ACE_FALLTHROUGH;
      case 1:
        if (send_count_ > 0)
          {
            // Enqueue a deferred callback to the reregister command.
            std::unique_ptr<SEND_HANDLER_CMD> re_register_callback (
              new (std::nothrow) SEND_HANDLER_CMD (*this,
                                                   &Send_Handler::reregister));
            if (!re_register_callback)
              {
                errno = ENOMEM;
                return -1;
              }
            if (queue_.enqueue_command (re_register_callback.get ()) < 0)
              return -1;
            re_register_callback.release ();
            return 0;
          }
        else
          {
            // All packets are sent, time to end the transmission, redisplay
            // the user menu, cancel any other timers, and go away.
            relay_.end_transmission (Bounded_Packet_Relay::COMPLETED);
            driver_.display_menu ();

            // Enqueue a deferred callback to the clear_all_timers command.
            std::unique_ptr<HANDLER_CMD> clear_timers_callback (
              new (std::nothrow) HANDLER_CMD (*this,
                                              &BPR_Handler_Base::clear_all_timers));
            if (!clear_timers_callback)
              {
                errno = ENOMEM;
                return -1;
              }
            if (queue_.enqueue_command (clear_timers_callback.get ()) < 0)
              return -1;
            clear_timers_callback.release ();
            return 0;
          }
        /* NOTREACHED */
      default:
        return -1;
    }
}

// Cancellation hook.

int
Send_Handler::cancelled ()
{
  delete this;
  return 0;
}

// Helper method: re-registers this timer

int
Send_Handler::reregister (void *)
{
  // Re-register the handler for a new timeout.
  if (queue_.schedule (this,
                       nullptr,
                       duration_ + ACE_OS::gettimeofday ()) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Send_Handler::reregister: "
                       "failed to reschedule send handler"),
                       -1);

  return 0;
}


// Constructor.

Termination_Handler::Termination_Handler (Bounded_Packet_Relay &relay,
                                          Thread_Timer_Queue &queue,
                                          Thread_Bounded_Packet_Relay_Driver &driver)
  : BPR_Handler_Base (relay, queue),
    driver_ (driver)
{
}

// Call back hook.

int
Termination_Handler::handle_timeout (ACE_Time_Value const &,
                                     void const *)
{
  // Transmission timed out, so end the transmission, display the user
  // menu, and register a callback to clear the timer queue and then
  // make this object go away.
  relay_.end_transmission (Bounded_Packet_Relay::TIMED_OUT);
  driver_.display_menu ();

  // Enqueue a deferred callback to the clear_all_timers command.
  std::unique_ptr<HANDLER_CMD> clear_timers_callback (
    new (std::nothrow) HANDLER_CMD (*this,
                                    &BPR_Handler_Base::clear_all_timers));
  if (!clear_timers_callback)
    {
      errno = ENOMEM;
      return -1;
    }
  if (queue_.enqueue_command (clear_timers_callback.get ()) < 0)
    return -1;
  clear_timers_callback.release ();
  return 0;
}

// Cancellation hook

int
Termination_Handler::cancelled ()
{
  delete this;
  return 0;
}

// Constructor.

Thread_Bounded_Packet_Relay_Driver::Thread_Bounded_Packet_Relay_Driver (Bounded_Packet_Relay *relay)
  : input_task_ (relay, &timer_queue_, *this)
{
}

// Display the user menu.

int
Thread_Bounded_Packet_Relay_Driver::display_menu ()
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
    "  5 <logging level flags = %d>\n"
    "     no logging =                                              0,\n"
    "     log packets created by input device =                     1,\n"
    "     log packets consumed by output device =                   2,\n"
    "     logging options 1,2 =                                     3,\n"
    "     print contents of packets consumed by output put device = 4,\n"
    "     logging options 1,4 =                                     5,\n"
    "     logging options 2,4 =                                     6,\n"
    "     logging options 1,2,4 =                                   7.\n"
    "  ----------------------------------------------------------------------\n"
    "  6 - runs a transmission using the current settings\n"
    "  7 - cancels a transmission (if there is one running)\n"
    "  8 - reports statistics from the most recent transmission\n"
    "  9 - quits the program\n"
    "  ----------------------------------------------------------------------\n"
    "  Please enter your choice: ";

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
Thread_Bounded_Packet_Relay_Driver::init ()
{
  // Initialize the <Command> objects with their corresponding
  // methods from <User_Input_Task>.
  packet_count_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::set_packet_count));
  if (!packet_count_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  arrival_period_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::set_arrival_period));
  if (!arrival_period_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  transmit_period_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::set_send_period));
  if (!transmit_period_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  duration_limit_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::set_duration_limit));
  if (!duration_limit_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  logging_level_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::set_logging_level));
  if (!logging_level_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  run_transmission_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::run_transmission));
  if (!run_transmission_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  cancel_transmission_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::end_transmission));
  if (!cancel_transmission_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  report_stats_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::report_stats));
  if (!report_stats_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
  shutdown_cmd_.reset (new (std::nothrow) DRIVER_CMD (
    input_task_, &User_Input_Task::shutdown));
  if (!shutdown_cmd_)
    {
      errno = ENOMEM;
      return -1;
    }
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
Thread_Bounded_Packet_Relay_Driver::run ()
{
  return this->init ();
}
