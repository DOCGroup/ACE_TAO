// $Id$

#include "Memory_Pools.h"
#include "Event_Channel.h"

#if !defined (__ACE_INLINE__)
#include "Memory_Pools.i"
#endif /* __ACE_INLINE__ */

ACE_TSS<ACE_ES_Dispatch_Request_Allocator> ACE_ES_Memory_Pools::Dispatch_Request_;
ACE_TSS<ACE_ES_Event_Container_Allocator> ACE_ES_Memory_Pools::Event_Container_;
ACE_TSS<ACE_ES_Event_Allocator> ACE_ES_Memory_Pools::Event_;

// ************************************************************

int
ACE_ES_Memory_Pools::thr_init (void)
{
  // Allocate the memory pool for this thread.
  Event_Container_.ts_object ();
  Dispatch_Request_.ts_object ();
  Event_.ts_object ();
  return 0;
}

// ************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// For ACE_ES_Memory_Pools.
template class ACE_TSS<ACE_ES_Event_Container_Allocator>;
template class ACE_TSS<ACE_ES_Dispatch_Request_Allocator>;
template class ACE_TSS<ACE_Malloc<ACE_Local_Memory_Pool, ACE_Local_Memory_Pool_Options, ACE_MEMORY_POOL_MUTEX> >;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<ACE_ES_Event_Container_Allocator>
#pragma instantiate ACE_TSS<ACE_ES_Dispatch_Request_Allocator>
#pragma instantiate ACE_TSS<ACE_Malloc<ACE_Local_Memory_Pool, ACE_Local_Memory_Pool_Options, ACE_MEMORY_POOL_MUTEX> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
