/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Synch.h
//
// = DESCRIPTION
//     Wrappers for various synchronization routines.
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SYNCH_H)
#define ACE_SYNCH_H

#include "ace/ACE.h"
#if !(defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM))
#include "ace/SV_Semaphore_Complex.h"
#endif /* !(defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)) */

// Forward declarations.
class ACE_Time_Value;
// template <class ACE_COND_MUTEX> class ACE_Condition;

class ACE_Export ACE_File_Lock
  // = TITLE
  //     A wrapper around the UNIX file locking mechanism.
  //
  // = DESCRIPTION
  //     Allows us to "adapt" the UNIX file locking mechanisms to work
  //     with all of our Guard stuff...
{
public:
  ACE_File_Lock (ACE_HANDLE handle = ACE_INVALID_HANDLE);
  // Set the <handle_> of the File_Lock to <handle>.  Note that this
  // constructor assumes ownership of the <handle> and will close it
  // down in <remove>.  If you want the <handle> stays open when
  // <remove> is called make sure to call <dup> on the <handle> before
  // closing it.

  ACE_File_Lock (const char *filename, int flags, mode_t mode = 0);
  // Open the <filename> with <flags> and <mode> and set the result to
  // <handle_>.
  
  ~ACE_File_Lock (void);
  // Remove a File lock by releasing it and closing down the <handle_>.

  int remove (void);
  // Remove a File lock by releasing it and closing down the <handle_>.

  int acquire (short whence = 0, off_t start = 0, off_t len = 1);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <acquire> method.  This is implemented as
  // a write-lock to be on the safe-side...

  int tryacquire (short whence = 0, off_t start = 0, off_t len = 1);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <tryacquire> method.  This is implemented
  // as a write-lock to be on the safe-side...

  int release (short whence = 0, off_t start = 0, off_t len = 1);
  // Unlock a readers/writer lock.

  int acquire_write (short whence = 0, off_t start = 0, off_t len = 1);
  // Acquire a write lock, but block if any readers or a 
  // writer hold the lock.

  int tryacquire_write (short whence = 0, off_t start = 0, off_t len = 1);
  // Conditionally acquire a write lock (i.e., won't block).

  int acquire_read (short whence = 0, off_t start = 0, off_t len = 1);
  // Acquire a read lock, but block if a writer hold the lock.

  int tryacquire_read (short whence = 0, off_t start = 0, off_t len = 1);
  // Conditionally acquire a read lock (i.e., won't block).

  ACE_HANDLE get_handle (void);
  // Get underlying <ACE_HANDLE>.

  void set_handle (ACE_HANDLE);
  // Set underlying <ACE_HANDLE>.  Note that this method assumes
  // ownership of the <handle> and will close it down in <remove>.  If
  // you want the <handle> stays open when <remove> is called make
  // sure to call <dup> on the <handle> before closing it.

  void dump (void) const;
  // Dump state of the object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_OS::flock_t lock_;
  // Locking structure for OS record locks.

  // = Prevent assignment and initialization.
  void operator= (const ACE_File_Lock &) {}
  ACE_File_Lock (const ACE_File_Lock &) {}
};

class ACE_Export ACE_Semaphore
  // = TITLE
  //     Wrapper for Dijkstra style general semaphores.
{
public:
  // = Initialization and termination.
  ACE_Semaphore (u_int count, 
		 int type = USYNC_THREAD, 
		 LPCTSTR name = 0, 
		 void * = 0,
		 int max = 0x7fffffff);
  // Initialize the semaphore, with default value of "count".

  ~ACE_Semaphore (void);       
  // Implicitly destroy the semaphore.

  int remove (void);
  // Explicitly destroy the semaphore.

  int acquire (void);
  // Block the thread until the semaphore count becomes
  // greater than 0, then decrement it.

  int tryacquire (void);
  // Conditionally decrement the semaphore if count is greater 
  // than 0 (i.e., won't block).

  int release (void);
  // Increment the semaphore, potentially unblocking
  // a waiting thread.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  const ACE_sema_t &lock (void) const;
  // Return the underlying lock.

private:
  ACE_sema_t semaphore_;

  // = Prevent assignment and initialization.
  void operator= (const ACE_Semaphore &) {}
  ACE_Semaphore (const ACE_Semaphore &) {}
};

class ACE_Export ACE_Process_Semaphore
  // = TITLE
  //     Wrapper for Dijkstra style general semaphores that work
  //     across processes.
{
public:
  ACE_Process_Semaphore (u_int count, LPCTSTR name = 0, 
			 void * = 0, int max = 0x7FFFFFFF);
  // Initialize the semaphore, with an initial value of <count> and a
  // maximum value of <max>.

  ~ACE_Process_Semaphore (void);       
  // Implicitly destroy the semaphore.

  int remove (void);
  // Explicitly destroy the semaphore.

  int acquire (void);
  // Block the thread until the semaphore count becomes
  // greater than 0, then decrement it.

  int tryacquire (void);
  // Conditionally decrement the semaphore if count is greater 
  // than 0 (i.e., won't block).

  int release (void);
  // Increment the semaphore, potentially unblocking
  // a waiting thread.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  ACE_Semaphore lock_;
#else
  ACE_SV_Semaphore_Complex lock_;
  // We need this to get the right semantics...
#endif /* ACE_WIN32 */
};

#if defined (ACE_HAS_THREADS) // ACE platform supports some form of threading.

class ACE_Export ACE_Event
  // = TITLE
  //     A wrapper around the Win32 event locking mechanism.
  //
  // = DESCRIPTION
  //     Portable implementation of an Event mechanism, which is
  //     native to Win32, but must be emulated on UNIX.
{
public:
  ACE_Event (int manual_reset = 0,
	     int initial_state = 0,
	     int type = USYNC_THREAD, 
	     LPCTSTR name = 0,
	     void *arg = 0);
  // Constructor which will create event.

  ~ACE_Event (void);
  // Implicitly destroy the event variable.     

  int remove (void);
  // Explicitly destroy the event variable.     

  ACE_event_t handle (void) const;
  // Underlying handle to event.

  int wait (void);
  // if MANUAL reset 
  //    sleep till the event becomes signaled
  //    event remains signaled after wait() completes.
  // else AUTO reset
  //    sleep till the event becomes signaled
  //    event resets wait() completes.

  int wait (const ACE_Time_Value *abstime);
  // Same as wait() above, but this one can be timed
  // <abstime> is absolute time-of-day. 

  int signal (void);
  // if MANUAL reset
  //    wake up all waiting threads
  //    set to signaled state
  // else AUTO reset
  //    if no thread is waiting, set to signaled state
  //    if thread(s) are waiting, wake up one waiting thread and 
  //    reset event

  int pulse (void);
  // if MANUAL reset
  //    wakeup all waiting threads and
  //    reset event
  // else AUTO reset
  //    wakeup one waiting thread (if present) and
  //    reset event

  int reset (void);
  // Set to nonsignaled state.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks

private:

  // prevent copy constructors
  ACE_Event (const ACE_Event& event);

  // prevent assignment operators
  const ACE_Event &operator= (const ACE_Event &rhs);

  ACE_event_t handle_;
};

class ACE_Export ACE_Manual_Event : public ACE_Event
  // = TITLE
  //     Manual Events.
  //
  // = DESCRIPTION
  //     Specialization of Event mechanism which 
  //     wakes up all on signal()
{
public:
  ACE_Manual_Event (int initial_state = 0,
		    int type = USYNC_THREAD, 
		    LPCTSTR name = 0,
		    void *arg = 0);
  // constructor which will create manual event

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks
};

class ACE_Export ACE_Auto_Event : public ACE_Event
  // = TITLE
  //     Auto Events.
  //
  // = DESCRIPTION
  //     Specialization of Event mechanism which 
  //     wakes up all on signal()
{
public:
  ACE_Auto_Event (int initial_state = 0,
		  int type = USYNC_THREAD, 
		  LPCTSTR name = 0,
		  void *arg = 0);
  // constructor which will create auto event

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks
};

class ACE_Export ACE_Mutex
  // = TITLE
  //     ACE_Mutex wrapper (valid in same process or across processes
  //     (depending on TYPE flag)) 
{
public:
  ACE_Mutex (int type = USYNC_THREAD, 
	     LPCTSTR name = 0,
	     void *arg = 0);
  // Initialize the mutex.

  ~ACE_Mutex (void);
  // Implicitly destroy the mutex.

  int remove (void);
  // Explicitly destroy the mutex.

  int acquire (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int tryacquire (void);
  // Conditionally acquire lock (i.e., don't wait on queue).

  int release (void);  
  // Release lock and unblock a thread at head of priority queue.

  int acquire_read (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int acquire_write (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int tryacquire_read (void);
  // Conditionally acquire a lock (i.e., won't block).

  int tryacquire_write (void);
  // Conditionally acquire a lock (i.e., won't block).

  const ACE_mutex_t &lock (void) const;
  // Return the underlying mutex.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

//private:
  ACE_mutex_t lock_;
  // Mutex type supported by the OS.

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Mutex &) {}
  ACE_Mutex (const ACE_Mutex &) {}
};

class ACE_Export ACE_Thread_Mutex
  // = TITLE
  //     ACE_Thread_Mutex wrapper (only valid for threads in the same
  //     process).
  //
  // = DESCRIPTION
  //     This implementation is optimized for locking threads that are
  //     in the same process.  It maps to <CRITICAL_SECTION>s on NT
  //     and <ACE_mutex_t> with <type> set to <USYNC_THREAD> on UNIX.
{
public:
  ACE_Thread_Mutex (LPCTSTR name = 0, void *arg = 0);

  ~ACE_Thread_Mutex (void);
  // Implicitly destroy the mutex.

  int remove (void);
  // Explicitly destroy the mutex.

  int acquire (void);
  // Acquire lock ownership (wait on priority queue if necessary). 

  int tryacquire (void);
  // Conditionally acquire lock (i.e., don't wait on queue).

  int release (void);  
  // Release lock and unblock a thread at head of priority queue.

  int acquire_read (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int acquire_write (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int tryacquire_read (void);
  // Conditionally acquire a lock (i.e., won't block).

  int tryacquire_write (void);
  // Conditionally acquire a lock (i.e., won't block).

  const ACE_thread_mutex_t &lock (void) const;
  // Return the underlying mutex.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

//private:
  ACE_thread_mutex_t lock_;
  // Mutex type that supports single-process locking efficiently.

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Thread_Mutex &) {}
  ACE_Thread_Mutex (const ACE_Thread_Mutex &) {}
};

class ACE_Export ACE_Thread_Mutex_Guard
  // = TITLE
  //     This data structure is meant to be used within a method or
  //     function...  It performs automatic aquisition and release of
  //     an ACE_Mutex.
  //
  // = DESCRIPTION
  //     This should be a specialization of ACE_Guard, but compiler
  //     bugs in older C++ compilers preclude this...
{
public:
  ACE_Thread_Mutex_Guard (ACE_Thread_Mutex &m, int block = 1);
  // Implicitly and automatically acquire the lock.

  ~ACE_Thread_Mutex_Guard (void);
  // Implicitly release the lock.

  int locked (void);
  // 1 if locked, 0 if couldn't acquire the lock (errno will contain
  // the reason for this).

  int remove (void);
  // Explicitly release the lock.

  int acquire (void);
  // Explicitly acquire the lock.

  int tryacquire (void);
  // Conditionally acquire the lock (i.e., won't block).

  int release (void);
  // Explicitly release the lock.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Thread_Mutex &lock_;
  // Reference to the mutex.

  int owner_;
  // Keeps track of whether we acquired the lock or failed.

  // = Prevent assignment and initialization.
  void operator= (const ACE_Thread_Mutex_Guard &) {}
  ACE_Thread_Mutex_Guard (const ACE_Thread_Mutex_Guard &g): lock_ (g.lock_) {}
};

class ACE_Export ACE_Condition_Thread_Mutex
  // = TITLE
  //     ACE_Condition variable wrapper written using ACE_Mutexes This
  //     allows threads to block until shared data changes state.
  //
  // = DESCRIPTION
  //     This should be an instantiation of ACE_Condition but problems
  //     with compilers precludes this...
{
public:
  ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
			      LPCTSTR name = 0, 
			      void *arg = 0);
  // Initialize the condition variable.

  ~ACE_Condition_Thread_Mutex (void);
  // Implicitly destroy the condition variable.     

  int remove (void);
  // Explicitly destroy the condition variable.     

  int wait (const ACE_Time_Value *abstime);
  // Block on condition, or until absolute time-of-day has passed.  If
  // abstime == 0 use "blocking" <wait> semantics.  Else, if <abstime>
  // != 0 and the call times out before the condition is signaled
  // <wait> returns -1 and sets errno to ETIME.

  int wait (void);
  // Block on condition.

  int wait (ACE_Thread_Mutex &mutex, const ACE_Time_Value *abstime = 0);
  // Block on condition or until absolute time-of-day has passed.  If
  // abstime == 0 use "blocking" wait() semantics on the <mutex>
  // passed as a parameter (this is useful if you need to store the
  // <Condition> in shared memory).  Else, if <abstime> != 0 and the
  // call times out before the condition is signaled <wait> returns -1
  // and sets errno to ETIME.

  int signal (void);
  // Signal one waiting thread.

  int broadcast (void);
  // Signal *all* waiting threads.

  ACE_Thread_Mutex &mutex (void);
  // Returns a reference to the underlying mutex_;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_cond_t cond_;
  // Condition variable.

  ACE_Thread_Mutex &mutex_; 
  // Reference to mutex lock.

  // = Prevent assignment and initialization.
  void operator= (const ACE_Condition_Thread_Mutex &) {}
  ACE_Condition_Thread_Mutex (const ACE_Condition_Thread_Mutex &c): mutex_ (c.mutex_) {}
};

class ACE_Export ACE_Recursive_Thread_Mutex
  // = TITLE
  //     Implement a C++ wrapper that allows calls to class
  //     <ACE_Thread_Mutex> to be nested for a nested acquire() that
  //     occurs in the same thread.
  //
  // = DESCRIPTION
  //     This class should be a specialization of the
  //     ACE_Recursive_Lock template class, but problems with some C++
  //     compilers preclude this.  This implementation is based
  //     on an algorithm sketched by Dave Butenhof  <butenhof@zko.dec.com>. 
  //     Naturally, I take the credit for any mistakes ;-)
{
  // friend class ACE_Condition<class ACE_COND_MUTEX>;
public:
  ACE_Recursive_Thread_Mutex (LPCTSTR name = 0,
			      void *arg = 0);
  // Initialize a recursive mutex.

  ~ACE_Recursive_Thread_Mutex (void);
  // Implicitly release a recursive mutex.

  int remove (void);
  // Implicitly release a recursive mutex.

  int acquire (void);
  // Acquire a recursive mutex (will increment the nesting level and
  // not deadmutex if the owner of the mutex calls this method more
  // than once).

  int tryacquire (void);
  // Conditionally acquire a recursive mutex (i.e., won't block).

  int release (void);
  // Releases a recursive mutex (will not release mutex until all the
  // nesting level drops to 0, which means the mutex is no longer
  // held).

  ACE_thread_t get_thread_id (void);
  // Return the id of the thread that currently owns the mutex.

  int get_nesting_level (void);
  // Return the nesting level of the recursion.  When a thread has
  // acquired the mutex for the first time, the nesting level == 1.
  // The nesting level is incremented every time the thread acquires
  // the mutex recursively.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  // These methods should *not* be public (they hold no locks...)
  void set_nesting_level (int d);
  void set_thread_id (ACE_thread_t t);

  ACE_Thread_Mutex nesting_mutex_; 
  // Guards the state of the nesting level and thread id.

  ACE_Condition_Thread_Mutex lock_available_;
  // This is the condition variable that actually suspends other
  // waiting threads until the mutex is available.

  int nesting_level_; 
  // Current nesting level of the recursion.

  ACE_thread_t owner_id_;
  // Current owner of the lock.

  // = Prevent assignment and initialization.
  void operator= (const ACE_Recursive_Thread_Mutex &) {}
  ACE_Recursive_Thread_Mutex (const ACE_Recursive_Thread_Mutex &);
};

class ACE_Export ACE_RW_Mutex
  // = TITLE
  //     Wrapper for readers/writer locks.
  //
  // = DESCRIPTION
  //     These are most useful for applications that have many more
  //     parallel readers than writers...
{
public:
  ACE_RW_Mutex (int type = USYNC_THREAD, 
		LPCTSTR name = 0,
		void *arg = 0);
  // Initialize a readers/writer lock.

  ~ACE_RW_Mutex (void);
  // Implicitly destroy a readers/writer lock

  int remove (void);
  // Explicitly destroy a readers/writer lock.

  int acquire_read (void);
  // Acquire a read lock, but block if a writer hold the lock.

  int acquire_write (void);
  // Acquire a write lock, but block if any readers or a 
  // writer hold the lock.

  int tryacquire_read (void);
  // Conditionally acquire a read lock (i.e., won't block).

  int tryacquire_write (void);
  // Conditionally acquire a write lock (i.e., won't block).

  int acquire (void);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <acquire> method.  This is implemented as
  // a write-lock to be on the safe-side...

  int tryacquire (void);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <tryacquire> method.  This is implemented
  // as a write-lock to be on the safe-side...

  int release (void);  
  // Unlock a readers/writer lock.

  const ACE_rwlock_t &lock (void) const;
  // Return the underlying lock.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_rwlock_t lock_;
  // Readers/writer lock.

  // = Prevent assignment and initialization.
  void operator= (const ACE_RW_Mutex &) {}
  ACE_RW_Mutex (const ACE_RW_Mutex &) {}
};

class ACE_Export ACE_RW_Thread_Mutex : public ACE_RW_Mutex
  // = TITLE
  //     Wrapper for readers/writer locks that exist within a process.
{
public:
  ACE_RW_Thread_Mutex (LPCTSTR name = 0,
		       void *arg = 0);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Thread_Semaphore : public ACE_Semaphore
  // = TITLE
  //     Wrapper for Dijkstra style general semaphores that work
  //     only within on process.
{
public:
  ACE_Thread_Semaphore (u_int count, LPCTSTR name = 0,
			void * = 0, int max = 0x7FFFFFFF);
  // Initialize the semaphore, with an initial value of <count> and a
  // maximum value of <max>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

struct ACE_Export ACE_Sub_Barrier 
{ 
  // = Initialization.
  ACE_Sub_Barrier (u_int count,
		   ACE_Thread_Mutex &lock,
		   LPCTSTR name = 0,
		   void *arg = 0);

  ACE_Condition_Thread_Mutex barrier_finished_;
  // True if this generation of the barrier is done.

  int running_threads_;
  // Number of threads that are still running.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Barrier
  // = TITLE 
  //     Implements "barrier synchronization".
  //
  // = DESCRIPTION
  //     This class allows <count> number of threads to synchronize
  //     their completion (so-called "barrier synchronization").  The
  //     implementation uses a "sub-barrier generation numbering"
  //     scheme to avoid overhead and to ensure that all threads exit
  //     the barrier correct.  This code is based on an article from
  //     SunOpsis Vol. 4, No. 1 by Richard Marejka
  //     (Richard.Marejka@canada.sun.com).
{
public:
  ACE_Barrier (u_int count, 
	       LPCTSTR name = 0, 
	       void *arg = 0);
  // Initialize the barrier to synchronize <count> threads.

  int wait (void);
  // Block the caller until all <count> threads have called <wait> and
  // then allow all the caller threads to continue in parallel.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Thread_Mutex lock_;
  // Serialize access to the barrier state.

  int current_generation_;
  // Either 0 or 1, depending on whether we are the first generation
  // of waiters or the next generation of waiters.

  int count_;
  // Total number of threads that can be waiting at any one time.
  
  ACE_Sub_Barrier sub_barrier_1_;
  ACE_Sub_Barrier sub_barrier_2_;
  ACE_Sub_Barrier *sub_barrier_[2];
  // We keep two <sub_barriers>, one for the first "generation" of
  // waiters, and one for the next "generation" of waiters.  This
  // efficiently solves the problem of what to do if all the first
  // generation waiters don't leave the barrier before one of the
  // threads calls wait() again (i.e., starts up the next generation
  // barrier).

  // = Prevent assignment and initialization.
  void operator= (const ACE_Barrier &) {}
  ACE_Barrier (const ACE_Barrier &): sub_barrier_1_ (0, lock_), sub_barrier_2_ (0, lock_) {}
};

#if 0
class ACE_Export ACE_Process_Barrier : public ACE_Barrier
  // = TITLE 
  //     Implements "barrier synchronization" using ACE_Process_Mutexes! 
  //
  // = DESCRIPTION
  //     This class is just a simple wrapper for ACE_Barrier that
  //     selects the USYNC_PROCESS variant for the locks.
{
public:
  ACE_Process_Barrier (u_int count, LPCTSTR name = 0);
  // Create a Process_Barrier, passing in the optional <name>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};
#endif /* 0 */

class ACE_Export ACE_Thread_Barrier : public ACE_Barrier
  // = TITLE 
  //     Implements "barrier synchronization" using ACE_Thread_Mutexes!
  //
  // = DESCRIPTION
  //     This class is just a simple wrapper for ACE_Barrier that
  //     selects the USYNC_THREAD variant for the locks.
{
public:
  ACE_Thread_Barrier (u_int count, LPCTSTR name = 0);
  // Create a Process_Barrier, passing in the optional <name>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#endif /* ACE_HAS_THREADS */

class ACE_Export ACE_Process_Mutex
  // = TITLE
  //     ACE_Mutex wrapper (valid in same process, as well as across
  //     processes).
{
public:
  ACE_Process_Mutex (LPCTSTR name = ACE_DEFAULT_MUTEX, void *arg = 0);
  // Create a Process_Mutex, passing in the optional <name>.

  ~ACE_Process_Mutex (void);

  int remove (void);
  // Explicitly destroy the mutex.

  int acquire (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int tryacquire (void);
  // Conditionally acquire lock (i.e., don't wait on queue).

  int release (void);  
  // Release lock and unblock a thread at head of priority queue.

  int acquire_read (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int acquire_write (void);
  // Acquire lock ownership (wait on priority queue if necessary).

  int tryacquire_read (void);
  // Conditionally acquire a lock (i.e., won't block).

  int tryacquire_write (void);
  // Conditionally acquire a lock (i.e., won't block).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  ACE_Mutex lock_;
#else
  ACE_SV_Semaphore_Complex lock_;
  // We need this to get the right semantics...
#endif /* ACE_WIN32 */
};

class ACE_Export ACE_RW_Process_Mutex : public ACE_Process_Mutex
  // = TITLE
  //     Wrapper for readers/writer locks that exist across processes.
{
public:
  ACE_RW_Process_Mutex (LPCTSTR name = ACE_DEFAULT_MUTEX,
			void *arg = 0);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Null_Barrier
  // = TITLE 
  //     Implements "NULL barrier synchronization".
{
public:
  ACE_Null_Barrier (u_int,
		    const char * = 0, 
		    void * = 0) {}
  // Initialize the barrier to synchronize <count> threads.

  int wait (void) { return 0; }
  // Block the caller until all <count> threads have called <wait> and
  // then allow all the caller threads to continue in parallel.

  void dump (void) const {}
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:

  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Barrier &) {}
  ACE_Null_Barrier (const ACE_Null_Barrier &) {}
};

class ACE_Export ACE_Null_Mutex
  // = TITLE
  //     Implement a do nothing <ACE_Mutex>, i.e., all the methods are
  //     no ops. 
{
public:
  ACE_Null_Mutex (LPCTSTR = 0) {}
  ~ACE_Null_Mutex (void) {}
  int remove (void) { return 0; }

  int acquire (void) { return 0; }
  int tryacquire (void) { return 0; }
  int release (void) { return 0; }
  int acquire_write (void) { return 0; }
  int tryacquire_write (void) { return 0; }
  int acquire_read (void) { return 0; }
  int tryacquire_read (void) { return 0; }

  void dump (void) const { }
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Null_Condition_Mutex
  // = TITLE
  //     Implement a do nothing <ACE_Condition> variable wrapper, i.e.,
  //     all methods are no ops.  This class is necessary since some
  //     C++ compilers are *very* lame...
{
public:
  ACE_Null_Condition_Mutex (ACE_Null_Mutex &m, int = 0, 
			    LPCTSTR = 0, void * = 0): mutex_ (m) {}
  ~ACE_Null_Condition_Mutex (void) {}
  int remove (void) { return 0; }
  int wait (ACE_Time_Value * = 0) { return 0; }
  int signal (void) { return 0; }
  int broadcast (void) { return 0; }
  ACE_Null_Mutex &mutex (void) { return this->mutex_; }

  void dump (void) const {}
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Null_Mutex &mutex_; // Reference to mutex lock.  

  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Condition_Mutex &) {}
  ACE_Null_Condition_Mutex (const ACE_Null_Condition_Mutex &c): mutex_ (c.mutex_) {}
};

class ACE_Export ACE_Null_Mutex_Guard
  // = TITLE
  //     This data structure is meant to be used within a method or
  //     function...  It performs automatic aquisition and release of
  //     an ACE_Null_Mutex.
  //
  // = DESCRIPTION
  //     This should be a specialization of ACE_Guard, but compiler
  //     bugs preclude this...
{
public:
  ACE_Null_Mutex_Guard (ACE_Null_Mutex &) {}
  ~ACE_Null_Mutex_Guard (void) {}
  int remove (void) { return 0; }
  int locked (void) { return 1; }
  int acquire (void) { return 0; }
  int tryacquire (void) { return 0; }
  int release (void) { return 0; }
  void dump (void) const { }

protected:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Mutex_Guard &) {}
  ACE_Null_Mutex_Guard (const ACE_Null_Mutex_Guard &) {}
};

#if defined (__ACE_INLINE__)
#include "ace/Synch.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Synch_T.h"

#endif /* ACE_SYNCH_H */
