/* -*- C++ -*- */
// $Id$

// Framework_Component.inl

ACE_INLINE 
ACE_Framework_Component::ACE_Framework_Component (const void *_this)
  :  this_ (_this)
{
  ACE_TRACE ("ACE_Framework_Component::ctor");
}

/***************************************************************/

ACE_INLINE int
ACE_Framework_Repository::current_size (void) const
{
  ACE_TRACE ("ACE_Framework_Repository::current_size");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->lock_, -1));
  return this->current_size_;
}

ACE_INLINE int
ACE_Framework_Repository::total_size (void) const
{
  ACE_TRACE ("ACE_Framework_Repository::total_size");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->lock_, -1));
  return this->total_size_;
}
