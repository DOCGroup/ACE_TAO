// Synch.cpp
// $Id$

#ifndef ACE_SYNCH_C
#define ACE_SYNCH_C

#define ACE_BUILD_DLL
#include "ace/Thread.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_T.h"
#include "ace/Synch.h"
#include "ace/Object_Manager.h"

ACE_RCSID(ace, Synch, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Synch.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Null_Mutex)
ACE_ALLOC_HOOK_DEFINE(ACE_File_Lock)
ACE_ALLOC_HOOK_DEFINE(ACE_RW_Process_Mutex)
ACE_ALLOC_HOOK_DEFINE(ACE_Process_Mutex)

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

void
ACE_Process_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Process_Mutex::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if !defined (ACE_WIN32) && !defined (ACE_HAS_POSIX_SEM)
LPCTSTR
ACE_Process_Mutex::unique_name (void)
{
  // For all platforms other than Win32, we are going to create a
  // machine wide unquie name if one is not provided by the user.  On
  // Win32, unnamed synchronization objects are acceptable.
  ACE::unique_name (this, this->name_, ACE_UNIQUE_NAME_LEN);
  return this->name_;
}
#endif /* !ACE_WIN32 && !ACE_HAS_POSIX_SEM */

ACE_Process_Mutex::ACE_Process_Mutex (LPCTSTR name, void *arg)
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  : lock_ (USYNC_PROCESS, name, arg)
#else
  : lock_ (name?name:ACE_Process_Mutex::unique_name ())
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM */
{
#if !defined (ACE_WIN32) && !defined (ACE_HAS_POSIX_SEM)
  ACE_UNUSED_ARG (arg);
#endif /* !ACE_WIN32 && !ACE_HAS_POSIX_SEM */
}

ACE_Process_Mutex::~ACE_Process_Mutex (void)
{
}

ACE_RW_Process_Mutex::ACE_RW_Process_Mutex (LPCTSTR name,
                                            int flags)
  : lock_ (name, flags
#if defined (ACE_WIN32)
           )
#else
           , S_IRUSR | S_IWUSR)
#endif /* ACE_WIN32 */
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
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_RW_Mutex::ACE_RW_Mutex (int type, LPCTSTR name, void *arg)
  : removed_ (0)
{
// ACE_TRACE ("ACE_RW_Mutex::ACE_RW_Mutex");
  if (ACE_OS::rwlock_init (&this->lock_, type, name, arg) != 0)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT("%p\n"), ASYS_TEXT("ACE_RW_Mutex::ACE_RW_Mutex")));
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
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Semaphore::ACE_Semaphore (u_int count,
                              int type,
                              LPCTSTR name,
                              void *arg,
                              int max)
  : removed_ (0)
{
// ACE_TRACE ("ACE_Semaphore::ACE_Semaphore");
  if (ACE_OS::sema_init (&this->semaphore_, count, type,
                         name, arg, max) != 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT("%p\n"),
                ASYS_TEXT("ACE_Semaphore::ACE_Semaphore")));
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
  : removed_ (0)
{
// ACE_TRACE ("ACE_File_Lock::ACE_File_Lock");
  if (ACE_OS::flock_init (&this->lock_) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_File_Lock::ACE_File_Lock")));
  this->set_handle (h);
}

ACE_File_Lock::ACE_File_Lock (LPCTSTR name,
                              int flags,
                              mode_t perms)
  : removed_ (0)
{
// ACE_TRACE ("ACE_File_Lock::ACE_File_Lock");

  if (this->open (name, flags, perms) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p %s\n"),
                ASYS_TEXT ("ACE_File_Lock::ACE_File_Lock"),
                name));
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
  //    ACE_ERROR ((LM_ERROR, ASYS_TEXT("%p\n"), ASYS_TEXT("ACE_Process_Mutex::~ACE_Process_Mutex")));
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
#if defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("lockname_ = %s\n"), this->lockname_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("process_lock_ = %x\n"), this->process_lock_));
#endif /* CHORUS */
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Mutex::ACE_Mutex (int type, LPCTSTR name, void *arg)
  :
#if defined (CHORUS)
    process_lock_ (0),
    lockname_ (0),
#endif /* CHORUS */
    removed_ (0)
{
  // ACE_TRACE ("ACE_Mutex::ACE_Mutex");

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
#endif /* CHORUS */
  if (ACE_OS::mutex_init (&this->lock_,
                          type,
                          name,
                          arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Mutex::ACE_Mutex")));
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
  : removed_ (0)
{
  if (ACE_OS::event_init (&this->handle_,
                          manual_reset,
                          initial_state,
                          type,
                          name,
                          arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT("%p\n"),
                ASYS_TEXT("ACE_Event::ACE_Event")));
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

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
void
ACE_Thread_Mutex_Guard::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex (LPCTSTR name,
                                                        void *arg)
  : removed_ (0)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex");
#if defined (ACE_HAS_FSU_PTHREADS) && ! defined (ACE_WIN32)
  // Initialize FSU pthreads package.  If called more than once,
  // pthread_init does nothing and so does no harm.
   pthread_init ();
#endif  /*  ACE_HAS_FSU_PTHREADS && ! ACE_WIN32 */
   if (ACE_OS::recursive_mutex_init (&this->recursive_mutex_,
                                     name,
                                     arg) == -1)
     ACE_ERROR ((LM_ERROR,
                 ASYS_TEXT ("%p\n"),
                 ASYS_TEXT ("recursive_mutex_init")));
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
      result = ACE_OS::recursive_mutex_destroy (&this->recursive_mutex_);
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
  ACE_OS::mutex_lock (&this->recursive_mutex_.nesting_mutex_);
  owner_id = this->recursive_mutex_.owner_id_;
  ACE_OS::mutex_unlock (&this->recursive_mutex_.nesting_mutex_);
  return owner_id;
#endif /* ACE_WIN32 */
}

int
ACE_Recursive_Thread_Mutex::get_nesting_level (void)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::get_nesting_level");
#if defined (ACE_HAS_WINCE) || defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_HAS_RECURSIVE_MUTEXES)
  // This is really a Win32-ism...
  return this->recursive_mutex_.RecursionCount;
#else
  int nesting_level = 0;
  ACE_OS::mutex_lock (&this->recursive_mutex_.nesting_mutex_);
  nesting_level = this->recursive_mutex_.nesting_level_;
  ACE_OS::mutex_unlock (&this->recursive_mutex_.nesting_mutex_);
  return nesting_level;
#endif /* !ACE_HAS_WINCE */
}

ACE_Recursive_Thread_Mutex::ACE_Recursive_Thread_Mutex (const ACE_Recursive_Thread_Mutex &)
{
}

int
ACE_Recursive_Thread_Mutex::acquire (void)
{
  return ACE_OS::recursive_mutex_lock (&this->recursive_mutex_);
}

int
ACE_Recursive_Thread_Mutex::release (void)
{
  return ACE_OS::recursive_mutex_unlock (&this->recursive_mutex_);
}

int
ACE_Recursive_Thread_Mutex::tryacquire (void)
{
  return ACE_OS::recursive_mutex_trylock (&this->recursive_mutex_);
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
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("waiters = %d\n"),
              this->cond_.waiters ()));
#endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
                                                        LPCTSTR name,
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
  if (ACE_OS::cond_init (&this->cond_, USYNC_THREAD, name, arg) != 0)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Condition_Thread_Mutex::ACE_Condition_Thread_Mutex")));
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
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("running_threads_ = %d"), this->running_threads_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
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
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("current_generation_ = %d"), this->current_generation_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\ncount_ = %d"), this->count_));
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
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Thread_Mutex::~ACE_Thread_Mutex (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::~ACE_Thread_Mutex");
  this->remove ();
}

ACE_Thread_Mutex::ACE_Thread_Mutex (LPCTSTR name, void *arg)
  : removed_ (0)
{
//  ACE_TRACE ("ACE_Thread_Mutex::ACE_Thread_Mutex");

  if (ACE_OS::thread_mutex_init (&this->lock_, USYNC_THREAD, name, arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT("%p\n"),
                ASYS_TEXT("ACE_Thread_Mutex::ACE_Thread_Mutex")));
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
template class ACE_Guard<ACE_Thread_Mutex>;
template class ACE_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Read_Guard<ACE_RW_Thread_Mutex>;
template class ACE_Write_Guard<ACE_RW_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// These are only specialized with ACE_HAS_THREADS.
#pragma instantiate ACE_Guard<ACE_Thread_Mutex>
#pragma instantiate ACE_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_RW_Thread_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

// These are specialized both with and without ACE_HAS_THREADS.
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Guard<ACE_Process_Mutex>;

// template class ACE_Guard<ACE_Null_Mutex>;
// template class ACE_Read_Guard<ACE_Null_Mutex>;
// template class ACE_Write_Guard<ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Guard<ACE_Process_Mutex>

// #pragma instantiate ACE_Guard<ACE_Null_Mutex>
// #pragma instantiate ACE_Read_Guard<ACE_Null_Mutex>
// #pragma instantiate ACE_Write_Guard<ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_SYNCH_C */
