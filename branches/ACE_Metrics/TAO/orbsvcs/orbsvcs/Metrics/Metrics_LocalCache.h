// $Id$

#ifndef METRICS_LOCAL_CACHE_H
#define METRICS_LOCAL_CACHE_H


#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/Metrics_Cache.h"
#include "ace/Metrics_Cache_T.h"
#include "ace/Singleton.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/MetricsC.h"

#include "Metrics_Utils.h"

#if defined (__ACE_INLINE__)
#include "Metrics_LocalCache.i"
#endif /* __ACE_INLINE__ */

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

#endif /* METRICS_LOCAL_CACHE_H */

