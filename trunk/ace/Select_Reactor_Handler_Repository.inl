/* -*- C++ -*- */
//$Id$
ACE_INLINE
ACE_Event_Tuple::~ACE_Event_Tuple (void)
{
}

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

ACE_INLINE int
ACE_Event_Tuple::operator== (const ACE_Event_Tuple &rhs) const
{
  return this->handle_ == rhs.handle_;
}

ACE_INLINE int
ACE_Event_Tuple::operator!= (const ACE_Event_Tuple &rhs) const
{
  return !(*this == rhs);
}

/************************************************************/

ACE_INLINE size_t
ACE_Select_Reactor_Handler_Repository::size (void) const
{
  return this->max_size_;
}
