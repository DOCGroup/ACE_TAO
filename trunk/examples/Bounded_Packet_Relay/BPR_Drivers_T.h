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

#ifndef _BPR_DRIVERS_T_H_
#define _BPR_DRIVERS_T_H_

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
class Input_Device_Wrapper_Base;
class Output_Device_Wrapper_Base;

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

  ~Bounded_Packet_Relay_Driver (void);
  // Destructor.

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

  int logging_level (void);
  // Get logging level.

  void logging_level (int ll);
  // Set logging level.

protected:
  // = Major Driver Mechanisms

  TQ timer_queue_;
  // Timer queue for transmission timeouts.

  // = Set of commands to be executed.

  ACE_Command_Base *packet_count_cmd_;
  // Set packet count command.

  ACE_Command_Base *arrival_period_cmd_;
  // Set arrival period command.

  ACE_Command_Base *transmit_period_cmd_;
  // Set transmit period command.

  ACE_Command_Base *duration_limit_cmd_;
  // Set duration limit command.

  ACE_Command_Base *logging_level_cmd_;
  // Set logging level command.

  ACE_Command_Base *run_transmission_cmd_;
  // Run transmission command.

  ACE_Command_Base *cancel_transmission_cmd_;
  // Cancel transmission command.

  ACE_Command_Base *report_stats_cmd_;
  // Report statistics command.

  ACE_Command_Base *shutdown_cmd_;
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

  int logging_level_;
  // Logging level.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "BPR_Drivers_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("BPR_Drivers_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* _BPR_DRIVERS_T_H_ */
