/* -*- C++ -*- */
// $Id$

// Thread_Manager.i

#include "ace/Log_Msg.h"

// Unique thread id.  
ACE_INLINE ACE_thread_t 
ACE_Thread_Descriptor::self (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  return this->thr_id_;
}
  
// Unique kernel-level thread handle.

ACE_INLINE void 
ACE_Thread_Descriptor::self (ACE_hthread_t &handle)
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  handle = this->thr_handle_;
}  

// Group ID.

ACE_INLINE int 
ACE_Thread_Descriptor::grp_id (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::grp_id");
  return grp_id_;
}

// Current state of the thread.
ACE_INLINE ACE_Thread_State 
ACE_Thread_Descriptor::state (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::state");
  return thr_state_;
}

// Set the exit status.

ACE_INLINE void *
ACE_Thread_Control::status (void *s)
{
  ACE_TRACE ("ACE_Thread_Control::status");
  return this->status_ = s;
}

// Get the exit status.

ACE_INLINE void *
ACE_Thread_Control::status (void)
{
  ACE_TRACE ("ACE_Thread_Control::status");
  return this->status_;
}

// Returns the current <Thread_Manager>.

ACE_INLINE ACE_Thread_Manager *
ACE_Thread_Control::thr_mgr (void)
{
  ACE_TRACE ("ACE_Thread_Control::thr_mgr");
  return this->tm_;
}

// Atomically set a new <Thread_Manager> and return the old
// <Thread_Manager>.

ACE_INLINE ACE_Thread_Manager *
ACE_Thread_Control::thr_mgr (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Control::thr_mgr");
  ACE_Thread_Manager *o_tm = this->tm_;
  return this->tm_ = tm;
  return o_tm;
}
