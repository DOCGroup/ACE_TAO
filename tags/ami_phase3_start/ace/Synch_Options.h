/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//   ACE_Synch_Options.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SYNCH_OPTIONS_H
#define ACE_SYNCH_OPTIONS_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Synch_Options
{
  // = TITLE
  //     Contains the values of options used to determine the
  //     synchronous and asynchronous behavior.
  //
  // = DESCRIPTION
  //     Values support the following behavior (TV == "timeout"
  //     and UR == "use ACE_Reactor"):
  //
  // = BEGIN<CODE>
  //          Parameters      | Description
  //                          |
  //          TV   |  UR      |
  //          -----|----------|-------------------------------
  //               |          |
  //          NULL | yes      | infinite timeout (using ACE_Reactor)
  //               |          |
  //          time | yes      | try asynch transaction for
  //               |          | the specified time (using ACE_Reactor)
  //               |          |
  //          0,0  | yes      | poll; try, if EWOULDBLOCK,
  //               |          |       then return immediately
  //               |          |       (using ACE_Reactor)
  //               |          |
  //          NULL | no       | block forever (don't use ACE_Reactor)
  //               |          |
  //          time | no       | do a blocking transaction
  //               |          | for the specified time
  //               |          | (don't use ACE_Reactor)
  //               |          |
  //          0,0  | no       | poll; but do not initiate a
  //               |          | nonblocking transaction
  //               |          | (don't use ACE_Reactor)
  // = END<CODE>
public:
  // = Options flags for controlling synchronization.  Note that these
  // flags can be bit-wise "or'd" together if both options are
  // desired.
  enum
  {
    USE_REACTOR = 01,
    // Use the Reactor.
    USE_TIMEOUT = 02
    // Interprete the Time_Value.
  };

  // = Initialization methods.
  ACE_Synch_Options (u_long options = 0,
                     const ACE_Time_Value &timeout = ACE_Time_Value::zero,
                     const void *arg = 0);
  // Initialize the Synch_Options based on parameters.

  ~ACE_Synch_Options (void);
  // Default dtor.

  void set (u_long options = 0,
            const ACE_Time_Value &timeout = ACE_Time_Value::zero,
            const void *arg = 0);
  // Initialize the Synch_Options based on parameters.

  int operator[] (u_long option) const;
  // Get method for determining which options are enabled.

  void operator= (u_long option);
  // Set method for enabling certain options.

  const void *arg (void) const;
  // Returns the "magic cookie" argument.

  void arg (const void *);
  // Set the "magic cookie" argument.

  const ACE_Time_Value &timeout (void) const;
  // Returns a reference to the <Time_Value>.  This value only makes
  // sense if (*this)[USE_TIMEOUT] is true.

  void timeout (ACE_Time_Value &tv);
  // Set the <Time_Value>.

  const ACE_Time_Value *time_value (void) const;
  // Returns the address of the timeout <Time_Value> if
  // (*this)[USE_TIMEOUT] is true, else 0.  This should be used with
  // care, e.g., the timeout pointer should not be stored in a manner
  // that will lead to dangling pointers...

  // = Static data members (singletons)

  static ACE_Synch_Options defaults;
  // This is the default setting for options, which will block
  // synchronously.

  static ACE_Synch_Options synch;
  // This is the default synchronous setting.

  static ACE_Synch_Options asynch;
  // This is the default asynchronous setting.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  u_long options_;
  // Keeps track of the enabled options.

  ACE_Time_Value timeout_;
  // Amount of time to wait for timeouts.

  const void *arg_;
  // "Magic cookie" always passed in as an argument to the ACE_Reactor's
  // <schedule_timer> method.  Used to communicate values for
  // asynchronous programming.
};

#if defined (__ACE_INLINE__)
#include "ace/Synch_Options.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SYNCH_OPTIONS_H */
