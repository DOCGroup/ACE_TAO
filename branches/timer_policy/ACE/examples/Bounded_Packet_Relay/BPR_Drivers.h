/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    BPR_Drivers.h
 *
 *  $Id$
 *
 *  This code builds abstractions to factor out common code from
 *  the different possible implementations of the Timer_Queue based
 *  bounded packet relay example.
 *
 *
 *  @author Chris Gill           <cdgill@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> Based on the Timer Queue Test example written by Carlos O'Ryan        <coryan@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BPR_DRIVERS_H_
#define _BPR_DRIVERS_H_

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Reactor.h"
#include "ace/Task.h"

// forward declarations
class Input_Device_Wrapper_Base;
class Output_Device_Wrapper_Base;

/**
 * @class Bounded_Packet_Relay
 *
 * @brief This class defines a packet relay abstraction for a
 * transmission bounded external commands to start and end the
 * transmission.  The transmission may be bounded by the number
 * of packets to send, the dration of the transmission, or any
 * other factors.
 *
 * The relay abstraction implemented by this class registers a
 * callback command with an input device wrapper, and relays
 * input to an output device at a pace specified in the start
 * transmission call.
 */
class Bounded_Packet_Relay
{
public:
  // = Enumerates possible status values for a transmission.
  enum Transmission_Status
  {
    UN_INITIALIZED,
    STARTED,
    COMPLETED,
    TIMED_OUT,
    CANCELLED,
    ERROR_DETECTED
  };

  enum Queue_Defaults
  {
    DEFAULT_HWM = 0x7FFFFFFF,
    DEFAULT_LWM = 0x7FFFFFFF
  };

  /// Command entry point type definition.
  typedef int (Bounded_Packet_Relay::*ACTION) (void *);

  // = Initialization method

  /// Constructor.
  Bounded_Packet_Relay (ACE_Thread_Manager *input_task_mgr,
                        Input_Device_Wrapper_Base *input_wrapper,
                        Output_Device_Wrapper_Base *output_wrapper);

  /// Destructor.
  virtual ~Bounded_Packet_Relay (void);

  /// Requests output be sent to output device.
  int send_input (void);

  /// Requests a transmission be started.
  int start_transmission (u_long packet_count,
                          u_long arrival_period,
                          int logging_level);

  /// Requests a transmission be ended.
  int end_transmission (Transmission_Status status);

  /// Requests a report of statistics from the last transmission.
  int report_statistics (void);

  // = Command accessible entry points.

  /// Public entry point to which to push input.
  int receive_input (void *);

  // = Accessors and mutators for relay settings

  /// Get high water mark for relay queue.
  ACE_UINT32 queue_hwm (void);

  /// Set high water mark for relay queue.
  void queue_hwm (ACE_UINT32 hwm);

  /// Get low water mark for relay queue.
  ACE_UINT32 queue_lwm (void);

  /// Set low water mark for relay queue.
  void queue_lwm (ACE_UINT32 lwm);

private:
  // = Concurrency Management.

  /// flag for whether or not a transmission is active
  int is_active_;

  /// Thread manager for the input device task.
  ACE_Thread_Manager * input_task_mgr_;

  /// Pointer to the input device wrapper.
  Input_Device_Wrapper_Base * input_wrapper_;

  /// Pointer to the output device wrapper.
  Output_Device_Wrapper_Base * output_wrapper_;

  /// Queue used to buffer input messages.
  ACE_Message_Queue<ACE_SYNCH> queue_;

  /// High water mark for relay queue.
  ACE_UINT32 queue_hwm_;

  /// Low water mark for relay queue.
  ACE_UINT32 queue_lwm_;

  /// Lock for thread-safe synchronization of transmission startup and
  /// termination.
  ACE_SYNCH_MUTEX transmission_lock_;

  // = Transmission Statistics

  /// Returns string corresponding to current status.
  const char *status_msg (void);

  /// Number of transmissions sent.
  u_long transmission_number_;

  /// Count of packets sent in the most recent transmission.
  u_long packets_sent_;

  /// Status of the current or most recent transmission.
  Transmission_Status status_;

  /// Start time of the most recent transmission.
  ACE_Time_Value transmission_start_;

  /// Ending time of the most recent transmission.
  ACE_Time_Value transmission_end_;

};

/**
 * @class Input_Device_Wrapper_Base
 *
 * @brief This class defines an abstract base class for an input device
 * wrapper that hides the details of the specific device and
 * provides a consistent message passing interface without
 * knowing anything about the implementation of the input device
 * or the message receiver.
 * The abstract base class ctor takes a command template object
 * that is instantiated with the correct receiver and action
 * types. This command object is used to send newly created input
 * messages to the receiver.
 * The abstract base class is designed to operate in an active
 * "push" mode, sending input data to the receiver whenever the
 * data is ready.  The underlying device may be active, notifying
 * the wrapper when data is ready, or may be passive in which
 * case the wrapper must rely on a reactive and/or polling
 * mechanism.
 *
 * Derived classes are responsible for filling in concrete
 * definitions for the abstract message creation method and the
 * svc method.
 */
class Input_Device_Wrapper_Base : public ACE_Task_Base
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Input_Device_Wrapper_Base (ACE_Thread_Manager *input_task_mgr);

  /// Destructor.
  virtual ~Input_Device_Wrapper_Base ();

  /// Sets send input message command in the input device driver
  /// object.
  int set_send_input_msg_cmd (ACE_Command_Base *send_input_msg_cmd);

  /// Sets period (in usecs) between when inputs are created.
  int set_input_period (u_long input_period);

  /// Sets count of messages to send.
  int set_send_count (long count);

  /**
   * Requests that the input device stop sending messages and
   * terminate its thread.  Should return 1 if it will do so, 0 if it
   * has already done so, or -1 if there is a problem doing so.
   */
  int request_stop (void);

  /// This method runs the input device loop in the new thread.
  virtual int svc (void);

  /// Provides an abstract interface to allow modifying device
  /// settings.
  virtual int modify_device_settings (void *) = 0;

protected:
  /// Creates a new message block, carrying data read from the
  /// underlying input device.
  virtual ACE_Message_Block *create_input_message (void) = 0;

  /**
   * Sends a newly created message block, carrying data read from the
   * underlying input device, by passing a pointer to the message
   * block to its command execution.
   */
  virtual int send_input_message (ACE_Message_Block *);

  /// Send newly created input message.
  ACE_Command_Base *send_input_msg_cmd_;

  /// Period between when input values are produced (usecs).
  u_long input_period_;

  /// Reactor used to multiplex input streams, timeouts.
  ACE_Reactor reactor_;

  /// Flag to indicate whether or not input object is
  /// (and should remain) active.
  int is_active_;

  /// Count of messages to send before stopping (-1 indicates the
  /// device should not stop).
  long send_count_;

  /// Currently remaining count of messages to send before stopping
  /// (-1 indicates the device should not stop).
  long current_count_;

};

/**
 * @class Output_Device_Wrapper_Base
 *
 * @brief This class defines an abstract base class for an output device
 * wrapper that hides the details of the specific device and
 * provides a consistent write method interface without knowing
 * anything about the implementation.
 *
 * The abstract methods write_output_message () and
 * modify_device_settings () are defined in derived classes to
 * write the contents of the passed message out the underlying
 * output device, and update device settings, respectively.
 */
class Output_Device_Wrapper_Base
{
public:

  virtual ~Output_Device_Wrapper_Base (void);

  /// Writes contents of the passed message block out to the underlying
  /// output device.
  virtual int write_output_message (void *) = 0;

  /// Provides an abstract interface to allow modifying device
  /// settings.
  virtual int modify_device_settings (void *) = 0;
};

// include the templates
#include "BPR_Drivers_T.h"

#endif /* _BPR_DRIVERS_H_ */
