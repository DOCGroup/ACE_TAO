// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_stddef.h
 *
 *  standard type definitions
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

// From http://www.UNIX-systems.org/single_unix_specification/

#ifndef ACE_OS_INCLUDE_OS_STDDEF_H
#define ACE_OS_INCLUDE_OS_STDDEF_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @todo move this to a windows config file
// Add new macro ACE_LACKS_STDDEF_H
#if defined (ACE_HAS_WINCE) && _WIN32_WCE < 400 
#  if !define (ACE_LACKS_STDDEF_H)
#    define ACE_LACKS_STDDEF_H
#    define ACE_LACKS_PTRDIFF_T
#  endif /* !ACE_LACKS_STDDEF_H */
#endif /* ACE_HAS_WINCE && _WIN32_WCE < 400 */

#if !defined (ACE_LACKS_STDDEF_H)
#  include /**/ <stddef.h>
#endif /* !ACE_LACKS_STDDEF_H */

// NULL pointer constant
#if defined (ACE_LACKS_NULL)
#  undef NULL
#  if defined(__cplusplus)
#    define NULL 0
#  else
#    define NULL ((void *)0)
#  endif
#endif /* ACE_LACKS_NULL */

/*
  Integer constant expression of type size_t, the value of which is the offset 
  in bytes to the structure member (member-designator), from the beginning of 
  its structure (type).
*/
#if defined (ACE_LACKS_OFFSETOF)
#  undef offsetof
#  define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif /* ACE_LACKS_OFFSETOF */

//  Signed integer type of the result of subtracting two pointers.
#if defined (ACE_LACKS_PTRDIFF_T)
#  if !defined (ACE_PTRDIFF_T_TYPE)
#    define ACE_PTRDIFF_T_TYPE unsigned long
#  endif /* !ACE_PTRDIFF_T_TYPE */
   typedef ACE_PTRDIFF_T_TYPE ptrdiff_t;
#endif /* ACE_LACKS_PTRDIFF_T */

/*
  Integer type whose range of values can represent distinct wide-character 
  codes for all members of the largest character set specified among the 
  locales supported by the compilation environment: the null character has 
  the code value 0 and each member of the portable character set has a code 
  value equal to its value when used as the lone character in an integer 
  character constant.
*/
#if defined (ACE_LACKS_WCHAR_T)
#  if !defined (ACE_WCHAR_T_TYPE)
#    define ACE_WCHAR_T_TYPE long;
#  endif /* !ACE_WCHAR_T_TYPE */
   typedef ACE_WCHAR_T_TYPE wchar_t;
#endif /* ACE_LACKS_WCHAR_T */

//  Unsigned integer type of the result of the sizeof operator.
#if defined (ACE_LACKS_SIZE_T)
#  if !defined (ACE_SIZE_T_TYPE)
#    define ACE_SIZE_T_TYPE unsigned int;
#  endif /* !ACE_SIZE_T_TYPE */
   typedef ACE_SIZE_T_TYPE size_t;
#endif /* ACE_LACKS_SIZE_T */

#if defined (ACE_PSOS_DIAB_MIPS)
#  undef size_t
   typedef unsigned int size_t;
#endif

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_STDDEF_H */
