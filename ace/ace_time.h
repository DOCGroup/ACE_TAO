/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_time.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_TIME_H
# define ACE_ACE_TIME_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_TIME_H)
#   include /**/ <time.h>
# endif /* ACE_LACKS_TIME_H */

# if defined (ACE_NEEDS_SYS_TIMES_H)
    include /**/ <sys/times.h>
# endif /* ACE_NEEDS_SYS_TIMES_H) */

# if defined (ACE_NEEDS_SYS_TIME_H)
#   include /**/ <sys/time.h>
# endif /* ACE_NEEDS_SYS_TIME_H */

# if defined (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB) && \
             (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB != 0)
using std::time_t;
using std::tm;
#   if defined (ACE_WIN32)
using std::_timezone;
#   else
using std::timezone;
#   endif
using std::difftime;
# endif /* ACE_USES_STD_NAMESPACE_FOR_STDC_LIB */

# if !defined (ACE_HAS_CLOCK_GETTIME) && !(defined (_CLOCKID_T_) || defined (_CLOCKID_T))
typedef int clockid_t;
#   if !defined (CLOCK_REALTIME)
#     define CLOCK_REALTIME 0
#   endif /* CLOCK_REALTIME */
# endif /* ! ACE_HAS_CLOCK_GETTIME && ! _CLOCKID_T_ */

// does this belong here?
# if defined (ACE_HAS_BROKEN_CTIME)
#   undef ctime
# endif /* ACE_HAS_BROKEN_CTIME */

# if defined (ACE_LACKS_STRPTIME_PROTOTYPE) && !defined (_XOPEN_SOURCE)
extern "C" char *strptime (const char *s, const char *fmt, struct tm *tp);
# endif  /* ACE_LACKS_STRPTIME_PROTOTYPE */


#endif /* ACE_ACE_TIME_H */

