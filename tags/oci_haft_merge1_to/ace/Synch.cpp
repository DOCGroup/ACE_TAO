#ifndef ACE_SYNCH_C
#define ACE_SYNCH_C

#include "ace/Thread.h"
#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Log_Msg.h"


ACE_RCSID (ace,
           Synch,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "ace/Synch.i"
#endif /* __ACE_INLINE__ */


ACE_ALLOC_HOOK_DEFINE(ACE_Null_Mutex)

ACE_Lock::~ACE_Lock (void)
{
}

ACE_Adaptive_Lock::ACE_Adaptive_Lock (void)
  : lock_ (0)
{
}

ACE_Adaptive_Lock::~ACE_Adaptive_Lock (void)
{
}

int
ACE_Adaptive_Lock::remove (void)
{
  return this->lock_->remove ();
}

int
ACE_Adaptive_Lock::acquire (void)
{
  return this->lock_->acquire ();
}

int
ACE_Adaptive_Lock::tryacquire (void)
{
  return this->lock_->tryacquire ();
}

int
ACE_Adaptive_Lock::release (void)
{
  return this->lock_->release ();
}

int
ACE_Adaptive_Lock::acquire_read (void)
{
  return this->lock_->acquire_read ();
}

int
ACE_Adaptive_Lock::acquire_write (void)
{
  return this->lock_->acquire_write ();
}

int
ACE_Adaptive_Lock::tryacquire_read (void)
{
  return this->lock_->tryacquire_read ();
}

int
ACE_Adaptive_Lock::tryacquire_write (void)
{
  return this->lock_->tryacquire_write ();
}

int
ACE_Adaptive_Lock::tryacquire_write_upgrade (void)
{
  return this->lock_->tryacquire_write_upgrade ();
}

void
ACE_Adaptive_Lock::dump (void) const
{
  //  return this->lock_->dump ();
}

ACE_TSS_Adapter::ACE_TSS_Adapter (void *object, ACE_THR_DEST f)
  : ts_obj_ (object),
    func_ (f)
{
  // ACE_TRACE ("ACE_TSS_Adapter::ACE_TSS_Adapter");
}

void
ACE_TSS_Adapter::cleanup (void)
{
  // ACE_TRACE ("ACE_TSS_Adapter::cleanup");
  (*this->func_)(this->ts_obj_);  // call cleanup routine for ts_obj_
}

extern "C" void
ACE_TSS_C_cleanup (void *object)
{
  // ACE_TRACE ("ACE_TSS_C_cleanup");
  if (object != 0)
    {
      ACE_TSS_Adapter *tss_adapter = (ACE_TSS_Adapter *) object;
      // Perform cleanup on the real TS object.
      tss_adapter->cleanup ();
      // Delete the adapter object.
      delete tss_adapter;
    }
}

ACE_ALLOC_HOOK_DEFINE(ACE_Semaphore)

void
ACE_Semaphore::dump (void) const
{
// ACE_TRACE ("ACE_Semaphore::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Semaphore::ACE_Semaphore (u_int count,
                              int type,
                              const ACE_TCHAR *name,
                              void *arg,
                              int max)
  : removed_ (0)
{
// ACE_TRACE ("ACE_Semaphore::ACE_Semaphore");
#if defined(ACE_LACKS_UNNAMED_SEMAPHORE)
// if the user does not provide a name, we generate a unique name here
  ACE_TCHAR iname[ACE_UNIQUE_NAME_LEN];
  if (name == 0) 
    ACE::unique_name (this, iname, ACE_UNIQUE_NAME_LEN);
  if (ACE_OS::sema_init (&this->semaphore_, count, type,
                         name ? name : iname, 
                         arg, max) != 0)
#else
  if (ACE_OS::sema_init (&this->semaphore_, count, type,
                         name, arg, max) != 0)
#endif
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Semaphore::ACE_Semaphore")));
}

ACE_Semaphore::~ACE_Semaphore (void)
{
// ACE_TRACE ("ACE_Semaphore::~ACE_Semaphore");

  this->remove ();
}

ACE_ALLOC_HOOK_DEFINE(ACE_Mutex)

void
ACE_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
#if defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("lockname_ = %s\n"), this->lockname_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("process_lock_ = %x\n"), this->process_lock_));
#endif /* CHORUS */
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Mutex::ACE_Mutex (int type, const ACE_TCHAR *name,
                      ACE_mutexattr_t *arg, mode_t mode)
  :
#if defined (CHORUS) || defined (ACE_HAS_PTHREADS) || defined(ACE_HAS_STHREADS)
    process_lock_ (0),
    lockname_ (0),
#endif /* CHORUS */
    removed_ (0)
{
  // ACE_TRACE ("ACE_Mutex::ACE_Mutex");

  // These platforms need process-wide mutex to be in shared memory.
#if defined (CHORUS) || defined(ACE_HAS_PTHREADS) || defined (ACE_HAS_STHREADS)
  if (type == USYNC_PROCESS)
    {
      // Let's see if the shared memory entity already exists.
      ACE_HANDLE fd = ACE_OS::shm_open (name, O_RDWR | O_CREAT | O_EXCL, mode);
      if (fd == ACE_INVALID_HANDLE)
        {
          if (errno == EEXIST)
            fd = ACE_OS::shm_open (name, O_RDWR | O_CREAT, mode);
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
          this->lockname_ = ACE_OS::strdup (name);
          if (this->lockname_ == 0)
            {
              ACE_OS::close (fd);
              return;
            }
        }

      this->process_lock_ =
        (ACE_mutex_t *) ACE_OS::mmap (0,
                                      sizeof (ACE_mutex_t),
                                      PROT_RDWR,
                                      MAP_SHARED,
                                      fd,
                                      0);
      ACE_OS::close (fd);
      if (this->process_lock_ == MAP_FAILED)
        return;

      if (this->lockname_
          && ACE_OS::mutex_init (this->process_lock_,
                                 type,
                                 name,
                                 arg) != 0)
        return;
    }
   // It is ok to fall through into the <mutex_init> below if the
   // USYNC_PROCESS flag is not enabled.
#else
  ACE_UNUSED_ARG (mode);
#endif /* CHORUS */

  if (ACE_OS::mutex_init (&this->lock_,
                          type,
                          name,
                          arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Mutex::ACE_Mutex")));
}

ACE_Mutex::~ACE_Mutex (void)
{
// ACE_TRACE ("ACE_Mutex::~ACE_Mutex");
  this->remove ();
}

ACE_Event::ACE_Event (int manual_reset,
                      int initial_state,
                      int type,
                      const ACE_TCHAR *name,
                      void *arg)
  : removed_ (0)
{
  if (ACE_OS::event_init (&this->handle_,
                          manual_reset,
                          initial_state,
                          type,
                          name,
                          arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Event::ACE_Event")));
}

ACE_Event::~ACE_Event (void)
{
  this->remove ();
}

int
ACE_Event::remove (void)
{
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::event_destroy (&this->handle_);
    }
  return result;
}

ACE_event_t
ACE_Event::handle (void) const
{
  return this->handle_;
}

void
ACE_Event::handle (ACE_event_t new_handle)
{
  this->handle_ = new_handle;
}

int
ACE_Event::wait (void)
{
  return ACE_OS::event_wait (&this->handle_);
}

int
ACE_Event::wait (const ACE_Time_Value *abstime, int use_absolute_time)
{
  return ACE_OS::event_timedwait (&this->handle_,
                                  (ACE_Time_Value *) abstime,
                                  use_absolute_time);
}

int
ACE_Event::signal (void)
{
  return ACE_OS::event_signal (&this->handle_);
}

int
ACE_Event::pulse (void)
{
  return ACE_OS::event_pulse (&this->handle_);
}

int
ACE_Event::reset (void)
{
  return ACE_OS::event_reset (&this->handle_);
}

void
ACE_Event::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Manual_Event::ACE_Manual_Event (int initial_state,
                                    int type,
                                    const char *name,
                                    void *arg)
  : ACE_Event (1,
               initial_state,
               type,
               ACE_TEXT_CHAR_TO_TCHAR (name),
               arg)
{
}

#if defined (ACE_HAS_WCHAR)
ACE_Manual_Event::ACE_Manual_Event (int initial_state,
                                    int type,
                                    const wchar_t *name,
                                    void *arg)
  : ACE_Event (1,
               initial_state,
               type,
               ACE_TEXT_WCHAR_TO_TCHAR (name),
               arg)
{
}
#endif /* ACE_HAS_WCHAR */

void
ACE_Manual_Event::dump (void) const
{
  ACE_Event::dump ();
}

ACE_Auto_Event::ACE_Auto_Event (int initial_state,
                                int type,
                                const char *name,
                                void *arg)
  : ACE_Event (0,
               initial_state,
               type,
               ACE_TEXT_CHAR_TO_TCHAR (name),
               arg)
{
}

#if defined (ACE_HAS_WCHAR)
ACE_Auto_Event::ACE_Auto_Event (int initial_state,
                                int type,
                                const wchar_t *name,
                                void *arg)
  : ACE_Event (0,
               initial_state,
               type,
               ACE_TEXT_WCHAR_TO_TCHAR (name),
               arg)
{
}
#endif /* ACE_HAS_WCHAR */

void
ACE_Auto_Event::dump (void) const
{
  ACE_Event::dump ();
}

#if defined (ACE_HAS_THREADS)

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Mutex_Guard)

void
ACE_Thread_Semaphore::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Semaphore::dump");

  ACE_Semaphore::dump ();
}

ACE_Thread_Semaphore::ACE_Thread_Semaphore (u_int count,
                                            const ACE_TCHAR *name,
                                            void *arg,
                                            int max)
  : ACE_Semaphore (count, USYNC_THREAD, name, arg, max)
{
// ACE_TRACE ("ACE_Thread_Semaphore::ACE_Thread_Semaphore");
}

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
void
ACE_Thread_Mutex_Guard::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

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

ACE_ALLOC_HOOK_DEFINE(ACE_Recursive_Thread_Mutex)

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
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Condition_Thread_Mutex)

void
ACE_Condition_Thread_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("waiters = %d\n"),
              this->cond_.waiters ()));
#endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
                                                        const ACE_TCHAR *name,
                                                        void *arg)
  : mutex_ ((ACE_Thread_Mutex &) m),
    removed_ (0)
{
#if defined (ACE_HAS_FSU_PTHREADS)
//      Initialize FSU pthreads package.
//      If called more than once, pthread_init does nothing
//      and so does no harm.
   pthread_init ();
#endif  /*  ACE_HAS_FSU_PTHREADS */

// ACE_TRACE ("ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex");
  if (ACE_OS::cond_init (&this->cond_,
                         (short) USYNC_THREAD,
                         name,
                         arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex")));
}

ACE_Condition_Thread_Mutex::
ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
                            ACE_Condition_Attributes &attributes,
                            const ACE_TCHAR *name,
                            void *arg)
  : mutex_ ((ACE_Thread_Mutex &) m),
    removed_ (0)
{
#if defined (ACE_HAS_FSU_PTHREADS)
//      Initialize FSU pthreads package.
//      If called more than once, pthread_init does nothing
//      and so does no harm.
   pthread_init ();
#endif  /*  ACE_HAS_FSU_PTHREADS */

// ACE_TRACE ("ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex");
  if (ACE_OS::cond_init (&this->cond_, attributes.attributes_,
                         name, arg) != 0)
    ACE_ERROR ((LM_ERROR, ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex")));
}

ACE_Condition_Thread_Mutex::~ACE_Condition_Thread_Mutex (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::~ACE_Condition_Thread_Mutex");
  this->remove ();
}

// Peform an "alertable" timed wait.  If the argument <abstime> == 0
// then we do a regular <cond_wait>, else we do a timed wait for up to
// <abstime> using the <cond_timedwait> function.

int
ACE_Condition_Thread_Mutex::wait (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::wait");
  return ACE_OS::cond_wait (&this->cond_, &this->mutex_.lock_);
}

int
ACE_Condition_Thread_Mutex::wait (ACE_Thread_Mutex &mutex,
                                  const ACE_Time_Value *abstime)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::wait");
  return ACE_OS::cond_timedwait (&this->cond_,
                                 &mutex.lock_,
                                 (ACE_Time_Value *) abstime);
}

int
ACE_Condition_Thread_Mutex::wait (const ACE_Time_Value *abstime)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::wait");
  return this->wait (this->mutex_, abstime);
}

int
ACE_Condition_Thread_Mutex::signal (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::signal");
  return ACE_OS::cond_signal (&this->cond_);
}

int
ACE_Condition_Thread_Mutex::broadcast (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::broadcast");
  return ACE_OS::cond_broadcast (&this->cond_);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Sub_Barrier)

void
ACE_Sub_Barrier::dump (void) const
{
// ACE_TRACE ("ACE_Sub_Barrier::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->barrier_finished_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("running_threads_ = %d"), this->running_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Sub_Barrier::ACE_Sub_Barrier (u_int count,
                                  ACE_Thread_Mutex &lock,
                                  const ACE_TCHAR *name,
                                  void *arg)
  : barrier_finished_ (lock, name, arg),
    running_threads_ (count)
{
// ACE_TRACE ("ACE_Sub_Barrier::ACE_Sub_Barrier");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Barrier)
ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Barrier)
ACE_ALLOC_HOOK_DEFINE(ACE_Process_Barrier)

void
ACE_Barrier::dump (void) const
{
// ACE_TRACE ("ACE_Barrier::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("current_generation_ = %d"), this->current_generation_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\ncount_ = %d"), this->count_));
  this->sub_barrier_1_.dump ();
  this->sub_barrier_2_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Barrier::ACE_Barrier (u_int count,
                          const ACE_TCHAR *name,
                          void *arg)
  : lock_ (name, (ACE_mutexattr_t *) arg),
    current_generation_ (0),
    count_ (count),
    sub_barrier_1_ (count, lock_, name, arg),
    sub_barrier_2_ (count, lock_, name, arg)
{
// ACE_TRACE ("ACE_Barrier::ACE_Barrier");
  this->sub_barrier_[0] = &this->sub_barrier_1_;
  this->sub_barrier_[1] = &this->sub_barrier_2_;
}

int
ACE_Barrier::wait (void)
{
// ACE_TRACE ("ACE_Barrier::wait");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  ACE_Sub_Barrier *sbp =
    this->sub_barrier_[this->current_generation_];

  // Check for shutdown...
  if (sbp == 0)
    return -1;

  if (sbp->running_threads_ == 1)
    {
      // We're the last running thread, so swap generations and tell
      // all the threads waiting on the barrier to continue on their
      // way.

      sbp->running_threads_ = this->count_;
      // Swap generations.
      this->current_generation_ = 1 - this->current_generation_;
      sbp->barrier_finished_.broadcast ();
    }
  else
    {
      --sbp->running_threads_;

      // Block until all the other threads wait().
      while (sbp->running_threads_ != this->count_)
        sbp->barrier_finished_.wait ();
    }

  return 0;
}

ACE_Thread_Barrier::ACE_Thread_Barrier (u_int count, const ACE_TCHAR *name)
  : ACE_Barrier (count, name)
{
// ACE_TRACE ("ACE_Thread_Barrier::ACE_Thread_Barrier");
}

void
ACE_Thread_Barrier::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Barrier::dump");
  ACE_Barrier::dump ();
}

#if 0
ACE_Process_Barrier::ACE_Process_Barrier (u_int count, const ACE_TCHAR *name)
  : ACE_Barrier (count, USYNC_PROCESS, name)
{
// ACE_TRACE ("ACE_Process_Barrier::ACE_Process_Barrier");
}

void
ACE_Process_Barrier::dump (void) const
{
// ACE_TRACE ("ACE_Process_Barrier::dump");
  ACE_Barrier::dump ();
}

template <class MUTEX> void
ACE_Process_Condition<MUTEX>::dump (void) const
{
// ACE_TRACE ("ACE_Process_Condition<MUTEX>::dump");

  ACE_Condition<MUTEX>::dump ();
}

template <class MUTEX>
ACE_Process_Condition<MUTEX>::ACE_Process_Condition (MUTEX &m,
                                                     const ACE_TCHAR *name,
                                                     void *arg)
  : ACE_Condition<MUTEX> (m, USYNC_PROCESS, name, arg)
{
// ACE_TRACE ("ACE_Process_Condition<MUTEX>::ACE_Process_Condition");
}
#endif /* 0 */

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Mutex)

void
ACE_Thread_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
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
                                 USYNC_THREAD,
                                 name,
                                 arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Thread_Mutex::ACE_Thread_Mutex")));
}

void
ACE_RW_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_RW_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_RW_Mutex::ACE_RW_Mutex (int type, const ACE_TCHAR *name, void *arg)
  : removed_ (0)
{
// ACE_TRACE ("ACE_RW_Mutex::ACE_RW_Mutex");
  if (ACE_OS::rwlock_init (&this->lock_, type, name, arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_RW_Mutex::ACE_RW_Mutex")));
}

ACE_RW_Mutex::~ACE_RW_Mutex (void)
{
// ACE_TRACE ("ACE_RW_Mutex::~ACE_RW_Mutex");
  this->remove ();
}

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
// ACE_TRACE ("ACE_RW_Thread_Mutex::dump");
  ACE_RW_Mutex::dump ();
}


//ACE_TEMPLATE_METHOD_SPECIALIZATION
int
ACE_Condition<ACE_Recursive_Thread_Mutex>::remove (void)
{
  return ACE_OS::cond_destroy (&this->cond_);
}

void
ACE_Condition<ACE_Recursive_Thread_Mutex>::dump (void) const
{
// ACE_TRACE ("ACE_Condition<MUTEX>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  // No dump method for ACE_cond_t even in emulated mode. 
  // cond_.dump ();
  this->mutex_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

//ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_Condition<ACE_Recursive_Thread_Mutex>::~ACE_Condition (void)
{
  this->remove ();
}

//ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_Condition<ACE_Recursive_Thread_Mutex>::ACE_Condition (ACE_Recursive_Thread_Mutex &m)
  : mutex_ (m)
{
  ACE_OS::cond_init (&this->cond_);
}

//ACE_TEMPLATE_METHOD_SPECIALIZATION
int
ACE_Condition<ACE_Recursive_Thread_Mutex>::wait (const ACE_Time_Value *abstime)
{
  return this->wait (this->mutex_, abstime);
}

//ACE_TEMPLATE_METHOD_SPECIALIZATION
int
ACE_Condition<ACE_Recursive_Thread_Mutex>::wait (ACE_Recursive_Thread_Mutex &mutex,
                                                 const ACE_Time_Value *abstime)
{
  ACE_recursive_mutex_state mutex_state_holder;
  ACE_recursive_thread_mutex_t &recursive_mutex = mutex.mutex ();

  if (ACE_OS::recursive_mutex_cond_unlock (&recursive_mutex,
                                           mutex_state_holder) == -1)
    return -1;

  // We wait on the condition, specifying the nesting mutex. For platforms
  // with ACE_HAS_RECURSIVE_MUTEXES, this is the recursive mutex itself,
  // and is the same as recursive_mutex, above. The caller should have been
  // holding the lock on entry to this method, and it is still held.
  // For other platforms, this is the nesting mutex that guards the
  // ACE_recursive_mutex_t internals, and recursive_mutex_cond_unlock()
  // returned with the lock held, but waiters primed and waiting to be
  // released. At cond_wait below, the mutex will be released.
  // On return, it will be reacquired.
  const int result = abstime == 0
    ? ACE_OS::cond_wait (&this->cond_,
                         &mutex.get_nesting_mutex ())
    : ACE_OS::cond_timedwait (&this->cond_,
                              &mutex.get_nesting_mutex (),
                              (ACE_Time_Value *) abstime);
  // We are holding the mutex, whether the wait succeeded or failed.
  // Stash errno (in case it failed) and then we need to reset the
  // recursive mutex state to what it was on entry to this method.
  // Resetting it may require a wait for another thread to release
  // the ACE_recursive_thread_mutex_t if this is a platform without
  // ACE_HAS_RECURSIVE_MUTEXES, and recursive_mutex_cond_relock() takes
  // care of that.
  {
    ACE_Errno_Guard error (errno);
    ACE_OS::recursive_mutex_cond_relock (&recursive_mutex,
                                         mutex_state_holder);
  }

  return result;
}

//ACE_TEMPLATE_METHOD_SPECIALIZATION
int
ACE_Condition<ACE_Recursive_Thread_Mutex>::signal (void)
{
  return ACE_OS::cond_signal (&this->cond_);
}

//ACE_TEMPLATE_METHOD_SPECIALIZATION
int
ACE_Condition<ACE_Recursive_Thread_Mutex>::broadcast (void)
{
  return ACE_OS::cond_broadcast (&this->cond_);
}

//ACE_TEMPLATE_METHOD_SPECIALIZATION
ACE_Recursive_Thread_Mutex &
ACE_Condition<ACE_Recursive_Thread_Mutex>::mutex (void)
{
  return this->mutex_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// These are only instantiated with ACE_HAS_THREADS.
template class ACE_Guard<ACE_Thread_Mutex>;
template class ACE_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Read_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Read_Guard<ACE_Thread_Mutex>;
template class ACE_Write_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Write_Guard<ACE_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// These are only instantiated with ACE_HAS_THREADS.
#pragma instantiate ACE_Guard<ACE_Thread_Mutex>
#pragma instantiate ACE_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_Thread_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */
#endif /* ACE_SYNCH_C */
