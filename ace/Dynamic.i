/* -*- C++ -*- */
// $Id$

// Dynamic.i

ACE_INLINE void *
ACE_Dynamic::set (void *x)
{ 
  ACE_TRACE ("ACE_Dynamic::set");
  return this->instance_ = x; 
}

ACE_INLINE int 
ACE_Dynamic::is_dynamic (void *x)
{ 
  ACE_TRACE ("ACE_Dynamic::is_dynamic");
  return this->instance_ == x;
}
