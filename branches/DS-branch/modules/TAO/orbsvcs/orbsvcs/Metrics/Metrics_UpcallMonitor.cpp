// $Id$

#ifndef METRICS_UPCALL_MONITOR_CPP
#define METRICS_UPCALL_MONITOR_CPP

#include "orbsvcs/Metrics/Metrics_UpcallMonitor.h"

#if defined (ACE_METRICS_COLLECTION)

#if defined (ACE_ENABLE_TIMEPROBES) && defined (ACE_COMPILE_TIMEPROBES)

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Metrics/Metrics_UpcallMonitor.inl"
#endif /* __ACE_INLINE__ */

#else
#error ACE_COMPILE_TIMEPROBES must be defined in order to collect metrics
#endif /* ACE_ENABLE_TIMEPROBES & ACE_COMPILE_TIMEPROBES */
#endif /* ACE_METRICS_COLLECTION */


#endif /* METRICS_UPCALL_MONITOR_CPP */

