// This may look like C, but it's really -*- C++ -*-
// $Id$

// ========================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     Arg_Shifter.h
//
// = AUTHOR
//     Seth Widoff
//
// ========================================================================

#ifndef ACE_ARG_SHIFTER_H
#define ACE_ARG_SHIFTER_H
#include "ace/pre.h"

#include "ace/OS.h"


class ACE_Export ACE_Arg_Shifter
{
  // = TITLE
  //    This ADT shifts known args to the back of the argv vector, so
  //    deeper levels of argument parsing can locate the yet
  //    unprocessed arguments at the beginning of the vector.
  //
  // = DESCRIPTION
  //    The <ACE_Arg_Shifter> copies the pointers of the argv vector
  //    into a temporary array. As the <ACE_Arg_Shifter> iterates over
  //    the temp, is places known arguments in the rear of the argv
  //    and unknown ones in the beginning. So, after having visited
  //    all the arguments in the temp vector, <ACE_Arg_Shifter> has
  //    placed all the unknown arguments in their original order at
  //    the front of argv.
public:
  // = Initialization and termination methods.
  ACE_Arg_Shifter (int& argc,
                   char** argv,
                   char** temp = 0);
  // Initialize the <ACE_Arg_Shifter> to the vector over which to
  // iterate, also providing the temporary array if the client doesn't
  // want the arg_shifter to dynamically allocate its own. If internal
  // dynamic allocation fails, the <ACE_Arg_Shifter> will set all the
  // indices to the end of the vector, forbidding iteration. Following
  // iteration over argv, the argc value will contain the number of
  // unconsumed arguments.

  ~ACE_Arg_Shifter (void);
  // Destructor.

  char* get_current (void) const;
  // Get the current head of the vector.

  char* get_the_parameter (const char* flag);
  // If the <flag> matches the current_arg of arg shifter
  // this method will attempt to return the associated
  // parameter value
  //
  // Safe to call without checking that a current arg exists
  //
  // In the following examples, a pointer to the char* "value" is ret
  //
  // eg: main -foobar value, main -FooBar value
  //     main -FOOBARvalue
  //
  //     all of the above will all match the <flag> == -FooBar
  //     and will return a char* to "value"
  //
  //     main -foobar 4 would succeed and return a char* to "4"
  //     main -foobar -4 does not succeed (-4 is not a parameter)
  //          but instead, would return 0
  //
  // 0 is returned:
  //     If the current argument does not match flag
  //     If there is no parameter found after a 'matched' flag
  //
  // If the flag is matched and the flag and paramter DO NOT RUN
  // together, the flag is consumed, the parameter is returned,
  // and the new current argument is the parameter value.
  // ie '-foobarflag  VALUE' leaves the new cur arg == "VALUE"
  //
  // If the flag is matched and the flag and parameter RUN
  // together '-foobarflagVALUE', the flag is NOT consumed
  // and the cur arg is left pointing to the entire flag/value pair

  int cur_arg_strncasecmp (const char* flag);
  // Check if the current argument matches (case insensitive) <flag>
  //
  // ------------------------------------------------------------
  //
  // Case A: Perfect Match (case insensitive)
  // 0 is returned.
  //
  //     ie: when current_arg = "-foobar" or "-FOOBAR" or "-fooBAR"
  //         this->cur_arg_strncasecmp ("-FooBar);
  //         will return 0
  //
  // ------------------------------------------------------------
  //
  // Case B: Perfect Match (case insensitive) but the current_arg
  // is longer than the flag. Returns a number equal to the index
  // in the char* indicating the start of the extra characters
  //
  //     ie: when current_arg = "-foobar98023"
  //         this->cur_arg_strncasecmp ("-FooBar);
  //         will return 7
  //
  // Notice: this number will always be > 0
  //
  // ------------------------------------------------------------
  //
  // Case C: If neither of Case A or B is met (no match)
  // then -1 is returned

  int consume_arg (int number = 1);
  // Consume <number> argument(s) by sticking them/it on the end of
  // the vector.

  int ignore_arg (int number = 1);
  // Place <number> arguments in the same relative order ahead of the
  // known arguemnts in the vector.

  int is_anything_left (void) const;
  // Returns the number of args left to see in the vector.

  int is_option_next (void) const;
  // Returns 1 if there's a next item in the vector and it begins with
  // '-'.

  int is_parameter_next (void) const;
  // Returns 1 if there's a next item in the vector and it doesn't
  // begin with '-'.

  int num_ignored_args (void) const;
  // Returns the number of irrelevant args seen.

private:
  int& argc_;
  // The size of the argument vector.

  int total_size_;
  // The size of argv_.

  char** temp_;
  // The temporary array over which we traverse.

  char** argv_;
  // The array in which the arguments are reordered.

  int current_index_;
  // The element in <temp_> we're currently examining.

  int back_;
  // The index of <argv_> in which we'll stick the next unknown
  // argument.

  int front_;
  // The index of <argv_> in which we'll stick the next known
  // argument.
};

#include "ace/post.h"
#endif /* ACE_ARG_SHIFTER_H */
