/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Get_Opt.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_GET_OPT_H
#define ACE_GET_OPT_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#undef optind
#undef optarg
#undef opterr

/**
 * @class ACE_Get_Opt
 *
 * @brief Iterator for parsing command-line arguments.
 *
 * This is a C++ wrapper for getopt(3c).
 */
class ACE_Export ACE_Get_Opt
{
public:
  /**
   * Initialize the internal data when the first call is made.  Start
   * processing options with <argv>-element 0 + <skip_argv0>; the
   * sequence of previously skipped non-option <argv>-elements is
   * empty.
   *
   * <optstring> is a string containing the legitimate option
   * characters.  A colon in <optstring> means that the previous
   * character is an option that wants an argument.  The argument is
   * taken from the rest of the current <argv>-element, or from the
   * following <argv>-element, and returned in <optarg>.
   *
   * If an option character is seen that is not listed in <optstring>,
   * return '?' after printing an error message.  If you set
   * <report_errors> to zero, the error message is suppressed but we
   * still return '?'.
   *
   * If a char in <optstring> is followed by a colon, that means it
   * wants an arg, so the following text in the same <argv>-element,
   * or the text of the following <argv>-element, is returned in
   * <optarg>.  */
  ACE_Get_Opt (int argc,
               ACE_TCHAR **argv,
               const ACE_TCHAR *optstring,
               int skip_argv0 = 1,
               int report_errors = 0);

  /// Default dtor.
  ~ACE_Get_Opt (void);

  /**
   * Scan elements of <argv> (whose length is <argc>) for option
   * characters given in <optstring>.
   *
   * If an element of <argv> starts with '-', and is not exactly "-"
   * or "--", then it is an option element.  The characters of this
   * element (aside from the initial '-') are option characters.  If
   * <operator()> is called repeatedly, it returns successively each
   * of the option characters from each of the option elements.
   *
   * If <operator()> finds another option character, it returns that
   * character, updating <optind> and <nextchar> so that the next call
   * to <operator()> can resume the scan with the following option
   * character or <argv>-element.
   *
   * If there are no more option characters, <operator()> returns
   * <EOF>.  Then <optind> is the index in <argv> of the first
   * <argv>-element that is not an option.  (The <argv>-elements have
   * been permuted so that those that are not options now come last.)
   */
  int operator () (void);

  // = Public data members (should be hidden...).

  /**
   * For communication from <operator()> to the caller.  When
   * <operator()> finds an option that takes an argument, the argument
   * value is returned here.
   */
  ACE_TCHAR *optarg;

  /**
   * Index in <argv> of the next element to be scanned.  This is used
   * for communication to and from the caller and for communication
   * between successive calls to <operator()>.  On entry to
   * <operator()>, zero means this is the first call; initialize.
   *
   * When <get_opt> returns <EOF>, this is the index of the first of
   * the non-option elements that the caller should itself scan.
   *
   * Otherwise, <optind> communicates from one call to the next how
   * much of <argv> has been scanned so far.
   */
  int optind;

  /// Callers store zero here to inhibit the error message for
  /// unrecognized options.
  int opterr;

  /// Holds the <argc> count.
  int argc_;

  /// Holds the <argv> pointer.
  ACE_TCHAR **argv_;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /**
   * The next char to be scanned in the option-element in which the
   * last option character we returned was found.  This allows us to
   * pick up the scan where we left off.
   *
   * If this is zero, or a null string, it means resume the scan
   * by advancing to the next <argv>-element.
   */
  ACE_TCHAR *nextchar_;

  /// Holds the option string.
  const ACE_TCHAR *optstring_;

  ACE_UNIMPLEMENTED_FUNC (ACE_Get_Opt (const ACE_Get_Opt &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Get_Opt &operator= (const ACE_Get_Opt &))
};

#if defined (__ACE_INLINE__)
#include "ace/Get_Opt.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_GET_OPT_H */
