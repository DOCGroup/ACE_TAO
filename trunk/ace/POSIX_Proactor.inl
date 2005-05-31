// -*- C++ -*-
//
// $Id$

ACE_INLINE 
ACE_Asynch_Pseudo_Task& ACE_POSIX_Proactor::get_asynch_pseudo_task (void)
{
  return this->pseudo_task_;
}
