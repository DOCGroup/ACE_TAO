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

#if !defined (ACE_ARGUMENT_VECTOR_H)
#define ACE_ARGUMENT_VECTOR_H

#include "ace/ACE.h"
#include "ace/Containers.h"

class ACE_Export ACE_ARGV
  // = TITLE
  //     Transforms a string <buf> into an <argv> style vector of
  //     strings or an <argv> style vector of string <buf>, performing
  //     environment variable substitutions if necessary.
{
public:
  // = Initialization and termination.
  ACE_ARGV (const char buf[], int substitute_env_args = 1);
  // Converts <buf> into an <argv>-style vector of strings.  If
  // <substitute_env_args> is enabled then we'll substitute the
  // environment variables for each $ENV encountered in the string.
  // The subscript and argv() operations are not allowed on an
  // ACE_ARGV created this way.

  ACE_ARGV (char *argv[], int substitute_env_args = 1);
  // Converts <argv> into a linear string.  If <substitute_env_args>
  // is enabled then we'll substitute the environment variables for
  // each $ENV encountered in the string.
  // The buf() operation is not allowed on an ACE_ARGV created
  // this way.

  ACE_ARGV (int substitute_env_args = 1);
  // Entry point for creating an char *[] command line iteratively
  // via the add() method.  The argv() and buf() method calls are
  // allowed, and the result is recreated when called multiple times.
  // The subscript operator is not allowed.

  ~ACE_ARGV (void);
  // Destructor.

  // = Accessor arguments.
  const char *operator[] (size_t index);
  // Returns the <index>th string in the ARGV array.  

  char **argv (void);
  // Returns the <argv> array.  Caller should not delete this memory
  // since the <ARGV> destructor will delete it.  If the caller modifies
  // the array in the iterative mode, the changes are not saved to the
  // queue.

  size_t argc (void) const;
  // Returns <argc>. 

  const char *buf (void);
  // Returns the <buf>.  Caller should not delete this memory since
  // the <ARGV> destructor will delete it.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  int add (const char *next_arg);
  // Add another argument.  This only works in the ITERATIVE state.

  int state (void) const;
  // What state is this ACE_ARGV in?

  // These are the states possible via the different constructors.
  enum States 
  {
    TO_STRING = 1,    
    // ACE_ARGV converts buf[] to char *argv[]
    TO_PTR_ARRAY = 2, 
    // ACE_ARGV converts char *argv[] to buf[]
    ITERATIVE = 3    
    // Builds buf[] or char *argv[] iteratively with add()
  };

private:

  int create_buf_from_queue (void);
  // Creates buf_ from the queue, deletes previous buf_.

  void string_to_array (void);
  // Converts buf_ into the char *argv[] format.

  int substitute_env_args_;
  // Replace args with environment variable values?

  int state_;
  // Current state marker.

  size_t argc_;
  // Number of arguments in the ARGV array. 

  char **argv_;
  // The array of string arguments. 

  char *buf_;
  // Buffer containing the <argv> contents.

  size_t length_;
  // Total length of the arguments in the queue, not counting
  // separating spaces

  ACE_Unbounded_Queue<char *> queue_;
  // Queue which keeps user supplied arguments.  This is only
  // active in the "iterative" mode.
};

#if defined (__ACE_INLINE__)
#include "ace/ARGV.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ARGUMENT_VECTOR_H */
