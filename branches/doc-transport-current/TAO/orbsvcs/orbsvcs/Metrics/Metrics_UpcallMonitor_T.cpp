// $Id$

#ifndef METRICS_UPCALL_MONITOR_T_CPP
#define METRICS_UPCALL_MONITOR_T_CPP

#include "orbsvcs/Metrics/Metrics_UpcallMonitor_T.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Metrics/Metrics_UpcallMonitor_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* METRICS_UPCALL_MONITOR_T_CPP */

