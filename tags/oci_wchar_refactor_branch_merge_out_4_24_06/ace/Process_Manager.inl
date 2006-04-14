/* -*- C++ -*- */
// $Id$

ACE_INLINE size_t
ACE_Process_Manager::managed (void) const
{
  return current_count_;
}
