// Synch.cpp
// $Id$

#if !defined (ACE_SYNCH_C)
#define ACE_SYNCH_C

#define ACE_BUILD_DLL
#include "ace/Thread.h"
#include "ace/Synch_T.h"
#include "ace/Synch.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Synch.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Null_Mutex)
ACE_ALLOC_HOOK_DEFINE(ACE_File_Lock)
ACE_ALLOC_HOOK_DEFINE(ACE_RW_Process_Mutex)
ACE_ALLOC_HOOK_DEFINE(ACE_Process_Mutex)

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

void
ACE_Process_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Process_Mutex::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_->dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Process_Mutex::ACE_Process_Mutex (LPCTSTR name, void *arg)
{
#if !defined (ACE_WIN32)
  // For all platforms other than Win32, we are going to create a
  // machine wide unquie name if one is not provided by the user.  On
  // Win32, unnamed synchronization objects are acceptable.
  TCHAR ace_name[ACE_UNIQUE_NAME_LEN];
  if (name == 0)
    {
      ACE::unique_name (this, ace_name, ACE_UNIQUE_NAME_LEN);
      name = ace_name;
    }
#endif
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  ACE_NEW (this->lock_, ACE_Mutex (USYNC_PROCESS, name, arg));
#else
  ACE_UNUSED_ARG (arg);
  ACE_NEW (this->lock_, ACE_SV_Semaphore_Complex (name));
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM */
}

ACE_Process_Mutex::~ACE_Process_Mutex (void)
{
  delete this->lock_;
}

ACE_RW_Process_Mutex::ACE_RW_Process_Mutex (LPCTSTR name,
                                            int flags)
  : lock_ (name, flags)
{
// ACE_TRACE ("ACE_RW_Process_Mutex::ACE_RW_Process_Mutex");
}

ACE_RW_Process_Mutex::~ACE_RW_Process_Mutex (void)
{
// ACE_TRACE ("ACE_RW_Process_Mutex::ACE_RW_Process_Mutex");
}

void
ACE_RW_Process_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_RW_Process_Mutex::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_RW_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_RW_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_RW_Mutex::ACE_RW_Mutex (int type, LPCTSTR name, void *arg)
{
// ACE_TRACE ("ACE_RW_Mutex::ACE_RW_Mutex");
  if (ACE_OS::rwlock_init (&this->lock_, type, name, arg) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_RW_Mutex::ACE_RW_Mutex"));
}

ACE_RW_Mutex::~ACE_RW_Mutex (void)
{
// ACE_TRACE ("ACE_RW_Mutex::~ACE_RW_Mutex");
  this->remove ();
}

ACE_ALLOC_HOOK_DEFINE(ACE_Semaphore)

void
ACE_Semaphore::dump (void) const
{
// ACE_TRACE ("ACE_Semaphore::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Semaphore::ACE_Semaphore (u_int count,
                              int type,
                              LPCTSTR name,
                              void *arg,
                              int max)
{
// ACE_TRACE ("ACE_Semaphore::ACE_Semaphore");
  if (ACE_OS::sema_init (&this->semaphore_, count, type,
                         name, arg, max) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Semaphore::ACE_Semaphore"));
}

ACE_Semaphore::~ACE_Semaphore (void)
{
// ACE_TRACE ("ACE_Semaphore::~ACE_Semaphore");
  this->remove ();
}

void
ACE_File_Lock::dump (void) const
{
// ACE_TRACE ("ACE_File_Lock::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_File_Lock::ACE_File_Lock (ACE_HANDLE h)
{
// ACE_TRACE ("ACE_File_Lock::ACE_File_Lock");
  if (ACE_OS::flock_init (&this->lock_) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_File_Lock::ACE_File_Lock"));
  this->set_handle (h);
}

ACE_File_Lock::ACE_File_Lock (LPCTSTR name,
                              int flags,
                              mode_t perms)
{
// ACE_TRACE ("ACE_File_Lock::ACE_File_Lock");

  if (this->open (name, flags, perms) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_File_Lock::ACE_File_Lock"));
}

int
ACE_File_Lock::open (LPCTSTR name,
                     int flags,
                     mode_t perms)
{
// ACE_TRACE ("ACE_File_Lock::open");

  return ACE_OS::flock_init (&this->lock_, flags, name, perms);
}

ACE_File_Lock::~ACE_File_Lock (void)
{
// ACE_TRACE ("ACE_File_Lock::~ACE_File_Lock");
  this->remove ();
}

void
ACE_Process_Semaphore::dump (void) const
{
// ACE_TRACE ("ACE_Process_Semaphore::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Process_Semaphore::ACE_Process_Semaphore (u_int count,
                                              LPCTSTR name,
                                              void *arg,
                                              int max)
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  : lock_ (count, USYNC_PROCESS, name, arg, max)
#else
  : lock_ (name, ACE_SV_Semaphore_Complex::ACE_CREATE, count)
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM */
{
  arg = arg;
  max = max;
// ACE_TRACE ("ACE_Process_Semaphore::ACE_Process_Semaphore");
}

ACE_Process_Semaphore::~ACE_Process_Semaphore (void)
{
// ACE_TRACE ("ACE_Process_Semaphore::~ACE_Process_Semaphore");
  //  if (this->remove () == -1)
  //    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Process_Mutex::~ACE_Process_Mutex"));
}

// Explicitly destroy the semaphore.

int
ACE_Process_Semaphore::remove (void)
{
// ACE_TRACE ("ACE_Process_Semaphore::remove");
  return this->lock_.remove ();
}

// Block the thread until the semaphore count becomes
// greater than 0, then decrement it.

int
ACE_Process_Semaphore::acquire (void)
{
// ACE_TRACE ("ACE_Process_Semaphore::acquire");
  return this->lock_.acquire ();
}

// Conditionally decrement the semaphore if count is greater
// than 0 (i.e., won't block).

int
ACE_Process_Semaphore::tryacquire (void)
{
// ACE_TRACE ("ACE_Process_Semaphore::tryacquire");
  return this->lock_.tryacquire ();
}

// Increment the semaphore, potentially unblocking
// a waiting thread.

int
ACE_Process_Semaphore::release (void)
{
// ACE_TRACE ("ACE_Process_Semaphore::release");
  return this->lock_.release ();
}

ACE_ALLOC_HOOK_DEFINE(ACE_Mutex)

void
ACE_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Mutex::ACE_Mutex (int type, LPCTSTR name, void *arg)
{
// ACE_TRACE ("ACE_Mutex::ACE_Mutex");

  if (ACE_OS::mutex_init (&this->lock_, type, name, arg) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Mutex::ACE_Mutex"));
}

ACE_Mutex::~ACE_Mutex (void)
{
// ACE_TRACE ("ACE_Mutex::~ACE_Mutex");
  this->remove ();
}

ACE_Event::ACE_Event (int manual_reset,
                      int initial_state,
                      int type,
                      LPCTSTR name,
                      void *arg)
{
  if (ACE_OS::event_init (&this->handle_,
                          manual_reset,
                          initial_state,
                          type,
                          name,
                          arg) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Event::ACE_Event"));
}

ACE_Event::~ACE_Event (void)
{
  this->remove ();
}

int
ACE_Event::remove (void)
{
  return ACE_OS::event_destroy (&this->handle_);
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
ACE_Event::wait (const ACE_Time_Value *abstime)
{
  return ACE_OS::event_timedwait (&this->handle_,
                                  (ACE_Time_Value *) abstime);
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
                                    LPCTSTR name,
                                    void *arg)
  : ACE_Event (1,
               initial_state,
               type,
               name,
               arg)
{
}

void
ACE_Manual_Event::dump (void) const
{
  ACE_Event::dump ();
}

ACE_Auto_Event::ACE_Auto_Event (int initial_state,
                                int type,
                                LPCTSTR name,
                                void *arg)
  : ACE_Event (0,
               initial_state,
               type,
               name,
               arg)
{
}

void
ACE_Auto_Event::dump (void) const
{
  ACE_Event::dump ();
}

#if defined (ACE_HAS_THREADS)

ACE_ALLOC_HOOK_DEFINE(ACE_Recursive_Thread_Mutex)
ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Mutex_Guard)

void
ACE_Thread_Semaphore::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Semaphore::dump");

  ACE_Semaphore::dump ();
}

ACE_Thread_Semaphore::ACE_Thread_Semaphore (u_int count,
                                            LPCTSTR name,
                                            void *arg,
                                            int max)
  : ACE_Semaphore (count, USYNC_THREAD, name, arg, max)
{
// ACE_TRACE ("ACE_Thread_Semaphore::ACE_Thread_Semaphore");
}

void
ACE_Thread_Mutex_Guard::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_thread_t
ACE_Recursive_Thread_Mutex::get_thread_id (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::get_thread_id");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->nesting_mutex_, ACE_OS::NULL_thread);
  return this->owner_id_;
}

int
ACE_Recursive_Thread_Mutex::get_nesting_level (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::get_nesting_level");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->nesting_mutex_, -1);
  return this->nesting_level_;
}

ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex (const ACE_Recursive_Thread_Mutex &rm)
  : lock_available_ ((ACE_Thread_Mutex &) rm.nesting_mutex_)
{
}

ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex (LPCTSTR name,
                                                        void *arg)
  : nesting_mutex_ (name, arg),
    lock_available_ (nesting_mutex_, name, arg),
    nesting_level_ (0),
    owner_id_ (ACE_OS::NULL_thread)
{
#if defined (ACE_HAS_FSU_PTHREADS)
//      Initialize FSU pthreads package.
//      If called more than once, pthread_init does nothing
//      and so does no harm.
   pthread_init ();
#endif  /*  ACE_HAS_FSU_PTHREADS */
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex");
}

ACE_Recursive_Thread_Mutex::~ACE_Recursive_Thread_Mutex (void)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::~ACE_Recursive_Thread_Mutex");
}

int
ACE_Recursive_Thread_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::acquire");
  ACE_thread_t t_id = ACE_Thread::self ();

  // Acquire the guard.
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->nesting_mutex_, -1);

  // If there's no contention, just grab the lock immediately (since
  // this is the common case we'll optimize for it).
  if (this->nesting_level_ == 0)
    this->set_thread_id (t_id);
  // If we already own the lock, then increment the nesting level and
  // return.
  else if (ACE_OS::thr_equal (t_id, this->owner_id_) == 0)
    {
      // Wait until the nesting level has dropped to zero, at which
      // point we can acquire the lock.
      while (this->nesting_level_ > 0)
        this->lock_available_.wait ();

      // Note that at this point the nesting_mutex_ is held...
      this->set_thread_id (t_id);
    }

  // At this point, we can safely increment the nesting_level_ no
  // matter how we got here!
  this->nesting_level_++;
  return 0;
}

int
ACE_Recursive_Thread_Mutex::release (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::release");
  ACE_thread_t t_id = ACE_Thread::self ();

  // Automatically acquire mutex.
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->nesting_mutex_, -1);

#if !defined (ACE_NDEBUG)
  if (this->nesting_level_ == 0
      || ACE_OS::thr_equal (t_id, this->owner_id_) == 0)
    {
      errno = EINVAL;
      ACE_RETURN (-1);
    }
#endif /* ACE_NDEBUG */

  this->nesting_level_--;
  if (this->nesting_level_ == 0)
    {
      // This may not be strictly necessary, but it does put the mutex
      // into a known state...
      this->set_thread_id (ACE_OS::NULL_thread);

      // Inform waiters that the lock is free.
      this->lock_available_.signal ();
    }
  return 0;
}

int
ACE_Recursive_Thread_Mutex::tryacquire (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::tryacquire");
  ACE_thread_t t_id = ACE_Thread::self ();

  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->nesting_mutex_, -1);

  // If there's no contention, just grab the lock immediately.
  if (this->nesting_level_ == 0)
    {
      this->set_thread_id (t_id);
      this->nesting_level_ = 1;
    }
  // If we already own the lock, then increment the nesting level and
  // proceed.
  else if (ACE_OS::thr_equal (t_id, this->owner_id_))
    this->nesting_level_++;
  else
    {
      errno = EBUSY;
      ACE_RETURN (-1);
    }
  return 0;
}

void
ACE_Recursive_Thread_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_available_.dump ();
  this->nesting_mutex_.dump ();
  ACE_DEBUG ((LM_DEBUG, "nesting_level_ = %d", this->nesting_level_));
#if !defined (ACE_HAS_DCETHREADS) && !defined (ACE_HAS_PTHREADS)
  ACE_DEBUG ((LM_DEBUG, "\nowner_id_ = %u", this->owner_id_));
#else
  ACE_DEBUG ((LM_DEBUG, "\n"));
#endif /* !ACE_HAS_DCETHREADS */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Condition_Thread_Mutex)

void
ACE_Condition_Thread_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG,
              "waiters = %d\n",
              this->cond_.waiters ()));
#endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
                                                        LPCTSTR name,
                                                        void *arg)
  : mutex_ ((ACE_Thread_Mutex &) m)
{
#if defined (ACE_HAS_FSU_PTHREADS)
//      Initialize FSU pthreads package.
//      If called more than once, pthread_init does nothing
//      and so does no harm.
   pthread_init ();
#endif  /*  ACE_HAS_FSU_PTHREADS */

// ACE_TRACE ("ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex");
  if (ACE_OS::cond_init (&this->cond_, USYNC_THREAD, name, arg) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n",
                "ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex"));
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
// ACE_TRACE ("ACE_Condition<MUTEX>::wait");
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
  ACE_DEBUG ((LM_DEBUG, "running_threads_ = %d", this->running_threads_));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Sub_Barrier::ACE_Sub_Barrier (u_int count,
                                  ACE_Thread_Mutex &lock,
                                  LPCTSTR name,
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
  ACE_DEBUG ((LM_DEBUG, "current_generation_ = %d", this->current_generation_));
  ACE_DEBUG ((LM_DEBUG, "\ncount_ = %d", this->count_));
  this->sub_barrier_1_.dump ();
  this->sub_barrier_2_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Barrier::ACE_Barrier (u_int count,
                          LPCTSTR name,
                          void *arg)
  : lock_ (name, arg),
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

ACE_Thread_Barrier::ACE_Thread_Barrier (u_int count, LPCTSTR name)
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
ACE_Process_Barrier::ACE_Process_Barrier (u_int count, LPCTSTR name)
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
                                                     LPCTSTR name,
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
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Thread_Mutex::~ACE_Thread_Mutex (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::~ACE_Thread_Mutex");
  this->remove ();
}

ACE_Thread_Mutex::ACE_Thread_Mutex (LPCTSTR name, void *arg)
{
//  ACE_TRACE ("ACE_Thread_Mutex::ACE_Thread_Mutex");

  if (ACE_OS::thread_mutex_init (&this->lock_, USYNC_THREAD, name, arg) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Thread_Mutex::ACE_Thread_Mutex"));
}

ACE_ALLOC_HOOK_DEFINE(ACE_RW_Thread_Mutex)

ACE_RW_Thread_Mutex::ACE_RW_Thread_Mutex (LPCTSTR name,
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// These are only specialized with ACE_HAS_THREADS.
template class ACE_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Read_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Write_Guard<ACE_RW_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// These are only specialized with ACE_HAS_THREADS.
#pragma instantiate ACE_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// These are specialized both with and without ACE_HAS_THREADS.
template class ACE_Guard<ACE_Process_Mutex>;
template class ACE_Guard<ACE_Null_Mutex>;
template class ACE_Read_Guard<ACE_Null_Mutex>;
template class ACE_Write_Guard<ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// These are specialized both with and without ACE_HAS_THREADS.
#pragma instantiate ACE_Guard<ACE_Process_Mutex>
#pragma instantiate ACE_Guard<ACE_Null_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_Null_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_SYNCH_C */
