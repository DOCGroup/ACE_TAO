/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    BPR_Drivers_T.h
//
// = DESCRIPTION
//    This code factors out common class templates for use in
//    the different possible implementations of the Timer_Queue
//    based bounded packet relay example.
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

#if !defined (_BPR_DRIVERS_T_H_)
#define _BPR_DRIVERS_T_H_

// forward declarations
class Input_Device_Wrapper_Base;
class Output_Device_Wrapper_Base;

template <class RECEIVER, class ACTION>
class Command : public Command_Base
{
  // = TITLE
  //    Defines an abstract class that allows us to invoke commands
  //    without knowing anything about the implementation.  This class
  //    is used in the <Bounded_Packet_Relay_Driver> to invoke
  //    operations of the driver.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute operations,
  //    binding a RECEIVER object with an ACTION.  The RECEIVER knows
  //    how to implement the operation.  A class can invoke operations
  //    without knowing anything about it, or how it was implemented.
public:
  Command (RECEIVER &recvr, ACTION action);
  // Sets the <receiver_> of the Command to recvr, and the <action_>
  // of the Command to <action>.

  virtual int execute (void *arg);
  // Invokes the method <action_> from the object <receiver_>.

private:
  RECEIVER &receiver_;
  // Object where the method resides.

  ACTION action_;
  // Method that is going to be invoked.
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
  typedef int (Bounded_Packet_Relay<ACE_SYNCH_USE>::*ACTION) (void *);
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

  const char * status_msg (void);
  // Returns string corresponding to current status.

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
  Bounded_Packet_Relay_Driver (void);
  // Constructor.

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

  u_long packet_count (void);
  // Get count of packets to send in a transmission.

  void packet_count (u_long pc);
  // Set count of packets to send in a transmission.

  u_long arrival_period (void);
  // Get rate at which input packets are to arrive.

  void arrival_period (u_long ap);
  // Set rate at which input packets are to arrive.

  u_long send_period (void);
  // Get rate at which packets are to be relayed (usec).

  void send_period (u_long sp);
  // Set rate at which packets are to be relayed (usec).

  u_long duration_limit (void);
  // Get limit on the duration of the transmission (usec).

  void duration_limit (u_long dl);
  // Set limit on the duration of the transmission (usec).

  u_long logging_level (void);
  // Get logging level.

  void logging_level (u_long ll);
  // Set logging level.

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

private:
  u_long packet_count_;
  // Count of packets to send in a transmission.

  u_long arrival_period_;
  // Rate at which input packets are to arrive.

  u_long send_period_;
  // Rate at which packets are to be relayed (usec).

  u_long duration_limit_;
  // Limit on the duration of the transmission (usec).

  u_long logging_level_;
  // Logging level.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "BPR_Drivers_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("BPR_Drivers_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* _BPR_DRIVERS_T_H_ */

