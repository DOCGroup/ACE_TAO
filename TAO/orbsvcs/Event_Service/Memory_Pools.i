/* -*- C++ -*- */
// $Id$

ACE_INLINE void *
ACE_ES_Memory_Pools::new_Event_Container (void)
{
  return Event_Container_->malloc (sizeof (ACE_ES_Event_Container));
}

ACE_INLINE void
ACE_ES_Memory_Pools::delete_Event_Container (void *mem)
{
  Event_Container_->free (mem);
}

ACE_INLINE void *
ACE_ES_Memory_Pools::new_Dispatch_Request (void)
{
  return Dispatch_Request_->malloc (sizeof (ACE_ES_Dispatch_Request));
}

ACE_INLINE void
ACE_ES_Memory_Pools::delete_Dispatch_Request (void *mem)
{
  Dispatch_Request_->free (mem);
}

#define USE_MEM_POOLS 1

ACE_INLINE void *
ACE_ES_Memory_Pools::new_Event (size_t len)
{
#if USE_MEM_POOLS
  const u_int ev_size = sizeof (RtecEventComm::Event);
  const u_int size = (ev_size % ACE_MALLOC_ALIGN) ?
      ((ev_size / ACE_MALLOC_ALIGN) + 1) * ACE_MALLOC_ALIGN : ev_size;

  char *const addr = (char *) Event_->malloc (len * size);
#else
  char *const addr = new char[len * sizeof (ACE_ES_Event)];
#endif

  return addr;
}

ACE_INLINE void
ACE_ES_Memory_Pools::delete_Event (void *mem)
{
#if USE_MEM_POOLS
  Event_->free (mem);
#else
  delete [] mem;
#endif
}
