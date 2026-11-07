/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Thread_Bounded_Packet_Relay.h
 *
 *  This code provides a thread based implementation
 *  of the bounded packet relay example.
 *
 *  @author Chris Gill <cdgill@cs.wustl.edu> and Douglas C. Schmidt
 *          <d.schmidt@vanderbilt.edu>. Based on Timer Queue Test by Carlos O'Ryan
 *          <coryan@cs.wustl.edu>, Douglas C. Schmidt <d.schmidt@vanderbilt.edu>,
 *          and Sergio Flores-Gaitan <sergio@cs.wustl.edu>.
 */
//=============================================================================


#ifndef _THREAD_BOUNDED_PACKET_RELAY_H_
#define _THREAD_BOUNDED_PACKET_RELAY_H_

#include "ace/Functor_T.h"
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"
#include "BPR_Drivers.h"

// These aliases ensure that we use the minimal amount of locking
// necessary.
using Upcall = ACE_Event_Handler_Handle_Timeout_Upcall;
using Timer_Heap = ACE_Timer_Heap_T<ACE_Event_Handler *, Upcall, ACE_Null_Mutex>;
using Timer_Heap_Iterator = ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, Upcall, ACE_Null_Mutex>;
using Thread_Timer_Queue = ACE_Thread_Timer_Queue_Adapter<Timer_Heap>;

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

  /// Constructor.
  Text_Input_Device_Wrapper (ACE_Thread_Manager *input_task_mgr,
                             std::string const &text,
                             int logging = 0);

  Text_Input_Device_Wrapper (Text_Input_Device_Wrapper const &) = delete;
  Text_Input_Device_Wrapper (Text_Input_Device_Wrapper &&) = delete;
  Text_Input_Device_Wrapper &operator= (Text_Input_Device_Wrapper const &) = delete;
  Text_Input_Device_Wrapper &operator= (Text_Input_Device_Wrapper &&) = delete;

  /// Destructor.
  ~Text_Input_Device_Wrapper () override = default;

  /**
   * Modifies device settings based on passed pointer to a u_long.
   * Turns logging on if u_long is non-zero, off if u_long is zero,
   * and does nothing if the pointer is null.
   */
  int modify_device_settings (void *logging) override;

protected:
  /// Creates a new message block, carrying data read from the
  /// underlying input device.
  ACE_Message_Block *create_input_message () override;

private:
  /// Owns the text used by this pseudo-device.
  std::string text_;

  /// Index into the string.
  size_t index_ {};

  /// This value is 0 if logging is turned off, non-zero otherwise
  int logging_ {};

  /// This value holds a count of packets created.
  u_long packet_count_ {};
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
  explicit Text_Output_Device_Wrapper (int logging = 0);

  Text_Output_Device_Wrapper (Text_Output_Device_Wrapper const &) = delete;
  Text_Output_Device_Wrapper (Text_Output_Device_Wrapper &&) = delete;
  Text_Output_Device_Wrapper &operator= (Text_Output_Device_Wrapper const &) = delete;
  Text_Output_Device_Wrapper &operator= (Text_Output_Device_Wrapper &&) = delete;

  // = Command Accessible Entry Points

  /// Consumes and possibly prints out the passed message.
  int write_output_message (ACE_Message_Block *message) override;

  /**
   * Modifies device settings based on passed pointer to a u_long.
   * Turns logging on if u_long is non-zero, off if u_long is zero,
   * and does nothing if the pointer is null.
   */
  int modify_device_settings (void *logging) override;

private:
  /// This value holds the logging level.
  int logging_ {};

  /// This value holds a count of packets received.
  u_long packet_count_ {};
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

  using ACTION = int (User_Input_Task::*) (void *);

  /// Constructor.
  User_Input_Task (Bounded_Packet_Relay *relay,
                   Thread_Timer_Queue *queue,
                   Thread_Bounded_Packet_Relay_Driver &timer_queue_driver);

  User_Input_Task (User_Input_Task const &) = delete;
  User_Input_Task (User_Input_Task &&) = delete;
  User_Input_Task &operator= (User_Input_Task const &) = delete;
  User_Input_Task &operator= (User_Input_Task &&) = delete;

  /// Destructor.
  ~User_Input_Task () override;

  /// This method runs the event loop in the new thread.
  int svc () override;

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
  int clear_all_timers ();

private:
  /// How many microseconds are in a second.
  static constexpr int usecs_ {ACE_ONE_SECOND_IN_USECS};

  /// Borrowed bounded packet relay.
  Bounded_Packet_Relay *relay_ {};

  /// Borrowed timer queue implementation.
  Thread_Timer_Queue *queue_ {};

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
 * all timers in the underlying timer queue, and then delete "this". The timer
 * queue keeps a non-owning handler pointer; callbacks and cancellation paths
 * delete each registered handler, so these objects must not have another owner.
 */
class BPR_Handler_Base : public ACE_Event_Handler
{
public:
  // = Trait for command accessible entry points.

  using ACTION = int (BPR_Handler_Base::*) (void *);


  /// Constructor.
  BPR_Handler_Base (Bounded_Packet_Relay &relay,
                    Thread_Timer_Queue &queue);

  BPR_Handler_Base (BPR_Handler_Base const &) = delete;
  BPR_Handler_Base (BPR_Handler_Base &&) = delete;
  BPR_Handler_Base &operator= (BPR_Handler_Base const &) = delete;
  BPR_Handler_Base &operator= (BPR_Handler_Base &&) = delete;

  /// Destructor.
  ~BPR_Handler_Base () override = default;

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

  using ACTION = int (Send_Handler::*) (void *);

  /// Constructor.
  Send_Handler (u_long send_count,
                ACE_Time_Value const &duration,
                Bounded_Packet_Relay &relay,
                Thread_Timer_Queue &queue,
                Thread_Bounded_Packet_Relay_Driver &driver);

  Send_Handler (Send_Handler const &) = delete;
  Send_Handler (Send_Handler &&) = delete;
  Send_Handler &operator= (Send_Handler const &) = delete;
  Send_Handler &operator= (Send_Handler &&) = delete;

  /// Destructor.
  ~Send_Handler () override = default;

  /// Call back hook.
  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *arg) override;

  /// Cancellation hook.
  virtual int cancelled ();

  // = Command accessible entry points.

  /// Helper method: re-registers this handler.
  virtual int reregister (void *timeout);

private:
  /// Count of the number of messages to send from the
  /// relay object to the output device object.
  u_long send_count_ {};

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

  Termination_Handler (Termination_Handler const &) = delete;
  Termination_Handler (Termination_Handler &&) = delete;
  Termination_Handler &operator= (Termination_Handler const &) = delete;
  Termination_Handler &operator= (Termination_Handler &&) = delete;

  /// Destructor.
  ~Termination_Handler () override = default;

  /// Call back hook.
  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *arg) override;

  /// Cancellation hook.
  virtual int cancelled ();

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
  using MYCOMMAND = ACE_Command_Callback<User_Input_Task, User_Input_Task::ACTION>;

  /// Constructor.
  explicit Thread_Bounded_Packet_Relay_Driver (Bounded_Packet_Relay *relay);

  Thread_Bounded_Packet_Relay_Driver (Thread_Bounded_Packet_Relay_Driver const &) = delete;
  Thread_Bounded_Packet_Relay_Driver (Thread_Bounded_Packet_Relay_Driver &&) = delete;
  Thread_Bounded_Packet_Relay_Driver &operator= (Thread_Bounded_Packet_Relay_Driver const &) = delete;
  Thread_Bounded_Packet_Relay_Driver &operator= (Thread_Bounded_Packet_Relay_Driver &&) = delete;

  /// Destructor.
  ~Thread_Bounded_Packet_Relay_Driver () override = default;

  /// Displays the user menu.
  int display_menu () override;

  /// Initializes the driver.
  int init () override;

  /// Run the driver.
  int run () override;

private:
  /// User input task, subclassed from ACE_Task.
  User_Input_Task input_task_;
};

#endif /* _THREAD_BOUNDED_PACKET_RELAY_H_ */
