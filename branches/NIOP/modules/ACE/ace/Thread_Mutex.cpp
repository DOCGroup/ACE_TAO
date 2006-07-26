/**
 * @file Thread_Mutex.cpp
 *
 * $Id$
 *
 * Originally in Synch.cpp
 *
 * @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */

#include "ace/Thread_Mutex.h"

#if defined (ACE_HAS_THREADS)

#if !defined (__ACE_INLINE__)
#include "ace/Thread_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Msg.h"
#include "ace/Guard_T.h"
#include "ace/Malloc_T.h"

ACE_RCSID(ace, Thread_Mutex, "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Mutex_Guard)

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
void
ACE_Thread_Mutex_Guard::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Thread_Mutex_Guard::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */
ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Mutex)

void
ACE_Thread_Mutex::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Thread_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_Thread_Mutex::~ACE_Thread_Mutex (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::~ACE_Thread_Mutex");
  this->remove ();
}

ACE_Thread_Mutex::ACE_Thread_Mutex (const ACE_TCHAR *name, ACE_mutexattr_t *arg)
  : removed_ (0)
{
//  ACE_TRACE ("ACE_Thread_Mutex::ACE_Thread_Mutex");

  if (ACE_OS::thread_mutex_init (&this->lock_,
                                 0,
                                 name,
                                 arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Thread_Mutex::ACE_Thread_Mutex")));
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_THREADS */
