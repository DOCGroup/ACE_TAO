// $Id$

#ifndef ACE_CONDITION_T_C
#define ACE_CONDITION_T_C

#include "ace/Condition_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Condition_T, "$Id$")

#if defined (ACE_HAS_THREADS)

#if !defined (__ACE_INLINE__)
#include "ace/Condition_T.inl"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Condition)

template <class MUTEX> void
ACE_Condition<MUTEX>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Condition<MUTEX>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
#if defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("condname_ = %s\n"), this->condname_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("process_cond_ = %x\n"),
              this->process_cond_));
#endif /* CHORUS */
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

template <class MUTEX>
ACE_Thread_Condition<MUTEX>::ACE_Thread_Condition (MUTEX &m,
                                                   const ACE_TCHAR *name,
                                                   void *arg)
  : ACE_Condition<MUTEX> (m, USYNC_THREAD, name, arg)
{
// ACE_TRACE ("ACE_Thread_Condition<MUTEX>::ACE_Thread_Condition");
}

template <class MUTEX> void
ACE_Thread_Condition<MUTEX>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Thread_Condition<MUTEX>::dump");

  ACE_Condition<MUTEX>::dump ();
#endif /* ACE_HAS_DUMP */
}

template <class MUTEX>
ACE_Condition<MUTEX>::ACE_Condition (MUTEX &m,
                                     int type,
                                     const ACE_TCHAR *name,
                                     void *arg)
  :
#if defined (CHORUS)
    process_cond_(0),
    condname_ (0),
#endif /* CHORUS */
    mutex_ (m)
{

#if defined(CHORUS)
  if (type == USYNC_PROCESS)
    {
      // Let's see if the shared memory entity already exists.
      ACE_HANDLE fd = ACE_OS::shm_open (name,
                                        O_RDWR | O_CREAT | O_EXCL,
                                        ACE_DEFAULT_FILE_PERMS);
      if (fd == ACE_INVALID_HANDLE)
        {
          if (errno == EEXIST)
            fd = ACE_OS::shm_open (name,
                                   O_RDWR | O_CREAT,
                                   ACE_DEFAULT_FILE_PERMS);
          else
            return;
        }
      else
        {
          // We own this shared memory object!  Let's set its size.
          if (ACE_OS::ftruncate (fd,
                                 sizeof (ACE_mutex_t)) == -1)
            {
              ACE_OS::close (fd);
              return;
            }
          this->condname_ = ACE_OS::strdup (name);
          if (this->condname_ == 0)
            {
              ACE_OS::close (fd);
              return;
            }
        }

      this->process_cond_ =
        (ACE_cond_t *) ACE_OS::mmap (0,
                                     sizeof (ACE_cond_t),
                                     PROT_RDWR,
                                     MAP_SHARED,
                                     fd,
                                     0);
      ACE_OS::close (fd);
      if (this->process_cond_ == MAP_FAILED)
        return;

      if (this->condname_
          && ACE_OS::cond_init (this->process_cond_,
                                type,
                                name,
                                arg) != 0)
        return;
    }
   // It is ok to fall through into the <cond_init> below if the
   // USYNC_PROCESS flag is not enabled.
#endif /* CHORUS */

  // ACE_TRACE ("ACE_Condition<MUTEX>::ACE_Condition");

  if (ACE_OS::cond_init (&this->cond_,
                         (short) type,
                         name,
                         arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Condition::ACE_Condition")));
}

template <class MUTEX>
ACE_Condition<MUTEX>::~ACE_Condition (void)
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::~ACE_Condition");

  if (this->remove () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Condition::~ACE_Condition")));
}

template <class MUTEX> int
ACE_Condition<MUTEX>::wait (void)
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::wait");
#if defined (CHORUS)
  if (this->process_cond_ != 0)
    return ACE_OS::cond_wait (this->process_cond_,
                              &this->mutex_.lock_);
#endif /* CHORUS */
  return ACE_OS::cond_wait (&this->cond_,
                            &this->mutex_.lock_);
}

template <class MUTEX> int
ACE_Condition<MUTEX>::wait (MUTEX &mutex,
                            const ACE_Time_Value *abstime)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::wait");
  if (abstime == 0)
    return this->wait ();
  else
    {
#if defined (CHORUS)
      if (this->process_cond_ != 0)
        return ACE_OS::cond_timedwait (this->process_cond_,
                                      &mutex_.lock_,
                                      (ACE_Time_Value *) abstime);
#endif /* CHORUS */
      return ACE_OS::cond_timedwait (&this->cond_,
                                     &mutex.lock_,
                                     (ACE_Time_Value *) abstime);
    }
}

// Peform an "alertable" timed wait.  If the argument ABSTIME == 0
// then we do a regular cond_wait(), else we do a timed wait for up to
// ABSTIME using the Solaris cond_timedwait() function.

template <class MUTEX> int
ACE_Condition<MUTEX>::wait (const ACE_Time_Value *abstime)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::wait");
  return this->wait (this->mutex_, abstime);
}
#endif /* ACE_HAS_THREADS */

#endif /* ACE_CONDITION_T_C */
