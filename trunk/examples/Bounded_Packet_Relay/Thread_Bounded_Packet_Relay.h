/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Thread_Bounded_Packet_Relay.h
//
// = DESCRIPTION
//    This code provides a thread based implementation
//    of the bounded packet relay example.
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

#ifndef _THREAD_BOUNDED_PACKET_RELAY_H_
#define _THREAD_BOUNDED_PACKET_RELAY_H_

#define ACE_HAS_DEFERRED_TIMER_COMMANDS

#include "ace/Functor.h"

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

class Text_Input_Device_Wrapper : public Input_Device_Wrapper_Base
{
  // = TITLE
  //    Defines a wrapper for a simple active looping text input
  //    pseudo-device.
  //
  // = DESCRIPTION
  //    The wrapper is an active object, running in its own thread,
  //    and uses a reactor to generate timeouts.  When a timeout
  //    occurs, the wrapper calls its concrete message creation
  //    method. The wrapper then calls its base class message send
  //    method to forward the message to the receiver.
  //
  //    A more sophisticated version of this class would use the
  //    reactive capabilities as well as the timeout generating
  //    capabilities of the reactor, multiplexing several input
  //    streams.  Comments to this effect appear in the definition of
  //    the event loop method.
public:

  // = Enumerated logging level flags
  enum Logging_Flags {NO_LOGGING = 0,
                      LOG_MSGS_CREATED = 1};

  // = Initialization and termination methods.
  Text_Input_Device_Wrapper (ACE_Thread_Manager *input_task_mgr,
                             size_t read_length,
                             const char* text,
                             int logging = 0);
  // Constructor.

  ~Text_Input_Device_Wrapper (void);
  // Destructor.

  virtual int modify_device_settings (void *logging);
  // Modifies device settings based on passed pointer to a u_long.
  // Turns logging on if u_long is non-zero, off if u_long is zero,
  // and does nothing if the pointer is null.

protected:
  virtual ACE_Message_Block *create_input_message (void);
  // Creates a new message block, carrying data read from the
  // underlying input device.

private:
  size_t read_length_;
  // Length of the buffer into which to "read".

  const char *text_;
  // Text to "read" into the buffer.

  size_t index_;
  // Index into the string.

  int logging_;
  // This value is 0 if logging is turned off, non-zero otherwise

  u_long packet_count_;
  // This value holds a count of packets created.

};

class Text_Output_Device_Wrapper : public Output_Device_Wrapper_Base
{
  // = TITLE
  //    Implements a simple wrapper for a output pseudo-device.
  //
  // = DESCRIPTION
  //    Data from the passed output message is printed to the standard
  //    output stream, if logging is turned on.
public:

  // = Enumerated logging level flags
  enum Logging_Flags {NO_LOGGING = 0,
                      LOG_MSGS_RCVD = 2,
                      PRINT_MSGS_RCVD = 4};

  Text_Output_Device_Wrapper (int logging = 0);
  // Default constructor.

  // = Command Accessible Entry Points

  virtual int write_output_message (void *message);
  // Consumes and possibly prints out the passed message.

  virtual int modify_device_settings (void *logging);
  // Modifies device settings based on passed pointer to a u_long.
  // Turns logging on if u_long is non-zero, off if u_long is zero,
  // and does nothing if the pointer is null.

private:

  int logging_;
  // This value holds the logging level.

  u_long packet_count_;
  // This value holds a count of packets received.

};

class User_Input_Task : public ACE_Task_Base
{
  // = TITLE
  //   Read user actions on the Timer_Queue from stdin.
  //
  // = DESCRIPTION
  //   This class reads user input from stdin. The commands allow
  //   the control of a Timer_Queue, which is dispatched by another
  //   thread.
public:

  // = Trait for command accessible entry points.

  typedef int (User_Input_Task::*ACTION) (void *);

  User_Input_Task (Bounded_Packet_Relay *relay,
                   Thread_Timer_Queue *queue,
                   Thread_Bounded_Packet_Relay_Driver &timer_queue_driver);
  // Constructor.

  ~User_Input_Task (void);
  // Destructor.

  virtual int svc (void);
  // This method runs the event loop in the new thread.

  // = Some helper methods.

  int set_packet_count (void *);
  // Sets the number of packets for the next transmission.

  int set_arrival_period (void *);
  // Sets the input device packet arrival period (usecs) for the next
  // transmission.

  int set_send_period (void *);
  // Sets the period between output device sends (usecs) for the next
  // transmission.

  int set_duration_limit (void *);
  // Sets a limit on the transmission duration (usecs).

  int set_logging_level (void *);
  // Sets logging level (0 or 1) for output device for the next
  // transmission.

  int run_transmission (void *);
  // Runs the next transmission (if one is not in progress).

  int end_transmission (void *);
  // Ends the current transmission (if one is in progress).

  int report_stats (void *);
  // Reports statistics for the previous transmission (if one is not
  // in progress).

  int shutdown (void *);
  // Shuts down the task.

  int clear_all_timers (void);
  // Helper method: clears all timers.

private:
  const int usecs_;
  // How many microseconds are in a second.

  Bounded_Packet_Relay *relay_;
  // The bounded packet relay.

  Thread_Timer_Queue *queue_;
  // The timer queue implementation.

  Thread_Bounded_Packet_Relay_Driver &driver_;
  // The thread timer queue test driver.
};

class BPR_Handler_Base : public ACE_Event_Handler
{
  // = TITLE
  //     Base event handler class for bounded packet relay example.
  //
  // = DESCRIPTION
  //     The base class provides a helper method that derived classes
  //     can register as a deferred execution callback that will cancel
  //     all timers in the underlying timer queue, and then delete "this".
  //
public:

  // = Trait for command accessible entry points.

  typedef int (BPR_Handler_Base::*ACTION) (void *);


  BPR_Handler_Base (Bounded_Packet_Relay &relay,
                    Thread_Timer_Queue &queue);
  // Constructor.

  virtual ~BPR_Handler_Base (void);
  // Destructor.

  // = Command accessible entry points.

  virtual int clear_all_timers (void *);
  // Helper method: clears all timers.

protected:
  Bounded_Packet_Relay &relay_;
  // Stores a reference to the relay object on which to invoke
  // the appropritate calls when the timer expires.

  Thread_Timer_Queue &queue_;
  // Store a reference to the timer queue, in which to re-register
  // the send timer and handler if there are still sends to perform.
};

class Send_Handler;

class Send_Handler : public BPR_Handler_Base
{
  // = TITLE
  //     Event handler for message send timeout events.
  //
  // = DESCRIPTION
  //     The <handle_timeout> hook method calls the relay's send
  //     method and decrements its count of messages to send.
  //     If there are still messages to send, it re-registers itself
  //     with the timer queue.  Otherwise it calls the relay's end
  //     transmission method, and registers a deferred execution
  //     callback to clear the timer queue, and then delete "this".
public:

  // = Trait for command accessible entry points.

  typedef int (Send_Handler::*ACTION) (void *);

  Send_Handler (u_long send_count,
                const ACE_Time_Value &duration,
                Bounded_Packet_Relay &relay,
                Thread_Timer_Queue &queue,
                Thread_Bounded_Packet_Relay_Driver &driver);
  // Constructor.

  virtual ~Send_Handler (void);
  // Destructor.

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);
  // Call back hook.

  virtual int cancelled (void);
  // Cancellation hook.

  // = Command accessible entry points.

  virtual int reregister (void *timeout);
  // Helper method: re-registers this handler.

private:

  u_long send_count_;
  // Count of the number of messages to send from the
  // relay object to the output device object.

  ACE_Time_Value duration_;
  // Stores the expected duration until expiration, and is used to
  // re-register the handler if there are still sends to perform.

  Thread_Bounded_Packet_Relay_Driver &driver_;
  // Reference to the driver that will redisplay the user input menu.
};

class Termination_Handler : public BPR_Handler_Base
{
  // = TITLE
  //     Event handler for end transmission timeout events.
  //
  // = DESCRIPTION
  //     The <handle_timeout> hook method calls the relay's end
  //     transmission method, then registers a deferred execution
  //     callback to clear all timers and then delete "this".
public:
  Termination_Handler (Bounded_Packet_Relay &relay,
                       Thread_Timer_Queue &queue,
                       Thread_Bounded_Packet_Relay_Driver &driver);
  // Constructor.

  virtual ~Termination_Handler (void);
  // Destructor.

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);
  // Call back hook.

  virtual int cancelled (void);
  // Cancellation hook.

private:
  Thread_Bounded_Packet_Relay_Driver &driver_;
  // Reference to the driver that will redisplay the user input menu.
};

class Thread_Bounded_Packet_Relay_Driver : public Bounded_Packet_Relay_Driver <Thread_Timer_Queue>
{
  // = TITLE
  //    Implements an example application that exercises
  //    <Thread_Timer_Queue> timer queue.
  //
  // = DESCRIPTION
  //    This class implements a simple test driver for the
  //    <Thread_Timer_Queue>.  The <display_menu> hook method is
  //    called from the base class to print a menu specific to the
  //    thread implementation of the timer queue.
public:

  // = Trait for commands issued from this driver

  typedef ACE_Command_Callback<User_Input_Task, User_Input_Task::ACTION> COMMAND;

  // = Initialization and termination methods.

  Thread_Bounded_Packet_Relay_Driver (Bounded_Packet_Relay *relay);
  // Constructor.

  ~Thread_Bounded_Packet_Relay_Driver (void);
  // Destructor.

  virtual int display_menu (void);
  // Displays the user menu.

  virtual int init (void);
  // Initializes the driver.

  virtual int run (void);
  // Run the driver.

private:
  User_Input_Task input_task_;
  // User input task, subclassed from ACE_Task.
};

#endif /* _THREAD_BOUNDED_PACKET_RELAY_H_ */
