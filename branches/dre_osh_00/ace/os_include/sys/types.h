// -*- C++ -*-

//=============================================================================
/**
 *  @file    types.h
 *
 *  data types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_TYPES_H
#define ACE_OS_INCLUDE_SYS_TYPES_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/stddef.h"

#if !defined (ACE_LACKS_SYS_TYPES_H)
# include /**/ <sys/types.h>
#endif /* !ACE_LACKS_SYS_TYPES_H */

// todo: don't forget to clean this up!  ;-)
# if !defined (ACE_HAS_CLOCK_GETTIME) && !(defined (_CLOCKID_T_) || defined (_CLOCKID_T))
typedef int clockid_t;
#   if !defined (CLOCK_REALTIME)
#     define CLOCK_REALTIME 0
#   endif /* CLOCK_REALTIME */
# endif /* ! ACE_HAS_CLOCK_GETTIME && ! _CLOCKID_T_ */



#if defined (ACE_HAS_WINCE)
#  include /**/ <types.h>

// CE's add-on for c-style fstat/stat functionalities.  This struct is
// by no mean complete compared to what you usually find in UNIX
// platforms.  Only members that have direct conversion using Win32's
// BY_HANDLE_FILE_INFORMATION are defined so that users can discover
// non-supported members at compile time.  Time values are of type
// ACE_Time_Value for easy comparison.

// Since CE does not have _stat by default as NT/2000 does, the 'stat'
// struct defined here will be used.  Also note that CE file system
// struct is only for the CE 3.0 or later.
// Refer to the WCHAR.H from Visual C++ and WIBASE.H from eVC 3.0.

typedef unsigned int dev_t;
#endif /* ACE_HAS_WINCE */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_TYPES_H */
