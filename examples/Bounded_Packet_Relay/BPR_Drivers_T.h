/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    BPR_Drivers_T.h
 *
 *  $Id$
 *
 *  This code factors out common class templates for use in
 *  the different possible implementations of the Timer_Queue
 *  based bounded packet relay example.
 *
 *
 *  @author Chris Gill           <cdgill@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> Based on the Timer Queue Test example written by Carlos O'Ryan        <coryan@cs.wustl.edu>  and Douglas C. Schmidt   <schmidt@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BPR_DRIVERS_T_H_
#define _BPR_DRIVERS_T_H_

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
class Input_Device_Wrapper_Base;
class Output_Device_Wrapper_Base;

/**
 * @class Bounded_Packet_Relay_Driver
 *
 * @brief This abstract base class provides a simple abstraction for a
 * test driver for the bounded packet relay example.
 *
 * This is the place where the common code to test the different
 * implementations of the timer queue resides.  This class has
 * the logic for the parse_commands () method, the run (),
 * read_input () and get_next_request () methods.  Subclasses can
 * override these methods if there is some logic that is specific
 * to that implementation.
 */
template <class TQ>
class Bounded_Packet_Relay_Driver
{
public:
  /// Constructor.
  Bounded_Packet_Relay_Driver (void);

  /// Destructor.
  virtual ~Bounded_Packet_Relay_Driver (void);

  /// Breaks up the input string buffer into pieces and executes the
  /// appropriate method to handle that operation.
  virtual int parse_commands (const char *buf);

  /**
   * This is the main entry point for the driver.  The user of the
   * class should normally invoke this method.  Returns 0 when
   * successful, or 0 otherwise.
   */
  virtual int run (void);

  /// This internal method gets the next request from the user.
  /// Returns -1 when user wants to exit.  Returns 0 otherwise.
  virtual int get_next_request (void);

  /**
   * Reads input from the user into the buffer <buf> with a maximum of
   * <bufsiz> bytes.  Returns the amount of bytes actually read
   * Otherwise, a -1 is returned and errno is set to indicate the
   * error.
   */
  virtual ssize_t read_input (char *buf, size_t bufsiz);

  /// Prints the user interface for the driver to STDERR.
  virtual int display_menu (void)=0;

  /// Initializes values and operations for the driver.
  virtual int init (void)=0;

  /// Get count of packets to send in a transmission.
  u_long packet_count (void);

  /// Set count of packets to send in a transmission.
  void packet_count (u_long pc);

  /// Get rate at which input packets are to arrive.
  u_long arrival_period (void);

  /// Set rate at which input packets are to arrive.
  void arrival_period (u_long ap);

  /// Get rate at which packets are to be relayed (usec).
  u_long send_period (void);

  /// Set rate at which packets are to be relayed (usec).
  void send_period (u_long sp);

  /// Get limit on the duration of the transmission (usec).
  u_long duration_limit (void);

  /// Set limit on the duration of the transmission (usec).
  void duration_limit (u_long dl);

  /// Get logging level.
  int logging_level (void);

  /// Set logging level.
  void logging_level (int ll);

protected:
  // = Major Driver Mechanisms

  /// Timer queue for transmission timeouts.
  TQ timer_queue_;

  // = Set of commands to be executed.

  /// Set packet count command.
  ACE_Command_Base *packet_count_cmd_;

  /// Set arrival period command.
  ACE_Command_Base *arrival_period_cmd_;

  /// Set transmit period command.
  ACE_Command_Base *transmit_period_cmd_;

  /// Set duration limit command.
  ACE_Command_Base *duration_limit_cmd_;

  /// Set logging level command.
  ACE_Command_Base *logging_level_cmd_;

  /// Run transmission command.
  ACE_Command_Base *run_transmission_cmd_;

  /// Cancel transmission command.
  ACE_Command_Base *cancel_transmission_cmd_;

  /// Report statistics command.
  ACE_Command_Base *report_stats_cmd_;

  /// Shut down the driver.
  ACE_Command_Base *shutdown_cmd_;

private:
  /// Count of packets to send in a transmission.
  u_long packet_count_;

  /// Rate at which input packets are to arrive.
  u_long arrival_period_;

  /// Rate at which packets are to be relayed (usec).
  u_long send_period_;

  /// Limit on the duration of the transmission (usec).
  u_long duration_limit_;

  /// Logging level.
  int logging_level_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "BPR_Drivers_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("BPR_Drivers_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* _BPR_DRIVERS_T_H_ */
