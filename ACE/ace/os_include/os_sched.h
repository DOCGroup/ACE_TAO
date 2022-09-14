// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_sched.h
 *
 *  execution scheduling (REALTIME)
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_SCHED_H
#define ACE_OS_INCLUDE_OS_SCHED_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_time.h"

#if !defined (ACE_LACKS_SCHED_H)
# include /**/ <sched.h>
#endif /* !ACE_LACKS_SCHED_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// We need a cpu_set_t - most platforms have it natively (ACE_HAS_CPU_SET_T).
// At least one has a similar cpuset_t (ACE_HAS_CPUSET_T)
// All others get it defined here.
#if !defined (ACE_HAS_CPU_SET_T)
#  if defined (ACE_HAS_CPUSET_T)
   typedef cpuset_t cpu_set_t;
#  else
#    define ACE_CPU_SETSIZE 1024
   typedef struct
   {
     ACE_UINT32 bit_array_[ACE_CPU_SETSIZE / (8 * sizeof (ACE_UINT32))];
   } cpu_set_t;
#endif /* ACE_HAS_CPUSET_T */
#endif /* ACE_HAS_CPU_SET_T */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_SCHED_H */
