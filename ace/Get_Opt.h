/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Get_Opt.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  
 *  Long option support added by Don Hinton <dhinton@gmx.net>.
 */
//=============================================================================

#ifndef ACE_GET_OPT_H
#define ACE_GET_OPT_H
#include "ace/pre.h"

#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#undef optind
#undef optarg
#undef opterr

/*
 *  These definitions are for backward compatibility with previous versions.
 *  of ACE_Get_Opt.
 */
#define optarg opt_arg ()
#define optind opt_ind ()

/**
 * @class ACE_Get_Opt
 *
 * @brief Iterator for parsing command-line arguments.
 *
 * This is a C++ wrapper for getopt(3c) and getopt_long(3c).
 */

class ACE_Export ACE_Get_Opt
{
public:
  /// Mutually exclusive ordering values.
  enum
  {
    /// The options must come first, return <EOF> as soon as a 
    /// non-option argument is encountered.
    REQUIRE_ORDER = 1, 

    /// Reorder the arguments so that all the options come first (the
    /// order of the options and the following arguments are
    /// maintained).
    PERMUTE_ARGS = 2, 

    /// Continue processing the command line for each argument.  The
    /// return value '1' signifies a non-option argument.
    RETURN_IN_ORDER = 3
  };

  /// Mutually exclusive option argument mode used by long options.
  enum OPTION_ARG_MODE
  {
    /// Doesn't take an argument.
    NO_ARG = 0,

    /// Requires an argument, same as passing ":" after a short option
    /// character in <optstring>.
    ARG_REQUIRED = 1,

    /// Argument is optional, same as passing "::" after a short 
    /// option character in <optstring>.
    ARG_OPTIONAL = 2
  };

  /**
   * Initialize the internal data when the first call is made.  Start
   * processing options with <argv>-element 0 + <skip_argv0>; the
   * sequence of previously skipped non-option <argv>-elements is
   * empty.
   *
   * <optstring> is a string containing the legitimate short option
   * characters.  A single colon ":" in <optstring> means that the 
   * previous character is an option that wants, requires, an argument,
   * whereas a double colon "::" signifies the argument is optional.
   * The argument is taken from the rest of the current <argv>-element,
   * or from the following <argv>-element (only valid for required
   * arguments, optional arguments must always reside in the same
   * <argv>-element), and returned in by <opt_arg ()>. 
   *
   * Multiple short options can be combined as long as only the last 
   * one can takes an argument, e.g., if <optstring> is defined as 
   * "abc:" or "abc::" then the command line "program -abcxxx" short
   * options a, b, and c are found with "xxx" as the argument for c.
   * However, if the command line is specified as "program -acb" only
   * options a and c are found with "b" as the argument for c.  Also,
   * for options with optional arguments, e.g., those followed by "::"
   * the argument must be in the same <argv>-element, so "program -abc
   * xxx" will only find "xxx" as the argument for c if <optstring> is
   * specified as "abc:" not "abc::". 
   *
   * If a missing required option argument is detected, return the
   * colon character ':' if the first character of <optstring> was a
   * colon, or a '?' otherwise. (Note that the standards are unclear in
   * this respect, so we scan the initial *characters* of <optstring>
   * up unto the first short option character for '+', '-', and ':' in
   * order to determine ordering and missing argument behavior.)
   *
   * If an short option character is seen that is not listed in
   * <optstring>, return '?' after printing an error message.  If you
   * set <report_errors> to zero, the error message is suppressed but
   * we still return '?'.
   *
   * <optstring> can be extended by adding long options, 
   * <long_option()>, that have corresponding short options.  If the 
   * short option already appears in <optstring> they argument
   * characteristics must match, otherwise it is added  -- see 
   * <long_option()> for more information.
   *
   * If 'W', followed by a semi-colon ';' appears in <optstring>, then
   * any time a 'W' appears on the command line, the following argument
   * is treated as a long option, e.g., if the command line contains 
   * "program -W foo" "foo" is treated as a long option, i.e., as if
   * "program --foo" had been passed.
   *
   * <ordering> refers to how the <argv>-elements are processed.  
   * <REQUIRE_ORDER> means that we stop processing and return <EOF> as
   * soon as a non-option argument is found, and <opt_ind ()>
   * holds the index of the next <argv>-element so the program can
   * continue processing the rest of the <argv>-elements.  If
   * <PERMUTE_ARGS> (default) is passed, the <argv>-elements are
   * reordered dynamically, permuted, so that all options appear first.
   * When the last option has been process, <EOF> is returned and
   * <opt_ind()> holds the index into the next non-option
   * element.  If <RETURN_IN_ORDER> is passed, then each <argv>-element
   * is processed in the order is it seen.  If the element is not
   * recognized as an option, '1' is returned and <opt_arg()>
   * contains the <argv>-element found.
   *
   * If <long_only> is non-zero, then all options are treated as long
   * options.  If a long option is not recognized, we try to find a
   * matching short option. */
  ACE_Get_Opt (int argc,
               ACE_TCHAR **argv,
               const ACE_TCHAR *optstring,
               int skip_argv0 = 1,
               int report_errors = 0,
               int ordering = PERMUTE_ARGS,
               int long_only = 0);

  /// Default dtor.
  ~ACE_Get_Opt (void);

  /**
   * Scan elements of <argv> (whose length is <argc>) for short option
   * characters given in <optstring> or long options (with no short
   * option equivilents).
   *
   * If an element of <argv> starts with '-', and is not exactly "-"
   * or "--", then it is a short option element.  The characters of this
   * element (aside from the initial '-') are option characters. If
   * it starts with "--" followed by other characters it is treated as
   * a long option.  If <operator()> is called repeatedly, it returns
   * successively each of the option characters from each of the option
   * elements.
   *
   * If <operator()> finds another option character, it returns that
   * character, updating <optind> and <nextchar> so that the next call
   * to <operator()> can resume the scan with the following option
   * character or <argv>-element.
   *
   * If <operator()> returns 0, it found a long option, '?' indicates
   * an unknown option character, and '1' means that <RETURN_IN_ORDER>
   * was specified and we found an non-option argument.
   *
   * If there are no more option characters, <operator()> returns
   * <EOF>.  Then <opt_ind()> is the index in <argv> of the first
   * <argv>-element that is not an option.  (If <PERMUTE_ARGS> was
   * specified, the <argv>-elements have been permuted so that those
   * that are not options now come last.)
   */
  int operator () (void);

  /**
   * For communication from <operator()> to the caller.  When
   * <operator()> finds an option that takes an argument, the argument
   * value is returned here, otherwise it returns 0.
   */
  ACE_TCHAR *opt_arg (void) const;

  /**
   * Index in <argv> of the next element to be scanned.  This is used
   * for communication to and from the caller and for communication
   * between successive calls to <operator()>.  On entry to
   * <operator()>, zero means this is the first call; initialize.
   *
   * When <operator()> returns <EOF>, this is the index of the first of
   * the non-option elements that the caller should itself scan.
   *
   * Otherwise, <opt_ind()> communicates from one call to the next how
   * much of <argv> has been scanned so far.
   */
  int &opt_ind (void);
  
  // Adds a long option with no corresponding short option.
  int long_option (const ACE_TCHAR *name,
                   OPTION_ARG_MODE has_arg = NO_ARG);

  /// Adds a long option with a corresponding short option.  If the
  /// short option has already been supplied in the <optstring>,
  /// has_arg match match or an error is returned, otherwise the new
  /// short option it is added to the <optstring>.
  /// Returns 0 on success and -1 if the long option can not be added.
  int long_option (const ACE_TCHAR *name,
                   int short_option,
                   OPTION_ARG_MODE has_arg = NO_ARG);

  /// Returns the name of the long option found on the last call to 
  /// <operator()> or 0 if none was found.
  const ACE_TCHAR *long_option (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Return the <optstring>.  This is handy to verify that calls to
  /// long_option added short options as expected.
  const ACE_TCHAR *optstring (void) const;

private:
  class ACE_Get_Opt_Long_Option 
  {
  public:  
    /// ctor
    ACE_Get_Opt_Long_Option (const ACE_TString name,
                             int has_arg,
                             int val = 0)
      :  name_ (name),
         has_arg_ (has_arg),
         val_ (val) {};
    
    /// Default dtor.
    ~ACE_Get_Opt_Long_Option (void) {};
    
    int operator < (const ACE_Get_Opt_Long_Option &rhs);
    
    /// Long option name.
    const ACE_TString name_;

    /// Contains value for <OPTION_ARG_MODE>. 
    int has_arg_;

    /// Contains a valid short option character or zero if it doesn't
    /// have a corresponding short option.  It can also contain a 
    /// non-printable value that cannot be passed to <optstring> but
    /// will be returned by <operator()>.  This is handy for 
    /// simplifying long option handling, see tests/Get_Opt_Test.cpp
    /// for an example of this technique.
    int val_;
  };

  /// Updates nextchar_.
  int nextchar_i (void);

  /// Handles long options.
  int long_option_i (void);

  /// Handles short options.
  int short_option_i (void);

  /// If permuting args, this functions manages the nonopt_start_ and 
  /// nonopt_end_ indexes and makes calls to permute to actually
  /// reorder the <argv>-elements.
  void permute_args (void);

  /// Handles reordering <argv>-elements.
  int permute (void);

  /// Holds the <argc> count.
  int argc_;

  /// Holds the <argv> pointer.
  ACE_TCHAR **argv_;

  /// Holds the option string.
  ACE_TString optstring_;

  /// Index in argv_ of the next element to be scanned.
  int opt_ind_;

  /// Callers store zero here to inhibit the error message for
  /// unrecognized options.
  int opt_err_;

  /// Treat all options as long options.
  int long_only_;

  /// Keeps track of whether or not a colon was passed in <optstring>.
  /// This is used to determine the return value when required 
  /// arguments are missing.
  int has_colon_;
  
  /// Points to the option argument when one is found on last call to
  /// <operator()>.
  ACE_TCHAR *opt_arg_;

  /**
   * The next char to be scanned in the option-element in which the
   * last option character we returned was found.  This allows us to
   * pick up the scan where we left off.
   *
   * If this is zero, or a null string, it means resume the scan
   * by advancing to the next <argv>-element.
   */
  ACE_TCHAR *nextchar_;

  /// Keeps track of ordering mode (default <PERMUTE_ARGS>).
  int ordering_;

  /// Index of the first non-option <argv>-element found (only valid
  /// when permuting).
  int nonopt_start_;
  
  /// Index of the <argv>-element following the last non-option element 
  /// (only valid when permuting).
  int nonopt_end_;

  /// Points to the long_option found on last call to <operator()>.
  ACE_Get_Opt_Long_Option *long_option_;

  /// Array of long options.
  ACE_Array<ACE_Get_Opt_Long_Option*> long_opts_;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  ACE_UNIMPLEMENTED_FUNC (ACE_Get_Opt (const ACE_Get_Opt &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Get_Opt &operator= (const ACE_Get_Opt &))
};

#if defined (__ACE_INLINE__)
#include "ace/Get_Opt.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_GET_OPT_H */
