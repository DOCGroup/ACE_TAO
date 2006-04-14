// -*- C++ -*-
//
// $Id$

#ifndef METRICS_LOCAL_CACHE_H
#define METRICS_LOCAL_CACHE_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/Metrics_Cache.h"
#include "ace/Metrics_Cache_T.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/MetricsC.h"

#include "Metrics_Utils.h"

#if defined (ACE_METRICS_COLLECTION)

#if defined (ACE_ENABLE_TIMEPROBES) && defined (ACE_COMPILE_TIMEPROBES)

#include "Metrics_LocalCache_T.h"

typedef TAO_Metrics_LocalCache<ACE_SYNCH_MUTEX, ACE_New_Allocator>
TAO_METRICS_CACHE_TYPE;

typedef ACE_Singleton<TAO_METRICS_CACHE_TYPE, ACE_SYNCH_MUTEX>
TAO_METRICS_CACHE_SINGLETON_TYPE;

typedef TAO_Metrics_ReportingLocalCache<ACE_SYNCH_MUTEX, ACE_New_Allocator>
TAO_METRICS_REPORTING_CACHE_TYPE;

typedef ACE_Singleton<TAO_METRICS_REPORTING_CACHE_TYPE, ACE_SYNCH_MUTEX>
TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE;

#if defined (TAO_USES_REPORTING_METRICS)
typedef TAO_METRICS_REPORTING_CACHE_TYPE TAO_METRICS_CACHE;
typedef TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE TAO_METRICS_CACHE_SINGLETON;
#else
typedef TAO_METRICS_CACHE_TYPE TAO_METRICS_CACHE;
typedef TAO_METRICS_CACHE_SINGLETON_TYPE TAO_METRICS_CACHE_SINGLETON;
#endif

/**
 * NOTE: This is a simple macro which calls the appropriate registration function with the cache to
 *       store the data from this pointer. At the ACE level there is no need to give a PROBE_TYPE
 *       because there is only one probe type (ACE_Timeprobe).
 *
 * USAGE_LEVEL: TAO
 *
 * Q1: Where is this and the next macro used? One example is on TAO/orbsvcs/orbsvcs/Metrics. No where in the ace code.
 *    So is this a ACE or TAO macro? 
 *
 * Q2: Is the function (register + METRICS_REGION) a ACE or TAO?
 * A2: TAO. Define in Metrics_Local_Cache_T.h. The only METRICS_REGION that exists is 'base_metrics'. Used
 *     in $TAO_ROOT/orbsvcs/orbsvcs/Metrics/Metrics_Utils.cpp
 *
 * @param METRICS_PTR - Initialized pointer to the Metrics_Cache. This pointer is used to register the probe
 *                      with the associated cache.
 * @param METIRCS_REGION - At present there is only one 'base_metrics'.
 * @param PROBE_NAME - String name of the probe
 * @param PROBE_TYPE - Unsigned integer representing the probe type
 * @param METRICS_HANDLE - Variable used to contain the metrics probe handle that is the result of this function
 *                         The type of the handle is a RtecScheduler::handle_t
 */
#  define REGISTER_METRICS_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_HANDLE) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_HANDLE = METRICS_PTR-> \
register_##METRICS_REGION ( \
PROBE_NAME, \
PROBE_TYPE); \
} } while (0)

/**
 * Same as above but instead of just reporting information to a cache there is also reference to a Metrics_Loger (TAO feature).
 * The logger is used to record the information. The type of probe is register with the logger
 *
 * USAGE_LEVEL: TAO
 */
#  define REGISTER_METRICS_REPORTING_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_LOGGER_REF,METRICS_HANDLE) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_HANDLE = METRICS_PTR-> \
register_##METRICS_REGION## ( \
PROBE_NAME, \
PROBE_TYPE, \
METRICS_LOGGER_REF); \
} } while (0)

#if defined (__ACE_INLINE__)
#include "Metrics_LocalCache.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ENABLE_TIMEPROBES && ACE_COMPILE_TIMEPROBES */
#endif /* ACE_METRICS_COLLECTION */

#include /**/ "ace/post.h"

#endif /* METRICS_LOCAL_CACHE_H */

