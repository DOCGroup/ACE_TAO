/* -*- C++ -*- */
/**
 * @file Recursive_Thread_Mutex.cpp
 *
 * $Id$
 *
 * Originally in Synch.cpp
 *
 * @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */

#include "ace/Recursive_Thread_Mutex.h"

#if defined (ACE_HAS_THREADS)

#if !defined (__ACE_INLINE__)
#include "ace/Recursive_Thread_Mutex.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Recursive_Thread_Mutex, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Recursive_Thread_Mutex)

ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex (const ACE_TCHAR *name,
                                                        ACE_mutexattr_t *arg)
  : removed_ (0)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex");
#if defined (ACE_HAS_FSU_PTHREADS) && ! defined (ACE_WIN32)
  // Initialize FSU pthreads package.  If called more than once,
  // pthread_init does nothing and so does no harm.
   pthread_init ();
#endif  /*  ACE_HAS_FSU_PTHREADS && ! ACE_WIN32 */
   if (ACE_OS::recursive_mutex_init (&this->lock_,
                                     name,
                                     arg) == -1)
     ACE_ERROR ((LM_ERROR,
                 ACE_LIB_TEXT ("%p\n"),
                 ACE_LIB_TEXT ("recursive_mutex_init")));
}

ACE_Recursive_Thread_Mutex::~ACE_Recursive_Thread_Mutex (void)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::~ACE_Recursive_Thread_Mutex");
  this->remove ();
}

int
ACE_Recursive_Thread_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::remove");
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::recursive_mutex_destroy (&this->lock_);
    }
  return result;
}

// The counter part of the following two functions for Win32 are
// located in file Synch.i
ACE_thread_t
ACE_Recursive_Thread_Mutex::get_thread_id (void)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::get_thread_id");
#if defined (ACE_HAS_RECURSIVE_MUTEXES)
  // @@ The structure CriticalSection in Win32 doesn't hold the thread
  // handle of the thread that owns the lock.  However it is still not
  // clear at this point how to translate a thread handle to its
  // corresponding thread id.
  errno = ENOTSUP;
  return ACE_OS::NULL_thread;
#else
  ACE_thread_t owner_id;
  ACE_OS::mutex_lock (&this->lock_.nesting_mutex_);
  owner_id = this->lock_.owner_id_;
  ACE_OS::mutex_unlock (&this->lock_.nesting_mutex_);
  return owner_id;
#endif /* ACE_WIN32 */
}

int
ACE_Recursive_Thread_Mutex::get_nesting_level (void)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::get_nesting_level");
#if defined (ACE_HAS_WINCE) || defined (VXWORKS) || defined (ACE_PSOS)
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_HAS_RECURSIVE_MUTEXES)
  // Nothing inside of a CRITICAL_SECTION object should ever be
  // accessed directly.  It is documented to change at any time.
# if defined (ACE_WIN64)
  // Things are different on Windows XP 64-bit
  return this->lock_.LockCount + 1;
# elif defined (ACE_WIN32)
  // This is really a Win32-ism...
  return this->lock_.RecursionCount;
# else
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_RECURSIVE_MUTEXES */
#else
  int nesting_level = 0;
  ACE_OS::mutex_lock (&this->lock_.nesting_mutex_);
  nesting_level = this->lock_.nesting_level_;
  ACE_OS::mutex_unlock (&this->lock_.nesting_mutex_);
  return nesting_level;
#endif /* !ACE_HAS_WINCE */
}

ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex (const ACE_Recursive_Thread_Mutex &)
{
}

int
ACE_Recursive_Thread_Mutex::acquire (void)
{
  return ACE_OS::recursive_mutex_lock (&this->lock_);
}

int
ACE_Recursive_Thread_Mutex::release (void)
{
  return ACE_OS::recursive_mutex_unlock (&this->lock_);
}

int
ACE_Recursive_Thread_Mutex::tryacquire (void)
{
  return ACE_OS::recursive_mutex_trylock (&this->lock_);
}

void
ACE_Recursive_Thread_Mutex::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

#endif /* ACE_HAS_THREADS */
