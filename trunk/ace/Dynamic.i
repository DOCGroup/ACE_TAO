/* -*- C++ -*- */
// $Id$

// Dynamic.i

ACE_INLINE void
ACE_Dynamic::set (void)
{ 
  ACE_TRACE ("ACE_Dynamic::set");
  this->is_dynamic_ = 1; 
}

ACE_INLINE int 
ACE_Dynamic::is_dynamic ()
{ 
  ACE_TRACE ("ACE_Dynamic::is_dynamic");
  return this->is_dynamic_;
}

ACE_INLINE void
ACE_Dynamic::reset (void)
{ 
  ACE_TRACE ("ACE_Dynamic::set");
  this->is_dynamic_ = 0; 
}

