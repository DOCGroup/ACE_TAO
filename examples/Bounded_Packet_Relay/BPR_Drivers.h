/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    BPR_Driver.h
//
// = DESCRIPTION
//    This code builds an abstraction to factor out common code from
//    the different possible implementations of the Timer_Queue based
//    bounded packet relay example.
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

#if !defined (_BPR_DRIVER_H_)
#define _BPR_DRIVER_H_

#include "ace/Task.h"

// forward declarations
class Input_Device_Wrapper_Base;
class Output_Device_Wrapper_Base;

class Command_Base
{
  // = TITLE
  //    Defines an abstract class that allows us to invoke commands
  //    without knowing anything about the implementation.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a command
  //    independent of the effect of the command, or the objects used
  //    to implement it.
public:
  virtual int execute (void *arg) = 0;
  // Invokes the method <action_> from the object <receiver_>.
};

class Bounded_Packet_Relay_Base
{
  // = TITLE
  //     Base class for the time-bounded packet relay class.
  //
  // = DESCRIPTION
  //     This enum must go here to avoid confusing certain broken C++
  //     compilers.
public:
  // = Enumerates possible status values for a transmission.
  enum Transmission_Status 
  {
    UN_INITIALIZED,
    STARTED,
    COMPLETED,
    TIMED_OUT,
    CANCELLED,
    ERROR
  };
};

template <ACE_SYNCH_DECL>
class Bounded_Packet_Relay : public Bounded_Packet_Relay_Base
{
  // = TITLE
  //     This class defines a packet relay abstraction for a
  //     transmission bounded external commands to start and end the
  //     transmission.  The transmission may be bounded by the number
  //     of packets to send, the dration of the transmission, or any
  //     other factors.
  //
  // = DESCRIPTION
  //     The relay abstraction implemented by this class registers a
  //     callback command with an input device wrapper, and relays
  //     input to an output device at a pace specified in the start
  //     transmission call.
public:

  typedef int (Bounded_Packet_Relay::*ACTION) (void *);
  // Command entry point type definition.

  // = Initialization method

  Bounded_Packet_Relay (ACE_Thread_Manager *input_task_mgr,
                        Input_Device_Wrapper_Base *input_wrapper,
                        Output_Device_Wrapper_Base *output_wrapper);
  // Constructor.

  virtual ~Bounded_Packet_Relay (void);
  // Destructor.

  int send_input (void);
  // Requests output be sent to output device.

  int start_transmission (u_long packet_count,
                          u_long arrival_period,
                          u_long logging_level);
  // Requests a transmission be started.

  int end_transmission (Transmission_Status status);
  // Requests a transmission be ended.

  int report_statistics (void);
  // Requests a report of statistics from the last transmission.

  // = Command Accessible Entry Points.

  int receive_input (void *);
  // Public entry point to which to push input.

private:
  // = Concurrency Management.

  ACE_Thread_Manager * input_task_mgr_;
  // Thread manager for the input device task.

  Input_Device_Wrapper_Base * input_wrapper_;
  // Pointer to the input device wrapper.

  Output_Device_Wrapper_Base * output_wrapper_;
  // Pointer to the output device wrapper.

  ACE_Message_Queue<ACE_SYNCH_USE> queue_;
  // Queue used to buffer input messages.

  ACE_SYNCH_MUTEX_T transmission_lock_;
  // Lock for thread-safe synchronization 
  // of transmission startup and termination.

  // = Transmission Statistics

  u_long transmission_number_;
  // Number of transmissions sent.

  u_long packets_sent_;
  // Count of packets sent in the most recent transmission.

  Transmission_Status status_;
  // Status of the current or most recent transmission.

  ACE_Time_Value transmission_start_;
  // Start time of the most recent transmission.

  ACE_Time_Value transmission_end_;
  // Ending time of the most recent transmission.

};

class Input_Device_Wrapper_Base : public ACE_Task_Base
{
  // = TITLE
  //    This class defines an abstract base class for an input device
  //    wrapper that hides the details of the specific device and
  //    provides a consistent message passing interface without
  //    knowing anything about the implementation of the input device
  //    or the message receiver.
  //
  //    The abstract base class ctor takes a command template object
  //    that is instantiated with the correct receiver and action
  //    types. This command object is used to send newly created input
  //    messages to the receiver.
  //
  //    The abstract base class is designed to operate in an active
  //    "push" mode, sending input data to the receiver whenever the
  //    data is ready.  The underlying device may be active, notifying
  //    the wrapper when data is ready, or may be passive in which
  //    case the wrapper must rely on a reactive and/or polling
  //    mechanism.
  //
  // = DESCRIPTION
  //    Derived classes are responsible for filling in concrete
  //    definitions for the abstract message creation method and the
  //    svc method.
public:
  // = Initialization and termination methods.
  Input_Device_Wrapper_Base (ACE_Thread_Manager *input_task_mgr);
  // Constructor.

  virtual ~Input_Device_Wrapper_Base ();
  // Destructor.
 
  int set_send_input_msg_cmd (Command_Base *send_input_msg_cmd);
  // Sets send input message command in the input device driver
  // object.

  int set_input_period (u_long input_period);
  // Sets period (in usecs) between when inputs are created.

  int set_send_count (long count);
  // Sets count of messages to send.

  int request_stop (void);
  // Requests that the input device stop sending messages and
  // terminate its thread.  Should return 1 if it will do so, 0 if it
  // has already done so, or -1 if there is a problem doing so.

  virtual int svc (void);
  // This method runs the input device loop in the new thread.

protected:
  virtual ACE_Message_Block *create_input_message (void) = 0;
  // Creates a new message block, carrying data read from the
  // underlying input device.

  virtual int send_input_message (ACE_Message_Block *);
  // Sends a newly created message block, carrying data read from the
  // underlying input device, by passing a pointer to the message
  // block to its command execution.

  Command_Base *send_input_msg_cmd_;
  // Send newly created input message.

  u_long input_period_;
  // Period between when input values are produced (usecs).

  ACE_Reactor reactor_;
  // Reactor used to multiplex input streams, timeouts.

  int is_active_;
  // Flag to indicate whether or not input object is 
  // (and should remain) active.

  long send_count_;
  // Count of messages to send before stopping (-1 indicates the
  // device should not stop).
};

class Output_Device_Wrapper_Base
{
  // = TITLE
  //    This class defines an abstract base class for an output device
  //    wrapper that hides the details of the specific device and
  //    provides a consistent write method interface without knowing
  //    anything about the implementation.
  //
  // = DESCRIPTION
  //    The abstract methods write_output_message () and
  //    modify_device_settings () are defined in derived classes to
  //    write the contents of the passed message out the underlying
  //    output device, and update device settings, respectively.
public:
  virtual int write_output_message (void *) = 0;
  // Writes contents of the passed message block out to the underlying
  // output device.

  virtual int modify_device_settings (void *) = 0;
  // Provides an abstract interface to allow modifying device
  // settings.
};

template <class TQ>
class Bounded_Packet_Relay_Driver
{
  // = TITLE
  //    This abstract base class provides a simple abstraction for a
  //    test driver for the bounded packet relay example.
  //
  // = DESCRIPTION
  //    This is the place where the common code to test the different
  //    implementations of the timer queue resides.  This class has
  //    the logic for the parse_commands () method, the run (),
  //    read_input () and get_next_request () methods.  Subclasses can
  //    override these methods if there is some logic that is specific
  //    to that implementation.
public:
  virtual int parse_commands (const char *buf);
  // Breaks up the input string buffer into pieces and executes the
  // appropriate method to handle that operation.

  virtual int run (void);
  // This is the main entry point for the driver.  The user of the
  // class should normally invoke this method.  Returns 0 when
  // successful, or 0 otherwise.

  virtual int get_next_request (void);
  // This internal method gets the next request from the user.
  // Returns -1 when user wants to exit.  Returns 0 otherwise.

  virtual ssize_t read_input (char *buf, size_t bufsiz);
  // Reads input from the user into the buffer <buf> with a maximum of
  // <bufsiz> bytes.  Returns the amount of bytes actually read
  // Otherwise, a -1 is returned and errno is set to indicate the
  // error.

  virtual int display_menu (void)=0;
  // Prints the user interface for the driver to STDERR.

  virtual int init (void)=0;
  // Initializes values and operations for the driver.

protected:
  // = Major Driver Mechanisms

  TQ timer_queue_;
  // Timer queue for transmission timeouts.

  // = Set of <Command>s to be executed.

  Command_Base *packet_count_cmd_;
  // Set packet count command.

  Command_Base *arrival_period_cmd_;
  // Set arrival period command.

  Command_Base *transmit_period_cmd_;
  // Set transmit period command.

  Command_Base *duration_limit_cmd_;
  // Set duration limit command.

  Command_Base *logging_level_cmd_;
  // Set logging level command.

  Command_Base *run_transmission_cmd_;
  // Run transmission command.

  Command_Base *cancel_transmission_cmd_;
  // Cancel transmission command.

  Command_Base *report_stats_cmd_;
  // Report statistics command.

  Command_Base *shutdown_cmd_;
  // Shut down the driver.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Driver.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Driver.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* _BPR_DRIVER_H_ */
