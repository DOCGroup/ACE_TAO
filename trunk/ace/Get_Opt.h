/* -*- C++ -*- */
// $Id$

/* ACE_Get_Opt for GNU.
   Copyright (C) 1987, 1989 Free Software Foundation, Inc.  (Modified
   by Douglas C. Schmidt for use with C++.)

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 1, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Get_Opt.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_GET_OPT_H)
#define ACE_GET_OPT_H

#include "ace/ACE.h"

class ACE_Export ACE_Get_Opt
  // = TITLE
  //   Iterator for parsing command-line arguments.
  //
  // = DESCRIPTION
  // This version of `get_opt' appears to the caller like standard
  // Unix `get_opt' but it behaves differently for the user, since it
  // allows the user to intersperse the options with the other
  // arguments.
  // 
  // As `get_opt' works, it permutes the elements of `argv' so that,
  // when it is done, all the options precede everything else.  Thus
  // all application programs are extended to handle flexible argument order.
  // 
  // Setting the environment variable _POSIX_OPTION_ORDER disables permutation.
  // Then the behavior is completely standard.
  // 
  // GNU application programs can use a third alternative mode in which
  // they can distinguish the relative order of options and other arguments. 

{
public:
  ACE_Get_Opt (int argc, char **argv, char *optstring, int skip_argv0 = 1, int report_errors = 0);
  // Initialize the internal data when the first call is made.  Start
  // processing options with ARGV-element 0 + <skip_argv0>; the
  // sequence of previously skipped non-option ARGV-elements is empty.

  int operator () (void);
  // Scan elements of ARGV (whose length is ARGC) for option
  // characters given in OPTSTRING.
  //
  // If an element of ARGV starts with '-', and is not exactly "-" or "--",
  // then it is an option element.  The characters of this element
  // (aside from the initial '-') are option characters.  If `get_opt'
  // is called repeatedly, it returns successively each of the option characters
  // from each of the option elements.
  // 
  // If `get_opt' finds another option character, it returns that character,
  // updating `optind' and `nextchar' so that the next call to `get_opt' can
  // resume the scan with the following option character or ARGV-element.
  // 
  // If there are no more option characters, `get_opt' returns `EOF'.
  // Then `optind' is the index in ARGV of the first ARGV-element
  // that is not an option.  (The ARGV-elements have been permuted
  // so that those that are not options now come last.)
  // 
  // OPTSTRING is a string containing the legitimate option characters.
  // A colon in OPTSTRING means that the previous character is an option
  // that wants an argument.  The argument is taken from the rest of the
  // current ARGV-element, or from the following ARGV-element,
  // and returned in `optarg'.
  // 
  // If an option character is seen that is not listed in OPTSTRING,
  // return '?' after printing an error message.  If you set `opterr' to
  // zero, the error message is suppressed but we still return '?'.
  // 
  // If a char in OPTSTRING is followed by a colon, that means it wants an arg,
  // so the following text in the same ARGV-element, or the text of the following
  // ARGV-element, is returned in `optarg.  Two colons mean an option that
  // wants an optional arg; if there is text in the current ARGV-element,
  // it is returned in `optarg'.
  // 
  // If OPTSTRING starts with `-', it requests a different method of handling the
  // non-option ARGV-elements.  See the comments about RETURN_IN_ORDER, above.  */

  // = Public data members (should be hidden...).

  char *optarg;
  // For communication from `get_opt' to the caller.  When `get_opt'
  // finds an option that takes an argument, the argument value is
  // returned here.  Also, when `ordering' is RETURN_IN_ORDER, each
  // non-option ARGV-element is returned here.
  
  int optind;
  // Index in ARGV of the next element to be scanned.  This is used
  // for communication to and from the caller and for communication
  // between successive calls to `get_opt'.  On entry to `get_opt',
  // zero means this is the first call; initialize.
  //      
  // When `get_opt' returns EOF, this is the index of the first of the
  // non-option elements that the caller should itself scan.
  //          
  // Otherwise, `optind' communicates from one call to the next
  // how much of ARGV has been scanned so far.  

  int opterr;
  // Callers store zero here to inhibit the error message for
  // unrecognized options.
  
  // = GNU extensions
  int    nargc;
  char **nargv;
  char  *noptstring;
  
  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  
  char *nextchar;
  // The next char to be scanned in the option-element in which the
  // last option character we returned was found.  This allows us to
  // pick up the scan where we left off.
  //    
  // If this is zero, or a null string, it means resume the scan
  // by advancing to the next ARGV-element.  
  
  int first_nonopt;
  int last_nonopt;
  // Describe the part of ARGV that contains non-options that have
  // been skipped.  `first_nonopt' is the index in ARGV of the first
  // of them; `last_nonopt' is the index after the last of them.
};

#if defined (__ACE_INLINE__)
#include "ace/Get_Opt.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_GET_OPT_H */
