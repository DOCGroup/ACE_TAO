// -*- C++ -*-

//==========================================================================
/**
 *  @file   Synch_Options.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@uci.edu>
 */
//==========================================================================

#ifndef ACE_SYNCH_OPTIONS_H
#define ACE_SYNCH_OPTIONS_H

#include "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"


/**
 * @class ACE_Synch_Options
 *
 * @brief Contains the values of options used to determine the
 * synchronous and asynchronous behavior.
 *
 * Values support the following behavior (TV == "timeout"
 * and UR == "use ACE_Reactor"):
 *
 * <CODE>
 * | Parameters     | Description
 * |
 * |TV   |  UR      |
 * |-----|----------|-------------------------------
 * |                |
 * |NULL | yes      | infinite timeout (using ACE_Reactor)
 * |                |
 * |time | yes      | try asynch transaction for
 * |                | the specified time (using ACE_Reactor)
 * |                |
 * |0,0  | yes      | poll; try, if EWOULDBLOCK,
 * |                | then return immediately
 * |                | (using ACE_Reactor)
 * |                |
 * |NULL | no       | block forever (don't use ACE_Reactor)
 * |                |
 * |time | no       | do a blocking transaction
 * |                | for the specified time
 * |                | (don't use ACE_Reactor)
 * |                |
 * |0,0  | no       | poll; but do not initiate a
 * |                | nonblocking transaction
 * |                | (don't use ACE_Reactor)
 * </CODE>
 */
class ACE_Export ACE_Synch_Options
{
public:
  /// Options flags for controlling synchronization.
  /**
   * Note that these flags can be bit-wise "or'd" together if both
   * options are desired.
   */
  enum
  {
    /// Use the Reactor.
    USE_REACTOR = 01,
    /// Interprete the Time_Value.
    USE_TIMEOUT = 02
  };

  // = Initialization methods.
  /// Initialize the Synch_Options based on parameters.
  ACE_Synch_Options (unsigned long options = 0,
                     const ACE_Time_Value &timeout = ACE_Time_Value::zero,
                     const void *arg = 0);

  /// Default dtor.
  ~ACE_Synch_Options (void);

  /// Initialize the Synch_Options based on parameters.
  void set (unsigned long options = 0,
            const ACE_Time_Value &timeout = ACE_Time_Value::zero,
            const void *arg = 0);

  /// Get method for determining which options are enabled.
  int operator[] (unsigned long option) const;

  /// Set method for enabling certain options.
  void operator= (unsigned long option);

  /// Returns the "magic cookie" argument.
  const void *arg (void) const;

  /// Set the "magic cookie" argument.
  void arg (const void *);

  /// Returns a reference to the <Time_Value>.  This value only makes
  /// sense if (*this)[USE_TIMEOUT] is true.
  const ACE_Time_Value &timeout (void) const;

  /// Set the <Time_Value>.
  void timeout (const ACE_Time_Value &tv);

  /**
   * Returns the address of the timeout <Time_Value> if
   * (*this)[USE_TIMEOUT] is true, else 0.  This should be used with
   * care, e.g., the timeout pointer should not be stored in a manner
   * that will lead to dangling pointers...
   */
  const ACE_Time_Value *time_value (void) const;

  // = Static data members (singletons)

  /// This is the default setting for options, which will block
  /// synchronously.
  static ACE_Synch_Options defaults;

  /// This is the default synchronous setting.
  static ACE_Synch_Options synch;

  /// This is the default asynchronous setting.
  static ACE_Synch_Options asynch;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Keeps track of the enabled options.
  unsigned long options_;

  /// Amount of time to wait for timeouts.
  ACE_Time_Value timeout_;

  /**
   * "Magic cookie" always passed in as an argument to the ACE_Reactor's
   * <schedule_timer> method.  Used to communicate values for
   * asynchronous programming.
   */
  const void *arg_;
};

#if defined (__ACE_INLINE__)
#include "ace/Synch_Options.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* ACE_SYNCH_OPTIONS_H */
