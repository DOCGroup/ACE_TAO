/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ARGV.h
//
// = AUTHOR
//    Doug Schmidt, Everett Anderson
//
// ============================================================================

#ifndef ACE_ARGUMENT_VECTOR_H
#define ACE_ARGUMENT_VECTOR_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"

class ACE_Export ACE_ARGV
{
  // = TITLE
  //     Transforms a string <buf> into an <argv> style vector of
  //     strings or an <argv> style vector of string <buf>, performing
  //     environment variable substitutions if necessary.
public:
  // = Initialization and termination.
  ACE_ARGV (const ASYS_TCHAR buf[], int substitute_env_args = 1);
  // Converts <buf> into an <argv>-style vector of strings.  If
  // <substitute_env_args> is enabled then we'll substitute the
  // environment variables for each $ENV encountered in the string.
  // The subscript and argv() operations are not allowed on an
  // ACE_ARGV created this way.

  ACE_ARGV (ASYS_TCHAR *argv[],
            int substitute_env_args = 1);
  // Converts <argv> into a linear string.  If <substitute_env_args>
  // is enabled then we'll substitute the environment variables for
  // each $ENV encountered in the string.
  // The buf() operation is not allowed on an ACE_ARGV created
  // this way.

  ACE_ARGV (ASYS_TCHAR *first_argv[], ASYS_TCHAR *second_argv[], int substitute_env_args =1);
  // Creates an ACE_ARGV which is the concatenation of the first_argv
  // and the second argv. The argv arguments should be null pointer terminated.

  ACE_ARGV (int substitute_env_args = 1);
  // Entry point for creating an ASYS_TCHAR *[] command line iteratively
  // via the add() method.  The argv() and buf() method calls are
  // allowed, and the result is recreated when called multiple times.
  // The subscript operator is not allowed.

  ~ACE_ARGV (void);
  // Destructor.

  // = Accessor arguments.
  const ASYS_TCHAR *operator[] (size_t index);
  // Returns the <index>th string in the ARGV array.

  ASYS_TCHAR **argv (void);
  // Returns the <argv> array.  Caller should not delete this memory
  // since the <ARGV> destructor will delete it.  If the caller
  // modifies the array in the iterative mode, the changes are not
  // saved to the queue.

  size_t argc (void) const;
  // Returns <argc>.

  const ASYS_TCHAR *buf (void);
  // Returns the <buf>.  Caller should not delete this memory since
  // the <ARGV> destructor will delete it.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  int add (const ASYS_TCHAR *next_arg);
  // Add another argument.  This only works in the ITERATIVE state.

  int state (void) const;
  // What state is this ACE_ARGV in?

  // These are the states possible via the different constructors.
  enum States
  {
    TO_STRING = 1,
    // ACE_ARGV converts buf[] to ASYS_TCHAR *argv[]
    TO_PTR_ARRAY = 2,
    // ACE_ARGV converts ASYS_TCHAR *argv[] to buf[]
    ITERATIVE = 3
    // Builds buf[] or ASYS_TCHAR *argv[] iteratively with add()
  };

private:

  int create_buf_from_queue (void);
  // Creates buf_ from the queue, deletes previous buf_.

  int string_to_argv (void);
  // Converts buf_ into the ASYS_TCHAR *argv[] format.

  int argv_to_string (ASYS_TCHAR **argv, ASYS_TCHAR *&buf);
  // Returns the string created from argv in buf and
  // returns the number of arguments.

  int substitute_env_args_;
  // Replace args with environment variable values?

  int state_;
  // Current state marker.

  size_t argc_;
  // Number of arguments in the ARGV array.

  ASYS_TCHAR **argv_;
  // The array of string arguments.

  ASYS_TCHAR *buf_;
  // Buffer containing the <argv> contents.

  size_t length_;
  // Total length of the arguments in the queue, not counting
  // separating spaces

  ACE_Unbounded_Queue<ASYS_TCHAR *> queue_;
  // Queue which keeps user supplied arguments.  This is only
  // active in the "iterative" mode.
};

#if defined (__ACE_INLINE__)
#include "ace/ARGV.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ARGUMENT_VECTOR_H */
