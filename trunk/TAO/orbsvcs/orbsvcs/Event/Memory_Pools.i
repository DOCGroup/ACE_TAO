/* -*- C++ -*- */
// $Id$

ACE_INLINE void *
ACE_ES_Memory_Pools::new_Event_Container (void)
{
  return Event_Container_::instance ()->malloc (sizeof (ACE_ES_Event_Container));
}

ACE_INLINE void
ACE_ES_Memory_Pools::delete_Event_Container (void *mem)
{
  Event_Container_::instance ()->free (mem);
}

ACE_INLINE void *
ACE_ES_Memory_Pools::new_Dispatch_Request (void)
{
  return Dispatch_Request_::instance ()->malloc (sizeof (ACE_ES_Dispatch_Request));
}

ACE_INLINE void
ACE_ES_Memory_Pools::delete_Dispatch_Request (void *mem)
{
  Dispatch_Request_::instance ()->free (mem);
}
