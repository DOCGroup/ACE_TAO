
//=============================================================================
/**
 *  @file    BPR_Drivers.cpp
 *
 *  $Id$
 *
 *  This code builds an abstraction to factor out common code for
 *  the different implementations of the Timer_Queue.
 *
 *
 *  @author Chris Gill           <cdgill@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> Based on the Timer Queue Test example written by Carlos O'Ryan        <coryan@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_sys_time.h"
#include "BPR_Drivers.h"



// Constructor.

Input_Device_Wrapper_Base::Input_Device_Wrapper_Base (ACE_Thread_Manager *input_task_mgr)
  : ACE_Task_Base (input_task_mgr),
    send_input_msg_cmd_ (0),
    input_period_ (ACE_ONE_SECOND_IN_USECS),
    is_active_ (0),
    send_count_ (0)
{
}

// Destructor.

Input_Device_Wrapper_Base::~Input_Device_Wrapper_Base (void)
{
}

// Sets send input message command in the input device driver object.

int
Input_Device_Wrapper_Base::set_send_input_msg_cmd (ACE_Command_Base *send_input_msg_cmd)
{
  // Set the new command.  Input device is not responsible
  // for deleting the old command, if any.
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
  ACE_Time_Value timeout;
  ACE_Message_Block *message;

  // Set a flag to indicate we're active.
  is_active_ = 1;

  // Start with the total count of messages to send.
  for (current_count_ = send_count_;
       // While we're still marked active, and there are packets to send.
       (is_active_) && (current_count_ != 0);
       )
    {
      // Create an input message to send.
      message = create_input_message ();
      if (message == 0)
        {
          if (is_active_)
            {
              is_active_ = 0;
              ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                                 "Failed to create input message object"),
                                 -1);
            }

          break;
        }

      // Make sure there is a send command object.
      if (send_input_msg_cmd_ == 0)
        {
          delete message;
          if (is_active_)
            {
              is_active_ = 0;
              ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                                 "send message command object not instantiated"),
                                -1);
            }

          break;
        }

      // Send the input message.
      if (send_input_msg_cmd_->execute ((void *) message) < 0)
        {
          delete message;
          if (is_active_)
            {
              is_active_ = 0;
              ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                                 "Failed executing send message command object"),
                                -1);
            }

          break;
        }

      // If all went well, decrement count of messages to send, and
      // run the reactor event loop unti we get a timeout or something
      // happens in a registered upcall.
      if (current_count_ > 0)
        --current_count_;

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
    {
      if (is_active_)
        ACE_ERROR ((LM_ERROR, "%t %p\n",
                    "Input_Device_Wrapper_Base::send_input_message: "
                    "command object not instantiated"));

      return -1;
    }
}

Output_Device_Wrapper_Base::~Output_Device_Wrapper_Base (void)
{
}

// Constructor.

Bounded_Packet_Relay::Bounded_Packet_Relay (ACE_Thread_Manager *input_task_mgr,
                                            Input_Device_Wrapper_Base *input_wrapper,
                                            Output_Device_Wrapper_Base *output_wrapper)
  : is_active_ (0),
    input_task_mgr_ (input_task_mgr),
    input_wrapper_ (input_wrapper),
    output_wrapper_ (output_wrapper),
    queue_ (Bounded_Packet_Relay::DEFAULT_HWM,
            Bounded_Packet_Relay::DEFAULT_LWM),
    queue_hwm_ (Bounded_Packet_Relay::DEFAULT_HWM),
    queue_lwm_ (Bounded_Packet_Relay::DEFAULT_LWM),
    transmission_number_ (0),
    packets_sent_ (0),
    status_ (Bounded_Packet_Relay::UN_INITIALIZED),
    transmission_start_ (ACE_Time_Value::zero),
    transmission_end_ (ACE_Time_Value::zero)
{
  if (input_task_mgr_ == 0)
    input_task_mgr_ = ACE_Thread_Manager::instance ();
}

// Destructor.

Bounded_Packet_Relay::~Bounded_Packet_Relay (void)
{
  // Reactivate the queue, and then clear it.
  queue_.activate ();
  while (! queue_.is_empty ())
    {
      ACE_Message_Block *msg;
      queue_.dequeue_head (msg);
      delete msg;
    }

}

// Requests output be sent to output device.

int
Bounded_Packet_Relay::send_input (void)
{
  // Don't block, return immediately if queue is empty.
  ACE_Message_Block *item;

  // Using a separate (non-const) time value
  // is necessary on some platforms
  ACE_Time_Value immediate (ACE_Time_Value::zero);

  if (queue_.dequeue_head (item,
                           &immediate) < 0)
    return 1;

  // If a message block was dequeued, send it to the output device.

  if (output_wrapper_->write_output_message ((void *) item) < 0)
    {
      if (is_active_)
        ACE_ERROR ((LM_ERROR,
                    "%t %p\n",
                    "failed to write to output device object"));

      return -1;
    }

  // If all went OK, increase count of packets sent.
  ++packets_sent_;
  return 0;
}

// Requests a transmission be started.

int
Bounded_Packet_Relay::start_transmission (u_long packet_count,
                                          u_long arrival_period,
                                          int logging_level)
{
  // Serialize access to start and end transmission calls, statistics
  // reporting calls.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->transmission_lock_, -1);

  // If a transmission is already in progress, just return.
  if (is_active_)
    return 1;

  // Set transmission in progress flag true.
  is_active_ = 1;

  // Update statistics for a new transmission.
  ++transmission_number_;
  packets_sent_ = 0;
  status_ = STARTED;
  transmission_start_ = ACE_OS::gettimeofday ();

  // Reactivate the queue, and then clear it.
  queue_.activate ();
  while (! queue_.is_empty ())
    {
      ACE_Message_Block *msg;
      queue_.dequeue_head (msg);
      delete msg;
    }

  // Initialize the output device.
  if (output_wrapper_->modify_device_settings ((void *) &logging_level) < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();
      is_active_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                         "failed to initialize output device object"),
                        -1);
    }
  // Initialize the input device.
  if (input_wrapper_->modify_device_settings ((void *) &logging_level) < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();
      is_active_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                         "failed to initialize output device object"),
                        -1);
    }
  else if (input_wrapper_->set_input_period (arrival_period) < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();
      is_active_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                         "failed to initialize input device object"),
                        -1);
    }
  else if (input_wrapper_->set_send_count (packet_count) < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();
      is_active_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                         "failed to initialize input device object"),
                        -1);
    }
  // Activate the input device.
  else if (input_wrapper_->activate () < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();
      is_active_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                         "failed to activate input device object"),
                        -1);
    }

  // If all went well, print a startup message and return success.
  ACE_DEBUG ((LM_DEBUG,
              "\n\nTransmission %u started\n\n",
              transmission_number_));
  return 0;
}

// Requests a transmission be ended.

int
Bounded_Packet_Relay::end_transmission (Transmission_Status status)
{
  // Serialize access to start and end transmission calls,
  // statistics reporting calls.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->transmission_lock_, -1);

  // If a transmission is not already in progress, just return.
  if (! is_active_)
    return 1;

  // Set transmission in progress flag false.
  is_active_ = 0;

  // Ask the the input thread to stop.
  if (input_wrapper_->request_stop () < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n",
                         "failed asking input device thread to stop"),
                        -1);
    }

  // Deactivate the queue, allowing all waiting threads to continue.
  queue_.deactivate ();

  // Wait for input thread to stop.
  input_task_mgr_->wait_task (input_wrapper_);

  // Reactivate the queue, and then clear it.
  queue_.activate ();
  while (! queue_.is_empty ())
    {
      ACE_Message_Block *msg;
      queue_.dequeue_head (msg);
      delete msg;
    }

  // If all went well, set passed status, stamp end time, print a
  // termination message, and return success.
  status_ = status;
  transmission_end_ = ACE_OS::gettimeofday ();
  ACE_DEBUG ((LM_DEBUG,
              "\n\nTransmission %u ended with status: %s\n\n",
              transmission_number_, status_msg ()));
  return 0;
}

// Requests a report of statistics from the last transmission.

int
Bounded_Packet_Relay::report_statistics (void)
{
  // Serialize access to start and end transmission calls,
  // statistics reporting calls.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->transmission_lock_, -1);

  // If a transmission is already in progress, just return.
  if (is_active_)
    return 1;

  // Calculate duration of trasmission.
  ACE_Time_Value duration (transmission_end_);
  duration -= transmission_start_;

  // Report transmission statistics.
  ACE_DEBUG ((LM_DEBUG,
              "\n\nStatisics for transmission %u:\n\n"
              "Transmission status: %s\n"
              "Start time:          %d (sec) %d (usec)\n"
              "End time:            %d (sec) %d (usec)\n"
              "Duration:            %d (sec) %d (usec)\n"
              "Packets relayed:     %u\n\n",
              transmission_number_, status_msg (),
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

int
Bounded_Packet_Relay::receive_input (void * arg)
{
  if (! arg)
    {
      if (is_active_)
        ACE_ERROR ((LM_ERROR, "%t %p\n",
                    "Bounded_Packet_Relay::receive_input: "
                    "null argument"));

      return -1;
    }
  ACE_Message_Block *message = static_cast<ACE_Message_Block *> (arg);
  if (queue_.enqueue_tail (message) < 0)
    {
      if (is_active_)
        ACE_ERROR ((LM_ERROR, "%t %p\n",
                    "Bounded_Packet_Relay::receive_input failed"));

      return -1;
    }

  return 0;
}

// Get high water mark for relay queue.

ACE_UINT32
Bounded_Packet_Relay::queue_hwm (void)
{
  return queue_lwm_;
}


// Set high water mark for relay queue.

void
Bounded_Packet_Relay::queue_hwm (ACE_UINT32 hwm)
{
  queue_hwm_ = hwm;
}

// Get low water mark for relay queue.

ACE_UINT32
Bounded_Packet_Relay::queue_lwm (void)
{
  return queue_lwm_;
}

// Set low water mark for relay queue.

void
Bounded_Packet_Relay::queue_lwm (ACE_UINT32 lwm)
{
  queue_lwm_ = lwm;
}



// Returns string corresponding to current status.

const char *
Bounded_Packet_Relay::status_msg (void)
{
  const char *status_msg;
  switch (status_)
    {
      case UN_INITIALIZED:
        status_msg = "uninitialized";
        break;
      case STARTED:
        status_msg = "in progress";
        break;
      case COMPLETED:
        status_msg = "completed with all packets sent";
        break;
      case TIMED_OUT:
        status_msg = "terminated by transmission duration timer";
        break;
      case CANCELLED:
        status_msg = "cancelled by external control";
        break;
      case ERROR_DETECTED:
        status_msg = "error was detected";
        break;
      default:
        status_msg = "unknown transmission status";
        break;
    }

  return status_msg;
}
