// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Arg_Shifter.h
 *
 *  $Id$
 *
 *  @author Seth Widoff
 */
//=============================================================================

#ifndef ACE_ARG_SHIFTER_H
#define ACE_ARG_SHIFTER_H

#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Arg_Shifter
 *
 * @brief This ADT operates on a specified set of arguments (@a argv).
 * As known arguments are scanned, they are shifted to the back of the
 * @a argv vector, so deeper levels of argument parsing can locate the yet
 * unprocessed arguments at the beginning of the vector.
 *
 * The @c ACE_Arg_Shifter copies the pointers of the @a argv vector
 * into a temporary array. As the @c ACE_Arg_Shifter iterates over
 * the copied vector, it places known arguments in the rear of the
 * vector, leaving the unknown ones in the beginning. So, after having
 * visited all the arguments in the temporary vector, @c ACE_Arg_Shifter
 * has placed all the unknown arguments in their original order at
 * the front of original @a argv.
 */
class ACE_Export ACE_Arg_Shifter
{
public:
  // = Initialization and termination methods.
  /**
   * Initialize the ACE_Arg_Shifter to the vector over which to
   * iterate. Optionally, also provide the temporary array for
   * use in shifting the arguments. If ACE_Arg_Shifter must allocate
   * the temporary vector internally and dynamic allocation fails, the
   * ACE_Arg_Shifter will set all indicators to end of the vector,
   * forbidding iteration. Following iteration over @a argv, the
   * @a argc value will be updated to contain the number of
   * unconsumed arguments.
   * @param argc The number of strings in @a argv. @a argc will be
   *    updated to reflect the number of unconsumed arguments.
   * @param argv The argument vector to shift. The string pointers in
   *    the vector will be reordered to place the @a argc unconsumed
   *    arguments at the front of the vector.
   * @param temp A vector of @c ACE_TCHAR pointers at least @a argc
   *    elements long. The vector will be used for argument shifting as
   *    the specified @a argv vector is consumed. The vector must not
   *    be modified while this object exists. If this argument is 0
   *    (the default) the object will allocate and free the temporary
   *    vector transparently.
   */
  ACE_Arg_Shifter (int& argc,
                   const ACE_TCHAR **argv,
                   const ACE_TCHAR **temp = 0);

  /// Same behavior as the preceding constructor, but without the
  /// "const" qualifier.
  ACE_Arg_Shifter (int& argc,
                   ACE_TCHAR **argv,
                   ACE_TCHAR **temp = 0);

  /// Destructor.
  ~ACE_Arg_Shifter (void);

  /// Get the current head of the vector.
  const ACE_TCHAR *get_current (void) const;

  /**
   * If the <flag> matches the current_arg of arg shifter
   * this method will attempt to return the associated
   * parameter value
   *
   * Safe to call without checking that a current arg exists
   *
   * In the following examples, a pointer to the char* "value" is ret
   *
   * eg: main -foobar value, main -FooBar value
   *     main -FOOBARvalue
   *
   *     all of the above will all match the <flag> == -FooBar
   *     and will return a char* to "value"
   *
   *     main -foobar 4 would succeed and return a char* to "4"
   *     main -foobar -4 does not succeed (-4 is not a parameter)
   *          but instead, would return 0
   *
   * 0 is returned:
   *     If the current argument does not match flag
   *     If there is no parameter found after a 'matched' flag
   *
   * If the flag is matched and the flag and paramter DO NOT RUN
   * together, the flag is consumed, the parameter is returned,
   * and the new current argument is the parameter value.
   * ie '-foobarflag  VALUE' leaves the new cur arg == "VALUE"
   *
   * If the flag is matched and the flag and parameter RUN
   * together '-foobarflagVALUE', the flag is NOT consumed
   * and the cur arg is left pointing to the entire flag/value pair
   */
  const ACE_TCHAR *get_the_parameter (const ACE_TCHAR* flag);

  /**
   * Check if the current argument matches (case insensitive) <flag>
   *
   * ------------------------------------------------------------
   *
   * Case A: Perfect Match (case insensitive)
   * 0 is returned.
   *
   *     ie: when current_arg = "-foobar" or "-FOOBAR" or "-fooBAR"
   *         this->cur_arg_strncasecmp ("-FooBar);
   *         will return 0
   *
   * ------------------------------------------------------------
   *
   * Case B: Perfect Match (case insensitive) but the current_arg
   * is longer than the flag. Returns a number equal to the index
   * in the char* indicating the start of the extra characters
   *
   *     ie: when current_arg = "-foobar98023"
   *         this->cur_arg_strncasecmp ("-FooBar);
   *         will return 7
   *
   * Notice: this number will always be > 0
   *
   * ------------------------------------------------------------
   *
   * Case C: If neither of Case A or B is met (no match)
   * then -1 is returned
   */
  int cur_arg_strncasecmp (const ACE_TCHAR *flag);

  /// Consume @a number argument(s) by sticking them/it on the end of
  /// the vector.
  int consume_arg (int number = 1);

  /// Place @a number arguments in the same relative order ahead of the
  /// known arguments in the vector.
  int ignore_arg (int number = 1);

  /// Returns the number of args left to see in the vector.
  int is_anything_left (void) const;

  /// Returns 1 if there's a next item in the vector and it begins with
  /// '-'.
  int is_option_next (void) const;

  /// Returns 1 if there's a next item in the vector and it doesn't
  /// begin with '-'.
  int is_parameter_next (void) const;

  /// Returns the number of irrelevant args seen.
  int num_ignored_args (void) const;

private:
  /// Copy Constructor should not be used.
  ACE_Arg_Shifter (const ACE_Arg_Shifter&);

  /// Assignment '=' operator should not be used.
  ACE_Arg_Shifter operator= (const ACE_Arg_Shifter&);

  /// Refactor the constructor logic.
  void init (void);

  /// The size of the argument vector.
  int& argc_;

  /// The size of argv_.
  int total_size_;

  /// The temporary array over which we traverse.
  const ACE_TCHAR **temp_;

  /// The array in which the arguments are reordered.
  const ACE_TCHAR **argv_;

  /// The element in <temp_> we're currently examining.
  int current_index_;

  /// The index of <argv_> in which we'll stick the next unknown
  /// argument.
  int back_;

  /// The index of <argv_> in which we'll stick the next known
  /// argument.
  int front_;
};

#include /**/ "ace/post.h"

#endif /* ACE_ARG_SHIFTER_H */
