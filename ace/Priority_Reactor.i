/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_Event_Tuple::ACE_Event_Tuple (void)
:  handle_ (ACE_INVALID_HANDLE),
   event_handler_ (0) 
{
}
		   
ACE_INLINE
ACE_Event_Tuple::ACE_Event_Tuple (ACE_Event_Handler* eh,
				  ACE_HANDLE h)
: handle_ (h),
  event_handler_ (eh)
{
}

