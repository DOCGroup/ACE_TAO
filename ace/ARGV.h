/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ARGV.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Everett Anderson
 */
//=============================================================================

#ifndef ACE_ARGUMENT_VECTOR_H
#define ACE_ARGUMENT_VECTOR_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Queue.h"

/**
 * @class ACE_ARGV
 *
 * @brief Transforms a string <buf> into an <argv> style vector of
 * strings or an <argv> style vector of string <buf>, performing
 * environment variable substitutions if necessary.
 */
class ACE_Export ACE_ARGV
{
public:
  // = Initialization and termination.
  /**
   * Converts <buf> into an <argv>-style vector of strings.  If
   * <substitute_env_args> is enabled then we'll substitute the
   * environment variables for each $ENV encountered in the string.
   * The subscript and <argv> operations are not allowed on an
   * ACE_ARGV created this way.
   */
  ACE_ARGV (const ACE_TCHAR buf[],
            int substitute_env_args = 1);

  /**
   * Converts <argv> into a linear string.  If <substitute_env_args>
   * is enabled then we'll substitute the environment variables for
   * each $ENV encountered in the string.  The <buf> operation is not
   * allowed on an ACE_ARGV created this way.
   */
  ACE_ARGV (ACE_TCHAR *argv[],
            int substitute_env_args = 1);

  /**
   * Creates an ACE_ARGV which is the concatenation of the first_argv
   * and the second argv. The argv arguments should be null pointer
   * terminated.
   */
  ACE_ARGV (ACE_TCHAR *first_argv[],
            ACE_TCHAR *second_argv[],
            int substitute_env_args =1);

  /**
   * Entry point for creating an ACE_TCHAR *[] command line
   * iteratively via the <add> method.  When this constructor is used,
   * the <ITERATIVE> state is enabled.  The <argv> and <buf> methods
   * are allowed, and the result is recreated when called multiple
   * times.  The subscript operator is not allowed.
   */
  ACE_ARGV (int substitute_env_args = 1);

  /// Destructor.
  ~ACE_ARGV (void);

  // = Accessor arguments.
  /// Returns the <index>th string in the ARGV array.
  const ACE_TCHAR *operator[] (size_t index);

  /**
   * Returns the <argv> array.  Caller should not delete this memory
   * since the <ARGV> destructor will delete it.  If the caller
   * modifies the array in the iterative mode, the changes are not
   * saved to the queue.
   */
  ACE_TCHAR **argv (void);

  /// Returns <argc>.
  size_t argc (void) const;

  /// Returns the <buf>.  Caller should not delete this memory since
  /// the <ARGV> destructor will delete it.
  const ACE_TCHAR *buf (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Add another argument.  This only works in the <ITERATIVE> state.
  /// Note that this method does not copy <next_arg>, but instead
  /// assumes ownership of it.  Returns -1 on failure and 0 on success.
  int add (const ACE_TCHAR *next_arg);

  /**
   * Add another <argv> array.  The <argv> parameter must be NULL
   * terminated.  This only works in the <ITERATIVE> state.  Returns
   * -1 on failure and 0 on success.
   */
  int add (ACE_TCHAR *argv[]);

  /// What state is this ACE_ARGV in?
  int state (void) const;

  // These are the states possible via the different constructors.
  enum States
  {
    /// ACE_ARGV converts buf[] to ACE_TCHAR *argv[]
    TO_STRING = 1,
    /// ACE_ARGV converts ACE_TCHAR *argv[] to buf[]
    TO_PTR_ARRAY = 2,
    /// Builds buf[] or ACE_TCHAR *argv[] iteratively with <add>.
    ITERATIVE = 3
  };

private:

  /// Creates buf_ from the queue, deletes previous buf_.
  int create_buf_from_queue (void);

  /// Converts buf_ into the ACE_TCHAR *argv[] format.
  int string_to_argv (void);

  /// Returns the string created from argv in buf and
  /// returns the number of arguments.
  int argv_to_string (ACE_TCHAR **argv, ACE_TCHAR *&buf);

  /// Replace args with environment variable values?
  int substitute_env_args_;

  /// Current state marker.
  int state_;

  /// Number of arguments in the ARGV array.
  size_t argc_;

  /// The array of string arguments.
  ACE_TCHAR **argv_;

  /// Buffer containing the <argv> contents.
  ACE_TCHAR *buf_;

  /// Total length of the arguments in the queue, not counting
  /// separating spaces
  size_t length_;

  /// Queue which keeps user supplied arguments.  This is only
  /// active in the "iterative" mode.
  ACE_Unbounded_Queue<ACE_TCHAR *> queue_;
};

#if defined (__ACE_INLINE__)
#include "ace/ARGV.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_ARGUMENT_VECTOR_H */
