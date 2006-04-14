// $Id$

#ifndef METRICS_UPCALL_MONITOR_T_CPP
#define METRICS_UPCALL_MONITOR_T_CPP

#include "Metrics_UpcallMonitor_T.h"

#if !defined (__ACE_INLINE__)
#include "Metrics_UpcallMonitor_T.i"
#endif /* __ACE_INLINE__ */

// Virtual destructor: destroy actual consumer if we own it.

template <class ACE_LOCK, class ALLOCATOR>
TAO_Metrics_UpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>::
~TAO_Metrics_UpcallMonitorAdapter ()
{
  if (adapter_owns_consumer_)
    {
      delete (& consumer_);
    }
}

#endif /* METRICS_UPCALL_MONITOR_T_CPP */

