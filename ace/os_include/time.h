// -*- C++ -*-

//=============================================================================
/**
 *  @file    time.h
 *
 *  time types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_TIME_H
#define ACE_OS_INCLUDE_TIME_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @todo should we include anything from signal.h?
#include "ace/os_include/sys/types.h"

#if !defined (ACE_LACKS_TIME_H)
# include /**/ <time.h>
#endif /* !ACE_LACKS_TIME_H */

# if defined (ACE_HAS_BROKEN_CTIME)
#   undef ctime
# endif /* ACE_HAS_BROKEN_CTIME */

// There are a lot of threads-related macro definitions in the config files.
// They came in at different times and from different places and platform
// requirements as threads evolved.  They are probably not all needed - some
// overlap or are otherwise confused.  This is an attempt to start
// straightening them out.
# if defined (ACE_HAS_PTHREADS_STD)    /* POSIX.1c threads (pthreads) */
  // ... and 2-parameter asctime_r and ctime_r
#   if !defined (ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R) && \
       !defined (ACE_HAS_STHREADS)
#     define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#   endif
# endif /* ACE_HAS_PTHREADS_STD */

#if defined (ACE_LACKS_STRPTIME_PROTOTYPE) && !defined (_XOPEN_SOURCE)
extern "C" char *strptime (const char *s, const char *fmt, struct tm *tp);
#endif  /* ACE_LACKS_STRPTIME_PROTOTYPE */

#if defined (ACE_PSOS) && !defined (USER_INCLUDE_SYS_TIME_TM)
#  if defined (ACE_PSOS_DIAB_PPC)
     typedef struct timespec timespec_t;
#  else /* ! defined (ACE_PSOS_DIAB_PPC) */
     typedef struct timespec
     {
       time_t tv_sec; // Seconds
       long tv_nsec; // Nanoseconds
     } timespec_t;
#  endif /* defined (ACE_PSOS_DIAB_PPC) */
#endif /*  defined (ACE_PSOS) && !defined (USER_INCLUDE_SYS_TIME_TM) */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_TIME_H */
