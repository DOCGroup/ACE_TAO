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
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_ARGUMENT_VECTOR_H)
#define ACE_ARGUMENT_VECTOR_H

#include "ace/ACE.h"

class ACE_Export ACE_ARGV
  // = TITLE
  //     Transforms a string <buf> into an <argv> style vector of
  //     strings or an <argv> style vector of string <buf>, performing
  //     environment variable substitutions if necessary.
{
public:
  // = Initialization and termination.
  ACE_ARGV (char buf[], int substitute_env_args = 1);
  // Converts <buf> into an <argv>-style vector of strings.  If
  // <substitute_env_args> is enabled then we'll substitute the
  // environment variables for each $ENV encountered in the string.

  ACE_ARGV (char *argv[], int substitute_env_args = 1);
  // Converts <argv> into a linear string.  If <substitute_env_args>
  // is enabled then we'll substitute the environment variables for
  // each $ENV encountered in the string.

  ~ACE_ARGV (void);
  // Destructor.

  // = Accessor arguments.
  char *operator[] (int index) const;
  // Returns the <index>th string in the ARGV array.

  char **argv (void) const;
  // Returns the <argv> array.  Caller should not delete this memory
  // since the <ARGV> destructor will delete it.

  size_t argc (void) const;
  // Returns <argc>. 

  char *buf (void) const;
  // Returns the <buf>.  Caller should not delete this memory since
  // the <ARGV> destructor will delete it.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  size_t argc_;
  // Number of arguments in the ARGV array. 

  char **argv_;
  // The array of string arguments. 

  char *buf_;
  // Buffer containing the <argv> contents.
};

#if defined (__ACE_INLINE__)
#include "ace/ARGV.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ARGUMENT_VECTOR_H */
