/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_Event_Tuple::ACE_Event_Tuple (void)
:  event_handler_ (0),
   handle_ (ACE_INVALID_HANDLE)
{
}
		   
ACE_INLINE
ACE_Event_Tuple::ACE_Event_Tuple (ACE_Event_Handler* eh,
				  ACE_HANDLE h)
: event_handler_ (eh),
  handle_ (h)
{
}

