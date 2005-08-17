/* -*- C++ -*- */
/**
 * @file RW_Thread_Mutex.cpp
 *
 * $Id$
 *
 * Originally in Synch.cpp
 *
 * @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */

#include "ace/RW_Thread_Mutex.h"

#if defined (ACE_HAS_THREADS)

#if !defined (__ACE_INLINE__)
#include "ace/RW_Thread_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Guard_T.h"

ACE_RCSID(ace, RW_Thread_Mutex, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_RW_Thread_Mutex)

ACE_RW_Thread_Mutex::ACE_RW_Thread_Mutex (const ACE_TCHAR *name,
                                          void *arg)
  : ACE_RW_Mutex (USYNC_THREAD, name, arg)
{
// ACE_TRACE ("ACE_RW_Thread_Mutex::ACE_RW_Thread_Mutex");
}

void
ACE_RW_Thread_Mutex::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_RW_Thread_Mutex::dump");
  ACE_RW_Mutex::dump ();
#endif /* ACE_HAS_DUMP */
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// These are only instantiated with ACE_HAS_THREADS.
template class ACE_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Read_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Write_Guard<ACE_RW_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// These are only instantiated with ACE_HAS_THREADS.
#pragma instantiate ACE_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */
