// -*- C++ -*-

//==========================================================================
/**
 *  @file    ARGV.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Everett Anderson <eea1@cs.wustl.edu>
 */
//==========================================================================

#ifndef ACE_ARGUMENT_VECTOR_H
#define ACE_ARGUMENT_VECTOR_H
#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Queue.h"

/**
 * @class ACE_TARGV
 *
 * @brief Builds a counted argument vector (ala argc/argv) from either
 * a string or a set of separate tokens. Can substitute environment
 * variable values for tokens that are environment variable references.
 */
template < typename CHAR_TYPE = char >
class ACE_TARGV
{
public:
  // = Initialization and termination.
  /**
   * Splits the specified string into an argument vector, split at whitespace.
   *
   * @param buf   An CHAR_TYPE array to split into tokens for the vector.
   *
   * @param substitute_env_args  If non-zero, any token that is an
   *              an environment variable reference ($VAR) will have
   *              its environment variable value in the resultant vector
   *              in place of the environment variable name. This only works
   *              if the token is an environment variable reference and
   *              nothing else; it doesn't substitute environment variable
   *              references within a token. For example, @c $HOME/file will
   *              not substitute the value of the HOME environment variable.
   */
  ACE_TARGV (const CHAR_TYPE buf[],
            int substitute_env_args = 1);

  /**
   * Converts @a argv into a linear string.  If @a substitute_env_args
   * is enabled then we'll substitute the environment variables for
   * each $ENV encountered in the string.  The <buf> operation is not
   * allowed on an ACE_TARGV created this way.
   */
  ACE_TARGV (CHAR_TYPE *argv[],
            int substitute_env_args = 1);

  /**
   * Creates an ACE_TARGV which is the concatenation of the first_argv
   * and the second argv. The argv arguments should be null pointer
   * terminated.
   */
  ACE_TARGV (CHAR_TYPE *first_argv[],
            CHAR_TYPE *second_argv[],
            int substitute_env_args =1);

  /**
   * Entry point for creating an CHAR_TYPE *[] command line
   * iteratively via the <add> method.  When this constructor is used,
   * the <ITERATIVE> state is enabled.  The <argv> and <buf> methods
   * are allowed, and the result is recreated when called multiple
   * times.  The subscript operator is not allowed.
   */
  ACE_TARGV (int substitute_env_args = 1);

  /// Destructor.
  ~ACE_TARGV (void);

  // = Accessor arguments.
  /// Returns the <index>th string in the ARGV array.
  const CHAR_TYPE *operator[] (size_t index);

  /**
   * Returns the @c argv array.  Caller should not delete this memory
   * since the ACE_TARGV destructor will delete it.  If the caller
   * modifies the array in the iterative mode, the changes are not
   * saved to the queue.
   */
  CHAR_TYPE **argv (void);

  /// Returns @c argc.
  int argc (void) const;

  /// Returns the @c buf.  Caller should not delete this memory since
  /// the ACE_TARGV destructor will delete it.
  const CHAR_TYPE *buf (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Add another argument.  This only works in the <ITERATIVE> state.
  /// Note that this method does not copy <next_arg>, nor does it
  /// assume ownership of managing its memory, i.e., the caller is
  /// responsible for memory management.  Returns -1 on failure and 0
  /// on success.
  int add (const CHAR_TYPE *next_arg);

  /**
   * Add another @a argv array.  The @a argv parameter must be NULL
   * terminated.  This only works in the <ITERATIVE> state.  Returns
   * -1 on failure and 0 on success.
   */
  int add (CHAR_TYPE *argv[]);

  /// What state is this ACE_TARGV in?
  int state (void) const;

  // These are the states possible via the different constructors.
  enum States
  {
    /// ACE_TARGV converts buf[] to CHAR_TYPE *argv[]
    TO_STRING = 1,
    /// ACE_TARGV converts CHAR_TYPE *argv[] to buf[]
    TO_PTR_ARRAY = 2,
    /// Builds buf[] or CHAR_TYPE *argv[] iteratively with <add>.
    ITERATIVE = 3
  };

private:
  /// Copy Constructor not implemented
  ACE_TARGV (const ACE_TARGV&);

  /// Assignment '=' operator not implemented
  ACE_TARGV operator= (const ACE_TARGV&);

  /// Creates buf_ from the queue, deletes previous buf_.
  int create_buf_from_queue (void);

  /// Converts buf_ into the CHAR_TYPE *argv[] format.
  int string_to_argv (void);

  /// Returns the string created from argv in buf and
  /// returns the number of arguments.
  int argv_to_string (CHAR_TYPE **argv, CHAR_TYPE *&buf);

  /// Replace args with environment variable values?
  int substitute_env_args_;

  /// Current state marker.
  int state_;

  /// Number of arguments in the ARGV array.
  int argc_;

  /// The array of string arguments.
  CHAR_TYPE **argv_;

  /// Buffer containing the <argv> contents.
  CHAR_TYPE *buf_;

  /// Total length of the arguments in the queue, not counting
  /// separating spaces
  size_t length_;

  /// Queue which keeps user supplied arguments.  This is only
  /// active in the "iterative" mode.
  ACE_Unbounded_Queue<CHAR_TYPE *> queue_;
};

typedef ACE_TARGV<ACE_TCHAR> ACE_ARGV;

#include "ace/ARGV.inl"

#include /**/ "ace/post.h"
#endif /* ACE_ARGUMENT_VECTOR_H */
