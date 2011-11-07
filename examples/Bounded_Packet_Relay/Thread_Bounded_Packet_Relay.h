/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Thread_Bounded_Packet_Relay.h
 *
 *  $Id$
 *
 *  This code provides a thread based implementation
 *  of the bounded packet relay example.
 *
 *
 *  @author Chris Gill           <cdgill@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> Based on the Timer Queue Test example written by Carlos O'Ryan        <coryan@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#ifndef _THREAD_BOUNDED_PACKET_RELAY_H_
#define _THREAD_BOUNDED_PACKET_RELAY_H_

#include "ace/Functor_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"
#include "BPR_Drivers.h"

// These typedefs ensure that we use the minimal amount of locking
// necessary.
typedef ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>
        Upcall;
typedef ACE_Timer_Heap_T<ACE_Event_Handler *,
                         Upcall,
                         ACE_Null_Mutex>
        Timer_Heap;
typedef ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *,
                                  Upcall,
                                  ACE_Null_Mutex>
        Timer_Heap_Iterator;
typedef ACE_Thread_Timer_Queue_Adapter<Timer_Heap>
        Thread_Timer_Queue;

// Forward declaration.
class Thread_Bounded_Packet_Relay_Driver;

/**
 * @class Text_Input_Device_Wrapper
 *
 * @brief Defines a wrapper for a simple active looping text input
 * pseudo-device.
 *
 * The wrapper is an active object, running in its own thread,
 * and uses a reactor to generate timeouts.  When a timeout
 * occurs, the wrapper calls its concrete message creation
 * method. The wrapper then calls its base class message send
 * method to forward the message to the receiver.
 * A more sophisticated version of this class would use the
 * reactive capabilities as well as the timeout generating
 * capabilities of the reactor, multiplexing several input
 * streams.  Comments to this effect appear in the definition of
 * the event loop method.
 */
class Text_Input_Device_Wrapper : public Input_Device_Wrapper_Base
{
public:

  // = Enumerated logging level flags
  enum Logging_Flags {NO_LOGGING = 0,
                      LOG_MSGS_CREATED = 1};

  // = Initialization and termination methods.
  /// Constructor.
  Text_Input_Device_Wrapper (ACE_Thread_Manager *input_task_mgr,
                             size_t read_length,
                             const char* text,
                             int logging = 0);

  /// Destructor.
  virtual ~Text_Input_Device_Wrapper (void);

  /**
   * Modifies device settings based on passed pointer to a u_long.
   * Turns logging on if u_long is non-zero, off if u_long is zero,
   * and does nothing if the pointer is null.
   */
  virtual int modify_device_settings (void *logging);

protected:
  /// Creates a new message block, carrying data read from the
  /// underlying input device.
  virtual ACE_Message_Block *create_input_message (void);

private:
  /// Length of the buffer into which to "read".
  size_t read_length_;

  /// Text to "read" into the buffer.
  const char *text_;

  /// Index into the string.
  size_t index_;

  /// This value is 0 if logging is turned off, non-zero otherwise
  int logging_;

  /// This value holds a count of packets created.
  u_long packet_count_;

};

/**
 * @class Text_Output_Device_Wrapper
 *
 * @brief Implements a simple wrapper for a output pseudo-device.
 *
 * Data from the passed output message is printed to the standard
 * output stream, if logging is turned on.
 */
class Text_Output_Device_Wrapper : public Output_Device_Wrapper_Base
{
public:

  // = Enumerated logging level flags
  enum Logging_Flags {NO_LOGGING = 0,
                      LOG_MSGS_RCVD = 2,
                      PRINT_MSGS_RCVD = 4};

  /// Default constructor.
  Text_Output_Device_Wrapper (int logging = 0);

  // = Command Accessible Entry Points

  /// Consumes and possibly prints out the passed message.
  virtual int write_output_message (void *message);

  /**
   * Modifies device settings based on passed pointer to a u_long.
   * Turns logging on if u_long is non-zero, off if u_long is zero,
   * and does nothing if the pointer is null.
   */
  virtual int modify_device_settings (void *logging);

private:

  /// This value holds the logging level.
  int logging_;

  /// This value holds a count of packets received.
  u_long packet_count_;

};

/**
 * @class User_Input_Task
 *
 * @brief Read user actions on the Timer_Queue from stdin.
 *
 * This class reads user input from stdin. The commands allow
 * the control of a Timer_Queue, which is dispatched by another
 * thread.
 */
class User_Input_Task : public ACE_Task_Base
{
public:

  // = Trait for command accessible entry points.

  typedef int (User_Input_Task::*ACTION) (void *);

  /// Constructor.
  User_Input_Task (Bounded_Packet_Relay *relay,
                   Thread_Timer_Queue *queue,
                   Thread_Bounded_Packet_Relay_Driver &timer_queue_driver);

  /// Destructor.
  virtual ~User_Input_Task (void);

  /// This method runs the event loop in the new thread.
  virtual int svc (void);

  // = Some helper methods.

  /// Sets the number of packets for the next transmission.
  int set_packet_count (void *);

  /// Sets the input device packet arrival period (usecs) for the next
  /// transmission.
  int set_arrival_period (void *);

  /// Sets the period between output device sends (usecs) for the next
  /// transmission.
  int set_send_period (void *);

  /// Sets a limit on the transmission duration (usecs).
  int set_duration_limit (void *);

  /// Sets logging level (0 or 1) for output device for the next
  /// transmission.
  int set_logging_level (void *);

  /// Runs the next transmission (if one is not in progress).
  int run_transmission (void *);

  /// Ends the current transmission (if one is in progress).
  int end_transmission (void *);

  /// Reports statistics for the previous transmission (if one is not
  /// in progress).
  int report_stats (void *);

  /// Shuts down the task.
  int shutdown (void *);

  /// Helper method: clears all timers.
  int clear_all_timers (void);

private:
  /// How many microseconds are in a second.
  const int usecs_;

  /// The bounded packet relay.
  Bounded_Packet_Relay *relay_;

  /// The timer queue implementation.
  Thread_Timer_Queue *queue_;

  /// The thread timer queue test driver.
  Thread_Bounded_Packet_Relay_Driver &driver_;
};

/**
 * @class BPR_Handler_Base
 *
 * @brief Base event handler class for bounded packet relay example.
 *
 * The base class provides a helper method that derived classes
 * can register as a deferred execution callback that will cancel
 * all timers in the underlying timer queue, and then delete "this".
 */
class BPR_Handler_Base : public ACE_Event_Handler
{
public:

  // = Trait for command accessible entry points.

  typedef int (BPR_Handler_Base::*ACTION) (void *);


  /// Constructor.
  BPR_Handler_Base (Bounded_Packet_Relay &relay,
                    Thread_Timer_Queue &queue);

  /// Destructor.
  virtual ~BPR_Handler_Base (void);

  // = Command accessible entry points.

  /// Helper method: clears all timers.
  virtual int clear_all_timers (void *);

protected:
  /// Stores a reference to the relay object on which to invoke
  /// the appropritate calls when the timer expires.
  Bounded_Packet_Relay &relay_;

  /// Store a reference to the timer queue, in which to re-register
  /// the send timer and handler if there are still sends to perform.
  Thread_Timer_Queue &queue_;
};

class Send_Handler;

/**
 * @class Send_Handler
 *
 * @brief Event handler for message send timeout events.
 *
 * The <handle_timeout> hook method calls the relay's send
 * method and decrements its count of messages to send.
 * If there are still messages to send, it re-registers itself
 * with the timer queue.  Otherwise it calls the relay's end
 * transmission method, and registers a deferred execution
 * callback to clear the timer queue, and then delete "this".
 */
class Send_Handler : public BPR_Handler_Base
{
public:

  // = Trait for command accessible entry points.

  typedef int (Send_Handler::*ACTION) (void *);

  /// Constructor.
  Send_Handler (u_long send_count,
                const ACE_Time_Value &duration,
                Bounded_Packet_Relay &relay,
                Thread_Timer_Queue &queue,
                Thread_Bounded_Packet_Relay_Driver &driver);

  /// Destructor.
  virtual ~Send_Handler (void);

  /// Call back hook.
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);

  /// Cancellation hook.
  virtual int cancelled (void);

  // = Command accessible entry points.

  /// Helper method: re-registers this handler.
  virtual int reregister (void *timeout);

private:

  /// Count of the number of messages to send from the
  /// relay object to the output device object.
  u_long send_count_;

  /// Stores the expected duration until expiration, and is used to
  /// re-register the handler if there are still sends to perform.
  ACE_Time_Value duration_;

  /// Reference to the driver that will redisplay the user input menu.
  Thread_Bounded_Packet_Relay_Driver &driver_;
};

/**
 * @class Termination_Handler
 *
 * @brief Event handler for end transmission timeout events.
 *
 * The <handle_timeout> hook method calls the relay's end
 * transmission method, then registers a deferred execution
 * callback to clear all timers and then delete "this".
 */
class Termination_Handler : public BPR_Handler_Base
{
public:
  /// Constructor.
  Termination_Handler (Bounded_Packet_Relay &relay,
                       Thread_Timer_Queue &queue,
                       Thread_Bounded_Packet_Relay_Driver &driver);

  /// Destructor.
  virtual ~Termination_Handler (void);

  /// Call back hook.
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);

  /// Cancellation hook.
  virtual int cancelled (void);

private:
  /// Reference to the driver that will redisplay the user input menu.
  Thread_Bounded_Packet_Relay_Driver &driver_;
};

/**
 * @class Thread_Bounded_Packet_Relay_Driver
 *
 * @brief Implements an example application that exercises
 * <Thread_Timer_Queue> timer queue.
 *
 * This class implements a simple test driver for the
 * <Thread_Timer_Queue>.  The <display_menu> hook method is
 * called from the base class to print a menu specific to the
 * thread implementation of the timer queue.
 */
class Thread_Bounded_Packet_Relay_Driver : public Bounded_Packet_Relay_Driver <Thread_Timer_Queue>
{
public:

  // = Trait for commands issued from this driver

  typedef ACE_Command_Callback<User_Input_Task, User_Input_Task::ACTION> MYCOMMAND;

  // = Initialization and termination methods.

  /// Constructor.
  Thread_Bounded_Packet_Relay_Driver (Bounded_Packet_Relay *relay);

  /// Destructor.
  virtual ~Thread_Bounded_Packet_Relay_Driver (void);

  /// Displays the user menu.
  virtual int display_menu (void);

  /// Initializes the driver.
  virtual int init (void);

  /// Run the driver.
  virtual int run (void);

private:
  /// User input task, subclassed from ACE_Task.
  User_Input_Task input_task_;
};

#endif /* _THREAD_BOUNDED_PACKET_RELAY_H_ */
