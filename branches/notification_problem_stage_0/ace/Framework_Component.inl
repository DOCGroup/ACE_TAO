/* -*- C++ -*- */
// $Id$

// Framework_Component.inl

#include "ace/ACE.h"

ACE_INLINE 
ACE_Framework_Component::ACE_Framework_Component (void *_this, 
                                                  const ACE_TCHAR *dll_name,
                                                  const ACE_TCHAR *name)
  :  this_ (_this),
     dll_name_ (ACE::strnew (dll_name ? dll_name : ACE_LIB_TEXT (""))),
     name_ (ACE::strnew (name ? name : ACE_LIB_TEXT ("")))
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
