/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Synch_T.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SYNCH_T_H)
#define ACE_SYNCH_T_H

#include "ace/Event_Handler.h"
#include "ace/Synch.h"

// Forward decl
class ACE_Time_Value;

template <class ACE_LOCKING_MECHANISM>
class ACE_Lock_Adapter : public ACE_Lock
  // = TITLE

  //     This is an adapter that allows applications to transparently
  //     combine the <ACE_Lock> abstract base class (which contains
  //     pure virtual methods) with any of the other concrete ACE
  //     synchronization classes (e.g., <ACE_Mutex>, <ACE_Semaphore>,
  //     <ACE_RW_Mutex>, etc.).
  //
  // = DESCRIPTION
  //     This class uses a form of the Adapter pattern.
{
public:
  typedef ACE_LOCKING_MECHANISM ACE_LOCK;

  // = Initialization/Finalization methods.

  ACE_Lock_Adapter (ACE_LOCKING_MECHANISM &lock);
  // Constructor. All locking requests will be forwarded to <lock>.

  ACE_Lock_Adapter (void);
  // Constructor. Since no lock is provided by the user, one will be
  // created internally.

  virtual ~ACE_Lock_Adapter (void);
  // Destructor. If <lock_> was not passed in by the user, it will be
  // deleted.

  // = Lock accessors.
  virtual int acquire (void);
  // Block the thread until the lock is acquired.

  virtual int tryacquire (void);
  // Conditionally acquire the lock (i.e., won't block).

  virtual int release (void);
  // Release the lock.

  virtual int acquire_read (void);
  // Block until the thread acquires a read lock.  If the locking
  // mechanism doesn't support read locks then this just calls
  // <acquire>.

  virtual int acquire_write (void);
  // Block until the thread acquires a write lock.  If the locking
  // mechanism doesn't support read locks then this just calls
  // <acquire>.

  virtual int tryacquire_read (void);
  // Conditionally acquire a read lock.  If the locking mechanism
  // doesn't support read locks then this just calls <acquire>.

  virtual int tryacquire_write (void);
  // Conditionally acquire a write lock.  If the locking mechanism
  // doesn't support read locks then this just calls <acquire>.

  virtual int remove (void);
  // Explicitly destroy the lock.

private:
  ACE_LOCKING_MECHANISM *lock_;
  // The concrete locking mechanism that all the methods delegate to.
  
  int delete_lock_;
  // This flag keep track of whether we are responsible for deleting
  // the lock
};

template <class ACE_LOCK, class TYPE>
class ACE_Test_and_Set : public ACE_Event_Handler
{
  // = TITLE
  //     Implements the classic ``test and set'' operation.
  //
  // = DESCRIPTION
  //     This class keeps track of the status of <is_set_>, which can
  //     be set based on various events (such as receipt of a
  //     signal).  This class is derived from <ACE_Event_Handler> so
  //     that it can be "signaled" by a Reactor when a signal occurs.
  //     We assume that <TYPE> is a data type that can be assigned the
  //     value 0 or 1.
public:
  ACE_Test_and_Set (TYPE initial_value = 0);

  TYPE is_set (void) const;
  // Returns true if we are set, else false.

  TYPE set (TYPE);
  // Sets the <is_set_> status, returning the original value of
  // <is_set_>.

  virtual int handle_signal (int signum, 
			     siginfo_t * = 0, 
			     ucontext_t * = 0);
  // Called when object is signaled by OS (either via UNIX signals or
  // when a Win32 object becomes signaled).

private:
  TYPE is_set_;
  // Keeps track of our state.

  ACE_LOCK lock_;
  // Protect the state from race conditions.
};

template <class ACE_LOCK, class TYPE>
class ACE_Atomic_Op
  // = TITLE
  //     Transparently parameterizes synchronization into basic
  //     arithmetic operations.
  //
  // = DESCRIPTION
  //     This class is described in an article in the July/August 1994
  //     issue of the C++ Report magazine.  It implements a
  //     templatized version of the Decorator pattern from the GoF book.
{
public:
  // = Initialization methods.

  ACE_Atomic_Op (void);
  // Initialize <count_> to 0.

  ACE_Atomic_Op (const TYPE &c);
  // Initialize <count_> to c.

  // = Accessors.

  TYPE operator++ (void);
  // Atomically pre-increment <count_>.

  TYPE operator++ (int);
  // Atomically post-increment <count_>.

  TYPE operator+= (const TYPE &i);
  // Atomically increment <count_> by inc.

  TYPE operator-- (void);
  // Atomically pre-decrement <count_>.

  TYPE operator-- (int);
  // Atomically post-decrement <count_>.

  TYPE operator-= (const TYPE &i);
  // Atomically decrement <count_> by dec.

  int operator== (const TYPE &i) const;
  // Atomically compare <count_> with rhs.

  int operator>= (const TYPE &i) const;
  // Atomically check if <count_> greater than or equal to rhs.

  int operator> (const TYPE &rhs) const;
  // Atomically check if <count_> greater than rhs.

  int operator<= (const TYPE &rhs) const;
  // Atomically check if <count_> less than or equal to rhs.

  int operator< (const TYPE &rhs) const;
  // Atomically check if <count_> less than rhs.

  void operator= (const TYPE &i);
  // Atomically assign rhs to <count_>.

  void operator= (const ACE_Atomic_Op<ACE_LOCK, TYPE> &rhs);
  // Atomically assign <rhs> to <count_>.

  operator TYPE () const;
  // Atomically return <count_>.

  TYPE value (void) const;
  // Explicitly return <count_>.

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  ACE_Atomic_Op (const ACE_Atomic_Op<ACE_LOCK, TYPE> &);
  // Manage copying...

private:
  ACE_LOCK lock_;
  // Type of synchronization mechanism.

  TYPE value_;
  // Current object decorated by the atomic op.
};

template <class TYPE>
class ACE_TSS
  // = TITLE
  //     Allows objects that are "physically" in thread specific
  //     storage (i.e., private to a thread) to be accessed as though
  //     they were "logically" global to a program.
  //
  // = DESCRIPTION
  //     This class is a wrapper around the OS thread library
  //     thread-specific functions.  It uses the C++ operator->() to
  //     shield applications from the details of accessing
  //     thread-specific storage.
{
public:
  // = Initialization and termination methods.

  ACE_TSS (TYPE *ts_obj = 0);
  // If caller has passed us a non-NULL ts_obj *, then we'll just use
  // this to initialize the thread-specific value (but only for the
  // calling thread).  Thus, subsequent calls to operator->() in this
  // thread will return this value.  This is useful since it enables
  // us to assign objects to thread-specific data that have
  // arbitrarily complex constructors.

  virtual ~ACE_TSS (void);
  // Deregister with thread-key administration.

  // = Accessors.

  TYPE *ts_object (void) const;
  // Get the thread-specific object for the key associated with this
  // object.  Returns 0 if the data has never been initialized,
  // otherwise returns a pointer to the data.

  TYPE *ts_object (TYPE *);
  // Set the thread-specific object for the key associated with this
  // object.  Returns 0 if the data has never been initialized,
  // otherwise returns a pointer to the previous value for the data.

  TYPE *operator-> () const;
  // Use a "smart pointer" to get the thread-specific object
  // associated with the <key_>.

  operator TYPE *(void) const;
  // return or create and return the calling threads TYPE object.

  virtual TYPE *make_TSS_TYPE (void) const;
  // hook for construction parameters.

  // = Utility methods.

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  TYPE *ts_get (void) const;
  // Actually implements the code that retrieves the object from
  // thread-specific storage. 

#if !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
  TYPE type_;
  // This implementation only works for non-threading systems...
#else
  ACE_Thread_Mutex keylock_;
  // Avoid race conditions during initialization.

  int once_;
  // "First time in" flag.

  ACE_thread_key_t key_;
  // Key for the thread-specific error data.

  static void cleanup (void *ptr);
  // "Destructor" that deletes internal TYPE * when thread exits.
#endif /* defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) */
  // = Disallow copying...
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_TSS<TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_TSS (const ACE_TSS<TYPE> &))
};

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
class ACE_NULL_SYNCH
  // = TITLE
  //     Implement a do nothing Synchronization wrapper that
  //     typedefs the <ACE_Condition> and <ACE_Mutex> to the Null* versions.
{
public:
  typedef ACE_Null_Mutex MUTEX;
  // "Do-nothing" Mutex type.

  typedef ACE_Null_Mutex RW_MUTEX;
  // "Do-nothing" RW Mutex type.
  
  typedef ACE_Null_Condition CONDITION;
  // "Do-nothing" Condition type.

  typedef ACE_Null_Mutex SEMAPHORE;
  // "Do-nothing" Condition type.
};
#else /* Necessary to support broken cfront-based C++ compilers... */
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
#define ACE_NULL_SYNCH ACE_Null_Mutex, ACE_Null_Condition, ACE_Null_Mutex
#else
#define ACE_NULL_SYNCH ACE_Null_Mutex, ACE_Null_Condition
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

template <class ACE_LOCK>
class ACE_Guard
  // = TITLE
  //     This data structure is meant to be used within a method or
  //     function...  It performs automatic aquisition and release of
  //     a parameterized synchronization object <ACE_LOCK>.
  //
  // = DESCRIPTION
  //     The <ACE_LOCK> class given as an actual parameter must provide at
  //     the very least the <acquire>, <tryacquire>, <release>, and
  //     <remove> methods.
{
public:
  // = Initialization and termination methods.
  ACE_Guard (ACE_LOCK &l): lock_ (&l) { this->owner_ = this->acquire(); }

  ACE_Guard (ACE_LOCK &l, int block): lock_ (&l)
    {
      if (block)
        this->acquire ();
      else
        this->tryacquire ();
    }
  // Implicitly and automatically acquire (or try to acquire) the
  // lock.

  ~ACE_Guard (void) { this->release (); }
  // Implicitly release the lock.

  // = Lock accessors.

  int acquire (void) { return this->owner_ = this->lock_->acquire (); }
  // Explicitly acquire the lock.

  int tryacquire (void) { return this->owner_ = this->lock_->tryacquire (); }
  // Conditionally acquire the lock (i.e., won't block).

  int release (void) 
    { 
      if (this->owner_ != -1) 
	{
	  this->owner_ = -1; 
	  return this->lock_->release (); 
	}
      else
	return 0;
    }
  // Explicitly release the lock, but only if it is held!

  // = Utility methods.
  int locked (void) { return this->owner_ != -1; }
  // 1 if locked, 0 if couldn't acquire the lock
  // (errno will contain the reason for this).

  int remove (void) { return this->release (); }
  // Explicitly release the lock.

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Guard (ACE_LOCK *lock): lock_ (lock) {}
  // Helper, meant for subclass only.

  ACE_LOCK *lock_;
  // Pointer to the ACE_LOCK we're guarding.

  int owner_;
  // Keeps track of whether we acquired the lock or failed.

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Guard<ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Guard (const ACE_Guard<ACE_LOCK> &))
};

template <class ACE_LOCK>
class ACE_Write_Guard : public ACE_Guard<ACE_LOCK>
  // = TITLE
  //     This class is similar to class <ACE_Guard>, though it
  //     acquires/releases a write lock automatically (naturally, the
  //     <ACE_LOCK> it is instantiated with must support the appropriate
  //     API).
{
public:
  // = Initialization method.

  ACE_Write_Guard (ACE_LOCK &m): ACE_Guard<ACE_LOCK> (&m)
    { this->owner_ = this->acquire_write (); }

  ACE_Write_Guard (ACE_LOCK &m, int block): ACE_Guard<ACE_LOCK> (&m) 
    { 
      this->owner_ = block ? this->acquire_write () : this->tryacquire_write ();
    }
  // Implicitly and automatically acquire (or try to acquire) a write
  // lock.

  // = Lock accessors.

  int acquire_write (void) { return this->owner_ = this->lock_->acquire_write (); }
  // Explicitly acquire the write lock.

  int acquire (void) { return this->owner_ = this->lock_->acquire_write (); }
  // Explicitly acquire the write lock.

  int tryacquire_write (void) { return this->owner_ = this->lock_->tryacquire_write (); }
  // Conditionally acquire the write lock (i.e., won't block).

  int tryacquire (void) { return this->owner_ = this->lock_->tryacquire_write (); }
  // Conditionally acquire the write lock (i.e., won't block).

  // Utility methods.

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

template <class ACE_LOCK>
class ACE_Read_Guard : public ACE_Guard<ACE_LOCK>
  // = TITLE
  //     This class is similar to class <ACE_Guard>, though it
  //     acquires/releases a read lock automatically (naturally, the
  //     <ACE_LOCK> it is instantiated with must support the appropriate
  //     API).
{
public:
  // = Initialization methods.

  ACE_Read_Guard (ACE_LOCK& m): ACE_Guard<ACE_LOCK> (&m) 
    { this->owner_ = this->acquire_read (); }

  ACE_Read_Guard (ACE_LOCK &m, int block): ACE_Guard<ACE_LOCK> (&m) 
    { 
      this->owner_ = block ? this->acquire_read () : this->tryacquire_read ();
    }
  // Implicitly and automatically acquire (or try to acquire) a read
  // lock.

  // = Lock accessors.

  int acquire_read (void) { return this->owner_ = this->lock_->acquire_read (); }
  // Explicitly acquire the read lock.

  int acquire (void) { return this->owner_ = this->lock_->acquire_read (); }
  // Explicitly acquire the read lock.

  int tryacquire_read (void) { return this->owner_ = this->lock_->tryacquire_read (); }
  // Conditionally acquire the read lock (i.e., won't block).

  int tryacquire (void) { return this->owner_ = this->lock_->tryacquire_read (); }
  // Conditionally acquire the read lock (i.e., won't block).

  // = Utility methods.

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))

#define ACE_TSS_Guard ACE_Guard
#define ACE_TSS_Write_GUARD ACE_Write_Guard
#define ACE_TSS_Read_GUARD ACE_Read_Guard

#else /* ACE platform supports some form of threading and */
      // thread-specific storage.

template <class ACE_LOCK>
class ACE_TSS_Guard
  // = TITLE
  //     This data structure is meant to be used within a method or
  //     function...  It performs automatic aquisition and release of
  //     a synchronization object.  Moreover, it ensures that the lock
  //     is released even if a thread exits via "thr_exit()"!
{
public:
  // = Initialization and termination methods.

  ACE_TSS_Guard (ACE_LOCK &lock, int block = 1);
  // Implicitly and automatically acquire the thread-specific lock.

  ~ACE_TSS_Guard (void);
  // Implicitly release the thread-specific lock.

  // = Lock accessors.

  int acquire (void);
  // Explicitly acquire the thread-specific lock.

  int tryacquire (void);
  // Conditionally acquire the thread-specific lock (i.e., won't
  // block).

  int release (void);
  // Explicitly release the thread-specific lock.

  // = Utility methods.
  int remove (void);
  // Explicitly release the thread-specific lock.

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_TSS_Guard (void);
  // Helper, meant for subclass only.

  void init_key (void);
  // Initialize the key.

  static void cleanup (void *ptr);
  // Called when thread exits to clean up the lock.

  ACE_thread_key_t key_;
  // Thread-specific key...

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_TSS_Guard<ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_TSS_Guard (const ACE_TSS_Guard<ACE_LOCK> &))
};

template <class ACE_LOCK>
class ACE_TSS_Write_Guard : public ACE_TSS_Guard<ACE_LOCK>
  // = TITLE
  //     This class is similar to class ACE_TSS_Guard, though it
  //     acquires/releases a write-lock automatically (naturally, the
  //     ACE_LOCK it is instantiated with must support the appropriate
  //     API).
{
public:
  // = Initialization method.

  ACE_TSS_Write_Guard (ACE_LOCK &lock, int block = 1);
  // Implicitly and automatically acquire the thread-specific write lock.

  // = Lock accessors.

  int acquire_write (void);
  // Explicitly acquire the thread-specific write lock.

  int acquire (void);
  // Explicitly acquire the thread-specific write lock.

  int tryacquire_write (void);
  // Conditionally acquire the thread-specific write lock (i.e., won't block).

  int tryacquire (void);
  // Conditionally acquire the thread-specific write lock (i.e., won't block).

  // = Utility methods.

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

template <class ACE_LOCK>
class ACE_TSS_Read_Guard : public ACE_TSS_Guard<ACE_LOCK>
  // = TITLE
  //     This class is similar to class <ACE_TSS_Guard>, though it
  //     acquires/releases a read lock automatically (naturally, the
  //     <ACE_LOCK> it is instantiated with must support the
  //     appropriate API).
{
public:
  // = Initialization method.
  ACE_TSS_Read_Guard (ACE_LOCK &lock, int block = 1);
  // Implicitly and automatically acquire the thread-specific read lock.

  // = Lock accessors.
  int acquire_read (void);
  // Explicitly acquire the thread-specific read lock.

  int acquire (void);
  // Explicitly acquire the thread-specific read lock.

  int tryacquire_read (void);
  // Conditionally acquire the thread-specific read lock (i.e., won't
  // block).

  int tryacquire (void);
  // Conditionally acquire the thread-specific read lock (i.e., won't
  // block).

  // = Utility methods.
  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};
#endif /* !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))) */

#if defined (ACE_HAS_THREADS) /* ACE platform supports some form of threading. */

template <class MUTEX>
class ACE_Condition
  // = TITLE
  //     ACE_Condition variable wrapper, which allows threads to block
  //     until shared data changes state.
  //
  //     A condition variable enables threads to atomically block and
  //     test the condition under the protection of a mutual exclu-
  //     sion lock (mutex) until the condition is satisfied.  That is,
  //     the mutex must have been held by the thread before calling
  //     wait or signal on the condition.  If the condition is false,
  //     a thread blocks on a condition variable and atomically
  //     releases the mutex that is waiting for the condition to
  //     change.  If another thread changes the condition, it may wake
  //     up waiting threads by signaling the associated condition
  //     variable.  The waiting threads, upon awakening, reacquire the
  //     mutex and re-evaluate the condition.
  //
{
public:
  // = Initialiation and termination methods.
  ACE_Condition (MUTEX &m, int type = USYNC_THREAD, 
		 LPCTSTR name = 0, void *arg = 0);
  // Initialize the condition variable.

  ~ACE_Condition (void);
  // Implicitly destroy the condition variable.     

  // = Lock accessors.
  int wait (const ACE_Time_Value *abstime);
  // Block on condition, or until absolute time-of-day has passed.  If
  // abstime == 0 use "blocking" <wait> semantics.  Else, if <abstime>
  // != 0 and the call times out before the condition is signaled
  // <wait> returns -1 and sets errno to ETIME.

  int wait (void);
  // Block on condition.

  int wait (MUTEX &mutex, const ACE_Time_Value *abstime = 0);
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

  // = Utility methods.
  int remove (void);
  // Explicitly destroy the condition variable.     

  MUTEX &mutex (void);
  // Returns a reference to the underlying mutex_;

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_cond_t cond_;
  // Condition variable.

  MUTEX &mutex_; 
  // Reference to mutex lock.

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Condition<MUTEX> &))
  ACE_Condition (const ACE_Condition<MUTEX> &c): mutex_ (c.mutex_) {}
};

template <class MUTEX>
class ACE_Thread_Condition : public ACE_Condition<MUTEX>
  // = TITLE
  //     ACE_Condition variable wrapper that works within processes.
  //
  //     A condition variable enables threads to atomically block and
  //     test the condition under the protection of a mutual exclu-
  //     sion lock (mutex) until the condition is satisfied.  That is,
  //     the mutex must have been held by the thread before calling
  //     wait or signal on the condition.  If the condition is false,
  //     a thread blocks on a condition variable and atomically
  //     releases the mutex that is waiting for the condition to
  //     change.  If another thread changes the condition, it may wake
  //     up waiting threads by signaling the associated condition
  //     variable.  The waiting threads, upon awakening, reacquire the
  //     mutex and re-evaluate the condition.
  //
{
public:
  // = Initialization method.
  ACE_Thread_Condition (MUTEX &m, LPCTSTR name = 0, void *arg = 0);

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
class ACE_MT_SYNCH
  // = TITLE
  //     Implement a default thread safe synchronization wrapper that
  //     typedefs the <ACE_Condition> and <ACE_Mutex> to the
  //     <ACE_Condition> and <ACE_Mutex> versions.  Note that this
  //     should be a template, but SunC++ 4.0.1 complains about
  //     this...
{
public:
  typedef ACE_Thread_Mutex MUTEX;
  typedef ACE_Null_Mutex NULL_MUTEX;
  typedef ACE_Process_Mutex PROCESS_MUTEX;
  typedef ACE_Recursive_Thread_Mutex RECURSIVE_MUTEX;
  typedef ACE_RW_Thread_Mutex RW_MUTEX;
  typedef ACE_Condition_Thread_Mutex CONDITION;
  typedef ACE_Thread_Semaphore SEMAPHORE;
  typedef ACE_Null_Mutex NULL_SEMAPHORE;
};
#define ACE_SYNCH_MUTEX ACE_MT_SYNCH::MUTEX
#define ACE_SYNCH_NULL_MUTEX ACE_MT_SYNCH::NULL_MUTEX
#define ACE_SYNCH_PROCESS_MUTEX  ACE_MT_SYNCH::PROCESS_MUTEX;
#define ACE_SYNCH_RECURSIVE_MUTEX ACE_MT_SYNCH::RECURSIVE_MUTEX
#define ACE_SYNCH_RW_MUTEX ACE_MT_SYNCH::RW_MUTEX
#define ACE_SYNCH_CONDITION ACE_MT_SYNCH::CONDITION
#define ACE_SYNCH_NULL_SEMAPHORE ACE_MT_SYNCH::NULL_SEMAPHORE
#define ACE_SYNCH_SEMAPHORE ACE_MT_SYNCH::SEMAPHORE
#else /* Necessary to support broken cfront-based C++ compilers... */
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
#define ACE_MT_SYNCH ACE_Thread_Mutex, ACE_Condition_Thread_Mutex, ACE_Thread_Semaphore
#else
#define ACE_MT_SYNCH ACE_Thread_Mutex, ACE_Condition_Thread_Mutex
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
#define ACE_SYNCH_MUTEX ACE_Thread_Mutex
#define ACE_SYNCH_NULL_MUTEX  ACE_Null_Mutex
#define ACE_SYNCH_PROCESS_MUTEX  ACE_Process_Mutex
#define ACE_SYNCH_RECURSIVE_MUTEX ACE_Recursive_Thread_Mutex
#define ACE_SYNCH_RW_MUTEX ACE_RW_Thread_Mutex
#define ACE_SYNCH_CONDITION ACE_Thread_Condition<ACE_Thread_Mutex>
#define ACE_SYNCH_SEMAPHORE ACE_Thread_Semaphore
#define ACE_SYNCH_NULL_SEMAPHORE  ACE_Null_Mutex
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

#define ACE_SYNCH ACE_MT_SYNCH
#else
#define ACE_SYNCH ACE_NULL_SYNCH
#define ACE_SYNCH_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_NULL_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_PROCESS_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_RECURSIVE_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_RW_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_CONDITION ACE_Null_Condition
#define ACE_SYNCH_SEMAPHORE ACE_Thread_Semaphore
#define ACE_SYNCH_NULL_SEMAPHORE ACE_Null_Mutex
#endif /* ACE_HAS_THREADS */

#if defined (__ACE_INLINE__)
#include "ace/Synch_T.i"
// On non-Win32 platforms, this code will be inlined
#if !defined (ACE_WIN32)
#include "ace/Atomic_Op.i"
#endif /* !ACE_WIN32 */
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Synch_T.cpp"
// On Win32 platforms, this code will be included as template source
// code and will not be inlined. Therefore, we first turn off
// ACE_INLINE, set it to be nothing, include the code, and then turn
// ACE_INLINE back to its original setting. All this nonsense is
// necessary, since the generic template code that needs to be
// specialized cannot be inlined, else the compiler will ignore the
// specialization code. Also, the specialization code *must* be
// inlined or the compiler will ignore the specializations.
#if defined (ACE_WIN32)
#undef ACE_INLINE
#define ACE_INLINE 
#include "ace/Atomic_Op.i"
#undef ACE_INLINE
#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#else
#define ACE_INLINE 
#endif /* __ACE_INLINE__ */
#endif /* ACE_WIN32 */
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Synch_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SYNCH_T_H */
