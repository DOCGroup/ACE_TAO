/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Metrics_Cache.h
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

#ifndef ACE_METRICS_CACHE_H
#define ACE_METRICS_CACHE_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_METRICS_COLLECTION)

#if defined (ACE_ENABLE_TIMEPROBES) && defined (ACE_COMPILE_TIMEPROBES)
#  define REGISTER_METRICS_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_HANDLE) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_HANDLE = METRICS_PTR-> \
register_##METRICS_REGION ( \
PROBE_NAME, \
PROBE_TYPE); \
} } while (0)

#  define REGISTER_METRICS_REPORTING_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_LOGGER_REF,METRICS_HANDLE) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_HANDLE = METRICS_PTR-> \
register_##METRICS_REGION## ( \
PROBE_NAME, \
PROBE_TYPE, \
METRICS_LOGGER_REF); \
} } while (0)

#  define START_DISPATCH_METRIC(METRICS_PTR,NAME,X) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_PTR->report_##NAME##_start(X); \
} } while (0)

#  define STOP_DISPATCH_METRIC(METRICS_PTR,NAME,X) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_PTR->report_##NAME##_stop(X); \
} } while (0)

#  define PAUSE_DISPATCH_METRIC(METRICS_PTR,NAME,X) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_PTR->report_##NAME##_suspend(X); \
} } while (0)

#  define RESUME_DISPATCH_METRIC(METRICS_PTR,NAME,X) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_PTR->report_##NAME##_resume(X); \
} } while (0)

#  define START_METRIC(METRICS_PTR,NAME) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_PTR->report_##NAME##_start(); \
} } while (0)

#  define STOP_METRIC(METRICS_PTR,NAME) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_PTR->report_##NAME##_stop(); \
} } while (0)

#if defined (__ACE_INLINE__)
#include "ace/Metrics_Cache.i"
#endif /* __ACE_INLINE__ */

#include "ace/Metrics_Cache_T.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"

#if defined (_MSC_VER)
// Disable warning of using Microsoft Extension.
#pragma warning(disable:4231)
#endif /* _MSC_VER */

#if defined (_MSC_VER)
// Default back the warning of using Microsoft Extension.
#pragma warning(default:4231)
#endif /* _MSC_VER */

#else
#error ACE_Compile_Timeprobes must be defined in order to collect metrics
#endif /* ACE_ENABLE_TIMEPROBES & ACE_COMPILE_TIMEPROBES */

#else

#  define REGISTER_METRICS_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_HANDLE)
#  define REGISTER_METRICS_REPORTING_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_LOGGER_REF,METRICS_HANDLE)
#  define START_DISPATCH_METRIC(METRICS_PTR,NAME,X)
#  define STOP_DISPATCH_METRIC(METRICS_PTR,NAME,X)
#  define PAUSE_DISPATCH_METRIC(METRICS_PTR,NAME,X)
#  define RESUME_DISPATCH_METRIC(METRICS_PTR,NAME,X)
#  define START_METRIC(METRICS_PTR,NAME)
#  define STOP_METRIC(METRICS_PTR,NAME)

#endif /* ACE_ENABLE_TIMEPROBES && ACE_COMPILE_TIMEPROBES */

#endif /* ACE_METRICS_CACHE_H */
