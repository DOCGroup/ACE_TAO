// $Id$

// ============================================================================
// = LIBRARY
//    examples
//
// = FILENAME
//    BPR_Driver.cpp
//
// = DESCRIPTION
//    This code builds an abstraction to factor out common code for
//    the different implementations of the Timer_Queue.
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

#if !defined (_BPR_DRIVER_CPP_)
#define _BPR_DRIVER_CPP_

#include "ace/Auto_Ptr.h"
#include "BPR_Drivers.h"

ACE_RCSID(Bounded_Packet_Relay, BPR_Driver, "$Id$")

// Constructor.

template <ACE_SYNCH_DECL>
Bounded_Packet_Relay<ACE_SYNCH_USE>::Bounded_Packet_Relay (ACE_Thread_Manager *input_task_mgr,
                                                           Input_Device_Wrapper_Base *input_wrapper,
                                                           Output_Device_Wrapper_Base *output_wrapper)
  : input_task_mgr_ (input_task_mgr),
    input_wrapper_ (input_wrapper),
    output_wrapper_ (output_wrapper),
    transmission_number_ (0),
    packets_sent_ (0),
    status_ (Bounded_Packet_Relay_Base::UN_INITIALIZED),
    transmission_start_ (0, 0),
    transmission_end_ (0, 0)
{
  if (input_task_mgr_ == 0)
    input_task_mgr_ = ACE_Thread_Manager::instance ();
}

// Destructor.

template <ACE_SYNCH_DECL>
Bounded_Packet_Relay<ACE_SYNCH_USE>::~Bounded_Packet_Relay (void)
{
}

// Requests output be sent to output device.

template <ACE_SYNCH_DECL> int
Bounded_Packet_Relay<ACE_SYNCH_USE>::send_input (void)
{
  // Don't block, return immediately if queue is empty.
  ACE_Message_Block *item;

  // @@ Chris, I think you can just say
  // if (queue_.dequeue_head (item, &ACE_Time_Value::zero) == -1)
  //   return 1;
  // rather than having to get the current time of day...
  ACE_Time_Value now = ACE_OS::gettimeofday ();

  if (queue_.dequeue_head (item, &now) < 0)
    return 1;

  // If a message block was dequeued, send it to the output device.

  if (output_wrapper_->write_output_message ((void *) item) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "failed to write to output device object"), 
                      -1);

  // If all went OK, increase count of packets sent.
  ++packets_sent_;
  return 0;
}

// Requests a transmission be started.

template <ACE_SYNCH_DECL> int
Bounded_Packet_Relay<ACE_SYNCH_USE>::start_transmission (u_long packet_count,
                                                         u_long arrival_period,
                                                         u_long logging_level)
{
  // Serialize access to start and end transmission calls, statistics
  // reporting calls.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->transmission_lock_, -1);

  // If a transmission is already in progress, just return.
  if (status_ == STARTED)
    return 1;

  // Update statistics for a new transmission.
  ++transmission_number_;
  packets_sent_ = 0;
  status_ = STARTED;
  transmission_start_ = ACE_OS::gettimeofday ();  

  // Initialize the output device.
  if (output_wrapper_->modify_device_settings ((void *) &logging_level) < 0)
    {
      status_ = ERROR;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed to initialize output device object"), 
                        -1);
    }

  // Initialize the input device.
  if (input_wrapper_->set_input_period (arrival_period) < 0)
    {
      status_ = ERROR;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed to initialize input device object"), 
                        -1);
    }

  // Activate the input device and save a handle to the new thread.
  if (input_wrapper_->activate () < 0)
    {
      status_ = ERROR;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed to activate input device object"), 
                        -1);
    }

  // If all went well, return success.
  return 0;
}

// Requests a transmission be ended.

template <ACE_SYNCH_DECL> int
Bounded_Packet_Relay<ACE_SYNCH_USE>::end_transmission (Transmission_Status status)
{
  // Serialize access to start and end transmission calls,
  // statistics reporting calls.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->transmission_lock_, -1);

  // If a transmission is not already in progress, just return.
  if (status_ != STARTED)
    return 1;

  // Ask the the input thread to stop.
  if (input_wrapper_->request_stop () < 0)
    {
      status_ = ERROR;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed asking input device thread to stop"), 
                        -1);
    }
 
  // Wait for input thread to stop.
  if (input_task_mgr_->wait_task (input_wrapper_) < 0)
    {
      status_ = ERROR;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed waiting for input device thread to stop"), 
                        -1);
    }

  // If all went well, set passed status, stamp end time, return
  // success.
  status_ = status;
  transmission_end_ = ACE_OS::gettimeofday ();  
  return 0;
}

// Requests a report of statistics from the last transmission.

template <ACE_SYNCH_DECL> int
Bounded_Packet_Relay<ACE_SYNCH_USE>::report_statistics (void)
{
  // Serialize access to start and end transmission calls,
  // statistics reporting calls.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->transmission_lock_, -1);

  // If a transmission is already in progress, just return.
  if (status_ == STARTED)
    return 1;

  const char *status_msg;
  switch (status_)
    {
      case UN_INITIALIZED:
        status_msg = "Uninitialized";
        break;
      case STARTED: 
        // NOT REACHED: user should never see this ;-)
        status_msg = "In progress";
        break;
      case COMPLETED: 
        status_msg = "Completed with all packets sent";
        break;
      case TIMED_OUT: 
        status_msg = "Terminated by transmission duration timer";
        break;
      case CANCELLED:
        status_msg = "Cancelled by external control";
        break;
      case ERROR:
        status_msg = "Error was detected";
        break;
      default:
        status_msg = "Unknown";
        break;
    }

  // Calculate duration of trasmission.
  ACE_Time_Value duration (transmission_end_);
  duration -= transmission_start_;

  // Report transmission statistics.
  ACE_DEBUG ((LM_DEBUG,
              "\n\nStatisics for transmission %lu:\n\n"
              "Transmission status: %s\n"
              "Start time:          %d (sec) %d (usec)\n"
              "End time:            %d (sec) %d (usec)\n"
              "Duration:            %d (sec) %d (usec)\n"
              "Packets relayed:     %u\n\n",
              transmission_number_, status_msg, 
              transmission_start_.sec (),
              transmission_start_.usec (),
              transmission_end_.sec (),
              transmission_end_.usec (),
              duration.sec (),
              duration.usec (),
              packets_sent_));
  return 0;
}

// Public entry point to which to push input.

template <ACE_SYNCH_DECL> int
Bounded_Packet_Relay<ACE_SYNCH_USE>::receive_input (void * arg)
{
  ACE_Message_Block *message = ACE_static_cast (ACE_Message_Block *,
                                                arg);
  if (queue_.enqueue_tail (message) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "Bounded_Packet_Relay<ACE_SYNCH_USE>::receive_input failed"), 
                      -1);
  return 0;
}

// Constructor.

Input_Device_Wrapper_Base::Input_Device_Wrapper_Base (ACE_Thread_Manager *input_task_mgr)
  : ACE_Task_Base (input_task_mgr),
    send_input_msg_cmd_ (0),
    input_period_ (ACE_ONE_SECOND_IN_USECS),
    is_active_ (0)
{
}

// Destructor.

Input_Device_Wrapper_Base::~Input_Device_Wrapper_Base (void)
{
  delete send_input_msg_cmd_;
}
 
// Sets send input message command in the input device driver object.

int
Input_Device_Wrapper_Base::set_send_input_msg_cmd (Command_Base *send_input_msg_cmd)
{
  // Delete the old command (if any), then set the new command.
  delete send_input_msg_cmd_;
  send_input_msg_cmd_ = send_input_msg_cmd;
  return 0;
}

// Sets period between when input messages are produced.

int 
Input_Device_Wrapper_Base::set_input_period (u_long input_period)
{
  input_period_ = input_period;
  return 0;
}

// Sets count of messages to send.

int 
Input_Device_Wrapper_Base::set_send_count (long count)
{
  send_count_ = count;
  return 0;
}

// Request that the input device stop sending messages
// and terminate its thread.  Should return 1 if it will do so, 0
// if it has already done so, or -1 if there is a problem doing so.

int 
Input_Device_Wrapper_Base::request_stop (void)
{
  if (is_active_)
    {
      is_active_ = 0;
      return 1;
    }

  return 0;
}

// This method runs the input device loop in the new thread.

int 
Input_Device_Wrapper_Base::svc (void)
{
  long count;
  ACE_Time_Value timeout;
  ACE_Message_Block *message;

  // Set a flag to indicate we're active.
  is_active_ = 1;

  // Start with the total count of messages to send.
  for (count = send_count_;
       // While we're still marked active, and there are packets to send.
       is_active_ && count != 0;
       )
    {
      // Make sure there is a send command object.
      if (send_input_msg_cmd_ == 0)
        {
          is_active_ = 0;
          ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                             "send message command object not instantiated"), 
                            -1);
        }

      // Create an input message to send.
      message = create_input_message ();
      if (message == 0)
        {
          is_active_ = 0;
          ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                             "Failed to create input message object"), 
                             -1);
        }

      // Send the input message.
      if (send_input_msg_cmd_->execute ((void *) message) < 0)
        {
          is_active_ = 0;
          delete message;
          ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                             "Failed executing send message command object"), 
                            -1);
        }

      // If all went well, decrement count of messages to send, and
      // run the reactor event loop unti we get a timeout or something
      // happens in a registered upcall.
      if (count > 0) 
        --count;

      timeout = ACE_Time_Value (0, input_period_);
      reactor_.run_event_loop (timeout);
    }

  is_active_ = 0;

  return 0;
}

// Sends a newly created message block, carrying data read from the
// underlying input device, by passing a pointer to the message block
// to its command execution.

int 
Input_Device_Wrapper_Base::send_input_message (ACE_Message_Block *amb)
{
  if (send_input_msg_cmd_)
    return send_input_msg_cmd_->execute ((void *) amb);
  else
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "Input_Device_Wrapper_Base::send_input_message: "
                       "command object not instantiated"), 
                      -1);
}

// Parse the input and execute the corresponding command.

template <class TQ> int
Bounded_Packet_Relay_Driver<TQ>::parse_commands (const char *buf)
{
  int option;

  if (::sscanf (buf, "%d", &option) <= 0)
    // If there was an error reading the option simply try on the next
    // line.
    return 0;

  switch (option)
    {
      case 1:  // set packet count
        {
          u_long count;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &count) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (packet_count_cmd_->execute ((void *) &count) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set packet count failed"),
                              -1);
          break;
        }
      case 2:  // Set the arrival period.
        {
          u_long usec;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (arrival_period_cmd_->execute ((void *) &usec) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set arrival period failed"),
                              -1);
          break;
        }
      case 3:  // Set transmit period.
        {
          u_long usec;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (transmit_period_cmd_->execute ((void *) &usec) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set transmit period failed"),
                              -1);
          break;
        }
      case 4:  // Set duration limit.
        {
          u_long usec;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (duration_limit_cmd_->execute ((void *) &usec) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set duration limit failed"),
                              -1);
          break;
        }
      case 5:  // Set logging level.
        {
          u_long level;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &level) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (logging_level_cmd_->execute ((void *) &level) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set logging level failed"),
                              -1);
          break;
        }
      case 6: // Run one transmission.
        return run_transmission_cmd_->execute (0);
        /* NOTREACHED */
      case 7: // Report statistics.
        return report_stats_cmd_->execute (0);
        /* NOTREACHED */
      case 8: // Shut down the driver.
        return shutdown_cmd_->execute (0);
        /* NOTREACHED */
      default:
        // Display an error message.
        ACE_ERROR_RETURN ((LM_ERROR, "invalid input %s\n", buf), 0);
        ACE_NOTREACHED (break);
        /* NOTREACHED */
    } /* ENDSWITCH */
  return 0;
}

// Runs the test.

template <class TQ> int
Bounded_Packet_Relay_Driver<TQ>::run (void)
{
  this->init ();

  // Process all the incoming events.

  for (;;)
    if (this->get_next_request () == -1)
      return -1;

  ACE_NOTREACHED (return 0);
}

// Gets the next request from the user input.

template <class TQ> int
Bounded_Packet_Relay_Driver<TQ>::get_next_request (void)
{
  char buf[BUFSIZ];

  this->display_menu ();

  ACE_DEBUG ((LM_DEBUG,
              "Please enter your choice: "));

  // Reads input from the user.
  if (this->read_input (buf, sizeof buf) <= 0)
    return -1;

  // Parse and run the command.
  return this->parse_commands (buf);
}

// Reads input from the user from ACE_STDIN into the buffer specified.

template <class TQ> ssize_t
Bounded_Packet_Relay_Driver<TQ>::read_input (char *buf, size_t bufsiz)
{
  ACE_OS::memset (buf, 0, bufsiz);

  // Wait for user to type commands.  This call is automatically
  // restarted when SIGINT or SIGALRM signals occur.
  return ACE_OS::read (ACE_STDIN, buf, bufsiz);
}

#endif /* _BPR_DRIVER_CPP_ */
