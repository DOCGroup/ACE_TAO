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
// = AUTHOR
//    Chris Gill           <cdgill@cs.wustl.edu>
//
//    Based on examples/Timer_Queue/Driver.cpp, written by
//    Douglas Schmidt      <schmidt@cs.wustl.edu> &&
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#if !defined (_BPR_DRIVER_CPP_)
#define _BPR_DRIVER_CPP_

#include "ace/Auto_Ptr.h"
#include "BPR_Driver.h"

ACE_RCSID(Bounded_Packet_Relay, BPR_Driver, "$Id$")

Input_Device_Wrapper_Base::Input_Device_Wrapper_Base (ACE_Thread_Manager * input_task_mgr)
  : ACE_Task_Base (input_task_mgr)
  , send_input_msg_cmd_ (0)
  , input_rate_ (ACE_ONE_SECOND_IN_USECS)
  , reactor_
  , is_active_ (0)
{
}
  // ctor

Input_Device_Wrapper_Base::Input_Device_Wrapper_Base ()
{
  delete send_input_msg_cmd_;
}
  // dtor
 
int
Input_Device_Wrapper_Base::set_send_input_msg_cmd (Command_Base *send_input_msg_cmd)
{
  // delete the old command (if any) first
  delete send_input_msg_cmd_;

  send_input_msg_cmd_ = send_input_msg_cmd;
  return 0;
}
  // sets send input message command in the input device driver object

int 
Input_Device_Wrapper_Base::set_input_period (u_long input_period)
{
  input_period_ = input_period;
  return 0;
}
  // sets period between when input messages are produced

int 
Input_Device_Wrapper_Base::set_send_count (long count)
{
  send_count_ = count;
}
  // sets count of messages to send

int 
Input_Device_Wrapper_Base::request_stop (void)
{
  is_active_ = 0;
}
  // request that the input device stop sending messages
  // and terminate its thread.  Should return 1 if it will do so, 0
  // if it has already done so, or -1 if there is a problem doing so.


int 
Input_Device_Wrapper_Base::svc (void)
{
  long count;
  ACE_Time_Value timeout;
  ACE_Message_Block *message;

  // set a flag ti indicate we're active
  is_active_ = 1;

  // start with the total count of messages to send
  count = send_count_;

  // while we're still marked active, and there are packets to send
  while ((is_active_) && (count != 0))
  {
    // make sure there is a send command object
    if (! send_input_msg_cmd_)
    {
      is_active_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "send message command object not instantiated"), 
                        -1);
    }

    // create an input message to send
    message = create_input_message ();
    if (! message)
    {
      is_active_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "Failed to create input message object"), 
                        -1);
    }

    // send the input message
    if (send_input_msg_cmd_->execute ((void *) message) < 0)
    {
      is_active_ = 0;
      delete message;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "Failed executing send message command object"), 
                        -1);
    }

    // if all went well, decrement count of messages to send,
    // and run the reactor event loop unti we get a timeout
    // or something happens in a registered upcall.
    if (count > 0) 
    {
      --count;
    }
    timeout = ACE_Time_Value (0, period_);
    reactor_.run_event_loop (timeout);
  }

  is_active_ = 0;

  return 0;
}
  // This method runs the input device loop in the new thread.

int 
Input_Device_Wrapper_Base::send_input_message (ACE_Message_Block *amb)
{
  if (send_input_msg_cmd_)
  {
    return send_input_msg_cmd_->execute ((void *) amb);
  }
  else
  {
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "Input_Device_Wrapper_Base::send_input_message: "
                       "command object not instantiated"), 
                      -1);
  }
}
  // sends a newly created message block, carrying data
  // read from the underlying input device, by passing a
  // pointer to the message block to its command execution

template <class SYNCH>
Bounded_Packet_Relay<SYNCH>::Bounded_Packet_Relay (
  ACE_Thread_Manager * input_task_mgr,
  Input_Device_Wrapper_Base * input_wrapper,
  Output_Device_Wrapper_Base * output_wrapper)
  : input_task_mgr_ (input_task_mgr)
  , input_wrapper_ (input_wrapper)
  , input_thread_handle_ (0)
  , output_wrapper_ (output_wrapper)
  , queue_
  , is_active_ (0)
  , transmission_lock_
  , queue_lock_
  , transmission_number_ (0)
  , packets_sent_ (0)
  , status_ (Bounded_Packet_Relay<SYNCH>::UN_INITIALIZED)
  , elapsed_duration_ (0)
{
  if (input_task_mgr_ == 0)
  {
    input_task_mgr_ = ACE_Thread_Manager::instance ();
  }
}
  // ctor

template <class SYNCH>
Bounded_Packet_Relay<SYNCH>::~Bounded_Packet_Relay (void)
{
  delete input_wrapper_;
  delete output_wrapper_;
}
  // dtor

template <class SYNCH> int
Bounded_Packet_Relay<SYNCH>::send_input (void)
{
  ACE_Message_Block *item;

  // don't block, return immediately if queue is empty
  if (queue_.dequeue_head (item, ACE_OS::gettimeofday ()) < 0)
  {
    return 1;
  }

  // if a message block was dequeued, send it to the output device
  if (output_wrapper_->write_output_message ((void *) item) < 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "failed to write to output device object"), 
                      -1);
  }

  // if all went OK, increase count of packets sent
  ++packets_sent_;
  return 0;
}
  // Requests output be sent to output device.

template <class SYNCH> int
Bounded_Packet_Relay<SYNCH>::start_transmission (u_long packet_count,
                                                 u_long arrival_period,
                                                 u_long logging_level)
{
  // serialize access to start and end transmission calls,
  // statistics reporting calls
  ACE_GUARD_RETURN (SYNCH, ace_mon, this->transmission_lock_, -1);

  // if a transmission is already in progress, just return
  if (status_ == STARTED)
  {
    return 1;
  }

  // Update statistics for a new transmission
  ++transmission_number;
  packets_sent_ = 0;
  status_ = STARTED;
  transmission_start_ = ACE_OS::gettimeofday ();  

  // initialize the output device
  if (output_wrapper_->modify_device_settings ((void *) &logging level) < 0)
  {
    status_ = ERROR;
    transmission_end_ = ACE_OS::gettimeofday ();  
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "failed to initialize output device object"), 
                      -1);
  }

  // initialize the input device
  if (input_wrapper_->set_input_period (u_long input_period) < 0)
  {
    status_ = ERROR;
    transmission_end_ = ACE_OS::gettimeofday ();  
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "failed to initialize input device object"), 
                      -1);
  }

  // activate the input device and save a handle to the new thread
  if (input_wrapper_->activate () < 0)
  {
    status_ = ERROR;
    transmission_end_ = ACE_OS::gettimeofday ();  
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "failed to activate input device object"), 
                      -1);
  }

  // If all went well, return success
  return 0;
}
  // Requests a transmission be started.

template <class SYNCH> int
Bounded_Packet_Relay<SYNCH>::end_transmission (Transmission_Status status)
{
  // serialize access to start and end transmission calls,
  // statistics reporting calls
  ACE_GUARD_RETURN (SYNCH, ace_mon, this->transmission_lock_, -1);

  // if a transmission is not already in progress, just return
  if (status_ != STARTED)
  {
    return 1;
  }

  // ask the the input thread to stop
  if (input_wrapper_->request_stop () < 0)
  {
    status_ = ERROR;
    transmission_end_ = ACE_OS::gettimeofday ();  
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "failed asking input device thread to stop"), 
                      -1);
  }
 
  // wait for input thread to stop
  if (input_task_mgr_->wait_task (input_wrapper_) < 0)
  {
    status_ = ERROR;
    transmission_end_ = ACE_OS::gettimeofday ();  
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "failed waiting for input device thread to stop"), 
                      -1);
  }

  // if all went well, set passed status, stamp end time, return success
  status_ = status;
  transmission_end_ = ACE_OS::gettimeofday ();  
  return 0;
}
  // Requests a transmission be ended.

template <class SYNCH> int
Bounded_Packet_Relay<SYNCH>::report_statistics (void)
{
  // serialize access to start and end transmission calls,
  // statistics reporting calls
  ACE_GUARD_RETURN (SYNCH, ace_mon, this->transmission_lock_, -1);

  // if a transmission is already in progress, just return
  if (status_ == STARTED)
  {
    return 1;
  }

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

  // calculate duration of trasmission
  ACE_Time_Value duration (transmission_end_);
  duration -= transmission_start_;

  // report transmission statistics
  if (ACE_OS::fprintf (ACE_STDOUT,
                   "\n\nStatisics for transmission %lu:\n\n"
                   "Transmission status: %s\n"
                   "Start time:          %ld (sec) %ld usec\n"
                   "End time:            %ld (sec) %ld usec\n"
                   "Duration:            %ld (sec) %ld usec\n"
                   "Packets relayed:     %lu\n\n",
                   transmission_number_, status_msg, 
                   transmission_start_.sec (),
                   transmission_start_.usec (),
                   transmission_end_.sec (),
                   transmission_end_.usec (),
                   duration.sec (),
                   duration.usec (),
                   packets_sent_) < 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "Bounded_Packet_Relay<SYNCH>::report_statistics"
                       "ACE_OS::fprintf failed"), -1);
  }

  if (ACE_OS::fflush (ACE_STDOUT) < 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "Bounded_Packet_Relay<SYNCH>::report_statistics"
                       "ACE_OS::fflush failed"), -1);
  }

  return 0;
}
  // Requests a report of statistics from the last transmission.

template <class SYNCH> int
Bounded_Packet_Relay<SYNCH>::receive_input (void * arg)
{
  ACE_Message_Block *message;

  message = ACE_static_cast (ACE_Message_Block *, arg);

  if (queue_.enqueue_tail (message) < 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "Bounded_Packet_Relay<SYNCH>::receive_input failed"), 
                      -1);
  }

  return 0;
}
  // public entry point to which to push input.

// Parse the input and execute the corresponding command

template <class TQ, class RECEIVER, class ACTION> int
Bounded_Packet_Relay_Driver<TQ, RECEIVER, ACTION>::parse_commands (const char *buf)
{
  int option;

  if (::sscanf (buf, "%d", &option) <= 0)
    // If there was an error reading the option simply try on the next line.
    return 0;

  switch (option)
  {
    case 1:  // set packet count
    {
      u_long count;

      // We just reread the option, this simplies parsing
      // (since sscanf can do it for us.)
      if (::sscanf (buf, "%d %lu", &option, &count) < 2)
      {
        // if there was not enough information on the line, 
        // ignore option and try the next line.
        return 0;
      }

      if (packet_count_cmd_->execute ((void *) &count) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "set packet count failed"), -1);
      }

      break;
    }

    case 2:  // set arrival period
    {
      u_long usec;

      // We just reread the option, this simplies parsing
      // (since sscanf can do it for us.)
      if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
      {
        // if there was not enough information on the line, 
        // ignore option and try the next line.
        return 0;
      }

      if (arrival_period_cmd_->execute ((void *) &usec) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "set arrival period failed"), -1);
      }

      break;
    }

    case 3:  // set transmit period
    {
      u_long usec;

      // We just reread the option, this simplies parsing
      // (since sscanf can do it for us.)
      if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
      {
        // if there was not enough information on the line, 
        // ignore option and try the next line.
        return 0;
      }

      if (transmit_period_cmd_->execute ((void *) &usec) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "set transmit period failed"), -1);
      }

      break;
    }

    case 4:  // set duration limit
    {
      u_long usec;

      // We just reread the option, this simplies parsing
      // (since sscanf can do it for us.)
      if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
      {
        // if there was not enough information on the line, 
        // ignore option and try the next line.
        return 0;
      }

      if (duration_limit_cmd_->execute ((void *) &usec) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "set duration limit failed"), -1);
      }

      break;
    }

    case 5:  // set logging level
    {
      u_long level;

      // We just reread the option, this simplies parsing
      // (since sscanf can do it for us.)
      if (::sscanf (buf, "%d %lu", &option, &level) < 2)
      {
        // if there was not enough information on the line, 
        // ignore option and try the next line.
        return 0;
      }

      if (logging_level_cmd_->execute ((void *) &level) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "set logging level failed"), -1);
      }

      break;
    }

    case 6: // run one transmission
      return run_transmission_cmd_->execute (0);
      /* NOTREACHED */

    case 7: // report statistics
      return report_stats_cmd_->execute (0);
      /* NOTREACHED */

    case 8: // shut down the driver
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

template <class TQ, class RECEIVER, class ACTION> int
Bounded_Packet_Relay_Driver<TQ, RECEIVER, ACTION>::run (void)
{
  this->init ();

  for (;;)
    if (this->get_next_request () == -1)
      return -1;

  ACE_NOTREACHED (return 0);
}

// gets the next request from the user input.

template <class TQ, class RECEIVER, class ACTION> int
Bounded_Packet_Relay_Driver<TQ, RECEIVER, ACTION>::get_next_request (void)
{
  char buf[BUFSIZ];

  this->display_menu ();

  ACE_OS::fprintf (ACE_STDERR, "Please enter your choice: ");
  ACE_OS::fflush (ACE_STDERR);

  // reads input from the user
  if (this->read_input (buf, sizeof buf) <= 0)
    return -1;

  // Parse and run the command.
  return this->parse_commands (buf);
}

// Reads input from the user from ACE_STDIN into the buffer specified.

template <class TQ, class RECEIVER, class ACTION> ssize_t
Bounded_Packet_Relay_Driver<TQ, RECEIVER, ACTION>::read_input (char *buf, size_t bufsiz)
{
  ACE_OS::memset (buf, 0, bufsiz);

  // Wait for user to type commands.  This call is automatically
  // restarted when SIGINT or SIGALRM signals occur.
  return ACE_OS::read (ACE_STDIN, buf, bufsiz);
}

#endif /* _BPR_DRIVER_CPP_ */
