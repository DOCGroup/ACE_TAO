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

#if !defined (_BPR_DRIVER_T_CPP_)
#define _BPR_DRIVER_T_CPP_

// #include BPR_Drivers.h instead of BPR_Drivers_T.h
// to avoid problems with circular includes
#include "BPR_Drivers.h"

ACE_RCSID(Bounded_Packet_Relay, BPR_Drivers_T, "$Id$")

// Constructor.

template <class RECEIVER, class ACTION>
Command<RECEIVER, ACTION>::Command (RECEIVER &recvr,
                                    ACTION action)
  : receiver_ (recvr),
    action_ (action)
{
}

// Invokes an operation.

template <class RECEIVER, class ACTION> int
Command<RECEIVER, ACTION>::execute (void *arg)
{
  return (receiver_.*action_) (arg);
}


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
    transmission_start_ (ACE_Time_Value::zero),
    transmission_end_ (ACE_Time_Value::zero)
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

  // Using a separate (non-const) time value 
  // is necessary on some platforms
  ACE_Time_Value immediate (ACE_Time_Value::zero);

  if (queue_.dequeue_head (item,
                           &immediate) < 0)
    return 1;

  // If a message block was dequeued, send it to the output device.

  if (output_wrapper_->write_output_message ((void *) item) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%t %p\n", 
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
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed to initialize output device object"), 
                        -1);
    }

  // Initialize the input device.
  if (input_wrapper_->set_input_period (arrival_period) < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed to initialize input device object"), 
                        -1);
    }
  if (input_wrapper_->set_send_count (packet_count) < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed to initialize input device object"), 
                        -1);
    }

  // Activate the input device.
  if (input_wrapper_->activate () < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();  
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
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed asking input device thread to stop"), 
                        -1);
    }
 
  // Deactivate the queue, allowing all waiting threads to continue.
  queue_.deactivate ();

  // Wait for input thread to stop.
  if (input_task_mgr_->wait_task (input_wrapper_) < 0)
    {
      status_ = ERROR_DETECTED;
      transmission_end_ = ACE_OS::gettimeofday ();  
      ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                         "failed waiting for input device thread to stop"), 
                        -1);
    }

  // Reactivate the queue, and then clear it.
  queue_.activate ();
  while (! queue_.is_empty ())
  {
    ACE_Message_Block *msg;
	queue_.dequeue_head (msg);
	delete msg;
  }

  // If all went well, set passed status, stamp end time,
  // print a termination message, and return success.
  status_ = status;
  transmission_end_ = ACE_OS::gettimeofday ();  
  ACE_DEBUG ((LM_DEBUG,
              "\n\nTransmission %u ended with status: %s\n\n",
              transmission_number_, status_msg ()));

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


// Returns string corresponding to current status.

template <ACE_SYNCH_DECL> const char *
Bounded_Packet_Relay<ACE_SYNCH_USE>::status_msg ()
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

// Constructor.

template <class TQ>
Bounded_Packet_Relay_Driver<TQ>::Bounded_Packet_Relay_Driver (void)
  : packet_count_ (1000),
    arrival_period_ (500),
    send_period_ (1000),
    duration_limit_ (1500000),
    logging_level_ (0)
{
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
      case 7: // Cancel current transmission.
        return cancel_transmission_cmd_->execute (0);
        /* NOTREACHED */
      case 8: // Report statistics.
        return report_stats_cmd_->execute (0);
        /* NOTREACHED */
      case 9: // Shut down the driver.
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


// Get count of packets to send in a transmission.

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::packet_count (void)
{
  return packet_count_;
}

// Set count of packets to send in a transmission.

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::packet_count (u_long pc)
{
  packet_count_ = pc;
}

// Get rate at which input packets are to arrive.

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::arrival_period (void)
{
  return arrival_period_;
}

// Set rate at which input packets are to arrive.

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::arrival_period (u_long ap)
{
  arrival_period_ = ap;
}

// Get rate at which packets are to be relayed (usec).

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::send_period (void)
{
  return send_period_;
}

// Set rate at which packets are to be relayed (usec).

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::send_period (u_long sp)
{
  send_period_ = sp;
}

// Get limit on the duration of the transmission (usec).

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::duration_limit (void)
{
  return duration_limit_;
}

// Set limit on the duration of the transmission (usec).

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::duration_limit (u_long dl)
{
  duration_limit_ = dl;
}
// Get logging level.

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::logging_level (void)
{
  return logging_level_;
}

// Set logging level.

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::logging_level (u_long ll)
{
  logging_level_ = ll;
}
#endif /* _BPR_DRIVER_T_CPP_ */


