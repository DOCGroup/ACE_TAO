// ============================================================================
//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Scheduler.cpp
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHEDULER_C
#define TAO_RECONFIG_SCHEDULER_C

#include "Reconfig_Scheduler_T.h"
#include "Reconfig_Scheduler.h"

// The templatized method parameters needed by this file are
// hopelessly broken on pre-2.8 versions of g++
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "Reconfig_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Sched, Reconfig_Scheduler, "$Id$")

#endif /* __GNUC__ */

#endif /* TAO_RECONFIG_SCHEDULER_C */
