/* -*- C++ -*- */
/**
 * @file Thread_Semaphore.cpp
 *
 * $Id$
 *
 * Originally in Synch.cpp
 *
 * @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */

#include "ace/Semaphore.h"

#if defined (ACE_HAS_THREADS)

#if !defined (__ACE_INLINE__)
#include "ace/Thread_Semaphore.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Thread_Semaphore, "$Id$")


void
ACE_Thread_Semaphore::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Thread_Semaphore::dump");

  ACE_Semaphore::dump ();
#endif /* ACE_HAS_DUMP */
}

ACE_Thread_Semaphore::ACE_Thread_Semaphore (u_int count,
                                            const ACE_TCHAR *name,
                                            void *arg,
                                            int max)
  : ACE_Semaphore (count, USYNC_THREAD, name, arg, max)
{
// ACE_TRACE ("ACE_Thread_Semaphore::ACE_Thread_Semaphore");
}
#endif /* ACE_HAS_THREADS */
