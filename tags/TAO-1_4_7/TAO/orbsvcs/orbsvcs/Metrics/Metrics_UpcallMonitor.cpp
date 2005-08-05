// $Id$

#ifndef METRICS_UPCALL_MONITOR_CPP
#define METRICS_UPCALL_MONITOR_CPP

#include "Metrics_UpcallMonitor.h"

#if defined (ACE_METRICS_COLLECTION)

#if defined (ACE_ENABLE_TIMEPROBES) && defined (ACE_COMPILE_TIMEPROBES)

#if !defined (__ACE_INLINE__)
#include "Metrics_UpcallMonitor.i"
#endif /* __ACE_INLINE__ */

// Added by joeh to fix undefined symbols for VxWorks load
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Metrics_LocalCache<ACE_Thread_Mutex, ACE_New_Allocator>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
#error ACE_COMPILE_TIMEPROBES must be defined in order to collect metrics
#endif /* ACE_ENABLE_TIMEPROBES & ACE_COMPILE_TIMEPROBES */
#endif /* ACE_METRICS_COLLECTION */


#endif /* METRICS_UPCALL_MONITOR_CPP */

