/* -*- C++ -*- */
// $Id$

ACE_INLINE ACE_event_t
ACE_Event::handle (void) const
{
  return this->handle_;
}

ACE_INLINE void
ACE_Event::handle (ACE_event_t new_handle)
{
  this->handle_ = new_handle;
}

