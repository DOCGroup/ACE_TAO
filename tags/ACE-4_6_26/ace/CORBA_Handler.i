/* -*- C++ -*- */
// $Id$

// CORBA_Handler.i

#if defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)
// = Set/get the number of iterations per processNextEvent() call.

/* static */ 
ACE_INLINE size_t 
ACE_ST_CORBA_Handler::iterations (void)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::iterations");
  return this->iterations_;
}

/* static */ 
ACE_INLINE void
ACE_ST_CORBA_Handler::iterations (size_t i)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::iterations");
  this->iterations_ = i;
}

#if defined (ACE_HAS_MT_ORBIX) && (ACE_HAS_MT_ORBIX != 0)
ACE_INLINE void
ACE_MT_CORBA_Handler::thr_mgr (ACE_Thread_Manager *tm) 
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::thr_mgr");
  this->thr_mgr_ = tm;
}

ACE_INLINE ACE_Thread_Manager *
ACE_MT_CORBA_Handler::thr_mgr (void) const
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::thr_mgr");
  return this->thr_mgr_;
}
#endif /* ACE_HAS_MT_ORBIX */
#endif /* ACE_HAS_ORBIX */
