/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Synch_T.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@uci.edu>
 */
//=============================================================================

#ifndef ACE_SYNCH_T_H
#define ACE_SYNCH_T_H
#include "ace/pre.h"
#include "ace/Threads/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decl
class ACE_Time_Value;

/**
 * @class ACE_Lock_Adapter
 *
 * @brief This is an adapter that allows applications to transparently
 * combine the <ACE_Lock> abstract base class (which contains
 * pure virtual methods) with any of the other concrete ACE
 * synchronization classes (e.g., <ACE_Mutex>, <ACE_Semaphore>,
 * <ACE_RW_Mutex>, etc.).
 *
 * This class uses a form of the Adapter pattern.
 */
template <class ACE_LOCKING_MECHANISM>
class ACE_Lock_Adapter : public ACE_Lock
{
public:
  typedef ACE_LOCKING_MECHANISM ACE_LOCK;

  // = Initialization/Finalization methods.

  /// Constructor. All locking requests will be forwarded to <lock>.
  ACE_Lock_Adapter (ACE_LOCKING_MECHANISM &lock);

  /// Constructor. Since no lock is provided by the user, one will be
  /// created internally.
  ACE_Lock_Adapter (void);

  /// Destructor. If <lock_> was not passed in by the user, it will be
  /// deleted.
  virtual ~ACE_Lock_Adapter (void);

  // = Lock accessors.
  /// Block the thread until the lock is acquired.
  virtual int acquire (void);

  /// Conditionally acquire the lock (i.e., won't block).
  virtual int tryacquire (void);

  /// Release the lock.
  virtual int release (void);

  /**
   * Block until the thread acquires a read lock.  If the locking
   * mechanism doesn't support read locks then this just calls
   * <acquire>.
   */
  virtual int acquire_read (void);

  /**
   * Block until the thread acquires a write lock.  If the locking
   * mechanism doesn't support read locks then this just calls
   * <acquire>.
   */
  virtual int acquire_write (void);

  /// Conditionally acquire a read lock.  If the locking mechanism
  /// doesn't support read locks then this just calls <acquire>.
  virtual int tryacquire_read (void);

  /// Conditionally acquire a write lock.  If the locking mechanism
  /// doesn't support read locks then this just calls <acquire>.
  virtual int tryacquire_write (void);

  /**
   * Conditionally try to upgrade a lock held for read to a write lock.
   * If the locking mechanism doesn't support read locks then this just
   * calls <acquire>. Returns 0 on success, -1 on failure.
   */
  virtual int tryacquire_write_upgrade (void);

  /// Explicitly destroy the lock.
  virtual int remove (void);

private:
  /// The concrete locking mechanism that all the methods delegate to.
  ACE_LOCKING_MECHANISM *lock_;

  /// This flag keep track of whether we are responsible for deleting
  /// the lock
  int delete_lock_;
};

/**
 * @class ACE_Acquire_Method
 *
 * @brief An enum class.
 *
 * These enums should have been inside the reverse lock class, but
 * some lame compilers cannot handle enums inside template classes.
 *
 * The METHOD_TYPE is used to indicate which acquire() method will be
 * called on the real lock when the release() method is called on the
 * reverse lock. REGULAR indicated the acquire() method, READ
 * indicates the acquire_read() method, and WRITE indicates the
 * acquire_write() method.  Note that the try_*() methods are not
 * represented here because we have to make sure that the release()
 * method on the reverse lock acquires a lock on the real lock.
 **/
class ACE_Acquire_Method
{
public:
  enum METHOD_TYPE
  {
    ACE_REGULAR,
    ACE_READ,
    ACE_WRITE
  };
};

/**
 * @class ACE_Reverse_Lock
 *
 * @brief A reverse (or anti) lock.
 *
 * This is an interesting adapter class that changes a lock into
 * a reverse lock, i.e., <acquire> on this class calls <release>
 * on the lock, and <release> on this class calls <acquire> on
 * the lock.
 * One motivation for this class is when we temporarily want to
 * release a lock (which we have already acquired) but then
 * reacquire it soon after.  An alternative design would be to
 * add a Anti_Guard or Reverse_Guard class which would <release>
 * on construction and <acquire> destruction.  However, there
 * are *many* varieties of the Guard class and this design
 * choice would lead to at least 6 new classes.  One new
 * ACE_Reverse_Lock class seemed more reasonable.
 */
template <class ACE_LOCKING_MECHANISM>
class ACE_Reverse_Lock : public ACE_Lock
{
public:

  typedef ACE_LOCKING_MECHANISM ACE_LOCK;

  // = Initialization/Finalization methods.

  /// Constructor. All locking requests will be forwarded to <lock>.
  ACE_Reverse_Lock (ACE_LOCKING_MECHANISM &lock,
                    ACE_Acquire_Method::METHOD_TYPE acquire_method = ACE_Acquire_Method::ACE_REGULAR);

  /// Destructor. If <lock_> was not passed in by the user, it will be
  /// deleted.
  virtual ~ACE_Reverse_Lock (void);

  // = Lock accessors.
  /// Release the lock.
  virtual int acquire (void);

  /// Release the lock.
  virtual int tryacquire (void);

  /// Acquire the lock.
  virtual int release (void);

  /// Release the lock.
  virtual int acquire_read (void);

  /// Release the lock.
  virtual int acquire_write (void);

  /// Release the lock.
  virtual int tryacquire_read (void);

  /// Release the lock.
  virtual int tryacquire_write (void);

  /// Release the lock.
  virtual int tryacquire_write_upgrade (void);

  /// Explicitly destroy the lock.
  virtual int remove (void);

private:
  /// The concrete locking mechanism that all the methods delegate to.
  ACE_LOCKING_MECHANISM &lock_;

  /// This indicates what kind of acquire method will be called.
  ACE_Acquire_Method::METHOD_TYPE acquire_method_;
};

/**
 * @class ACE_TSS
 *
 * @brief Allows objects that are "physically" in thread specific
 * storage (i.e., private to a thread) to be accessed as though
 * they were "logically" global to a program.
 *
 * This class is a wrapper around the OS thread library
 * thread-specific functions.  It uses the <C++ operator->> to
 * shield applications from the details of accessing
 * thread-specific storage.
 *
 * NOTE: For maximal portability, <TYPE> cannot be a built-in type,
 * but instead should be a user-defined class (some compilers will
 * allow a built-in type, others won't).  See template class
 * ACE_TSS_Type_Adapter, below, for adapting built-in types to work
 * with ACE_TSS.
 */
template <class TYPE>
class ACE_TSS
{
public:
  // = Initialization and termination methods.

  /**
   * If caller has passed us a non-NULL ts_obj *, then we'll just use
   * this to initialize the thread-specific value (but only for the
   * calling thread).  Thus, subsequent calls to <operator->> in this
   * thread will return this value.  This is useful since it enables
   * us to assign objects to thread-specific data that have
   * arbitrarily complex constructors.
   */
  ACE_TSS (TYPE *ts_obj = 0);

  /// Deregister with thread-key administration.
  virtual ~ACE_TSS (void);

  // = Accessors.

  /**
   * Get the thread-specific object for the key associated with this
   * object.  Returns 0 if the data has never been initialized,
   * otherwise returns a pointer to the data.
   */
  TYPE *ts_object (void) const;

  /// Set the thread-specific object for the key associated with this
  /// object.
  TYPE *ts_object (TYPE *);

  /// Use a "smart pointer" to get the thread-specific object
  /// associated with the <key_>.
  TYPE *operator-> () const;

  /// Return or create and return the calling threads TYPE object.
  operator TYPE *(void) const;

  /// Hook for construction parameters.
  virtual TYPE *make_TSS_TYPE (void) const;

  // = Utility methods.

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  /// Actually implements the code that retrieves the object from
  /// thread-specific storage.
  TYPE *ts_get (void) const;

  /// Factors out common code for initializing TSS.  This must NOT be
  /// called with the lock held...
  int ts_init (void) const;

#if !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
  /// This implementation only works for non-threading systems...
  TYPE *type_;
#else
  /// Avoid race conditions during initialization.
  ACE_Thread_Mutex keylock_;

  /// "First time in" flag.
  int once_;

  /// Key for the thread-specific error data.
  ACE_thread_key_t key_;

  /// "Destructor" that deletes internal TYPE * when thread exits.
  static void cleanup (void *ptr);
#endif /* defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) */
  // = Disallow copying...
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_TSS<TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_TSS (const ACE_TSS<TYPE> &))
};

/**
 * @class ACE_TSS_Type_Adapter
 *
 * @brief Adapter that allows built-in types to be used with ACE_TSS.
 *
 * Wraps a value of a built-in type, providing conversions to
 * and from the type.  Example use with ACE_TSS:
 * ACE_TSS<ACE_TSS_Type_Adapter<int> > i;
 * *i = 37;
 * ACE_OS::fprintf (stderr, "%d\n", *i);
 * Unfortunately, though, some compilers have trouble with the
 * implicit type conversions.  This seems to work better:
 * ACE_TSS<ACE_TSS_Type_Adapter<int> > i;
 * i->operator int & () = 37;
 * ACE_OS::fprintf (stderr, "%d\n", i->operator int ());
 */
template <class TYPE>
class ACE_TSS_Type_Adapter
{
public:
  /// Constructor.  Inlined here so that it should _always_ be inlined.
  ACE_TSS_Type_Adapter (const TYPE value = 0): value_ (value) {}

  /// TYPE conversion.  Inlined here so that it should _always_ be
  /// inlined.
  operator TYPE () const { return value_; };

  /// TYPE & conversion.  Inlined here so that it should _always_ be
  /// inlined.
  operator TYPE &() { return value_; };

private:
  /// The wrapped value.
  TYPE value_;
};

/**
 * @class ACE_Guard
 *
 * @brief This data structure is meant to be used within a method or
 * function...  It performs automatic aquisition and release of
 * a parameterized synchronization object <ACE_LOCK>.
 *
 * The <ACE_LOCK> class given as an actual parameter must provide at
 * the very least the <acquire>, <tryacquire>, <release>, and
 * <remove> methods.
 */
template <class ACE_LOCK>
class ACE_Guard
{
public:

  // = Initialization and termination methods.
  ACE_Guard (ACE_LOCK &l);

  /// Implicitly and automatically acquire (or try to acquire) the
  /// lock.  If <block> is non-0 then <acquire> the <ACE_LOCK>, else
  /// <tryacquire> it.
  ACE_Guard (ACE_LOCK &l, int block);

  /// Initialise the guard without implicitly acquiring the lock. The
  /// <become_owner> parameter indicates whether the guard should release
  /// the lock implicitly on destruction. The <block> parameter is
  /// ignored and is used here to disambiguate with the preceding
  /// constructor.
  ACE_Guard (ACE_LOCK &l, int block, int become_owner);

  /// Implicitly release the lock.
  ~ACE_Guard (void);

  // = Lock accessors.

  /// Explicitly acquire the lock.
  int acquire (void);

  /// Conditionally acquire the lock (i.e., won't block).
  int tryacquire (void);

  /// Explicitly release the lock, but only if it is held!
  int release (void);

  /// Relinquish ownership of the lock so that it is not released
  /// implicitly in the destructor.
  void disown (void);

  // = Utility methods.
  /// 1 if locked, 0 if couldn't acquire the lock
  /// (errno will contain the reason for this).
  int locked (void) const;

  /// Explicitly remove the lock.
  int remove (void);

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  /// Helper, meant for subclass only.
  ACE_Guard (ACE_LOCK *lock): lock_ (lock) {}

  /// Pointer to the ACE_LOCK we're guarding.
  ACE_LOCK *lock_;

  /// Keeps track of whether we acquired the lock or failed.
  int owner_;

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Guard<ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Guard (const ACE_Guard<ACE_LOCK> &))
};

/**
 * @class ACE_Write_Guard
 *
 * @brief This class is similar to class <ACE_Guard>, though it
 * acquires/releases a write lock automatically (naturally, the
 * <ACE_LOCK> it is instantiated with must support the appropriate
 * API).
 */
template <class ACE_LOCK>
class ACE_Write_Guard : public ACE_Guard<ACE_LOCK>
{
public:
  // = Initialization method.

  /// Implicitly and automatically acquire a write lock.
  ACE_Write_Guard (ACE_LOCK &m);

  /// Implicitly and automatically acquire (or try to acquire) a write
  /// lock.
  ACE_Write_Guard (ACE_LOCK &m, int block);

  // = Lock accessors.

  /// Explicitly acquire the write lock.
  int acquire_write (void);

  /// Explicitly acquire the write lock.
  int acquire (void);

  /// Conditionally acquire the write lock (i.e., won't block).
  int tryacquire_write (void);

  /// Conditionally acquire the write lock (i.e., won't block).
  int tryacquire (void);

  // = Utility methods.

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

/**
 * @class ACE_Read_Guard
 *
 * @brief This class is similar to class <ACE_Guard>, though it
 * acquires/releases a read lock automatically (naturally, the
 * <ACE_LOCK> it is instantiated with must support the appropriate
 * API).
 */
template <class ACE_LOCK>
class ACE_Read_Guard : public ACE_Guard<ACE_LOCK>
{
public:
  // = Initialization methods.

  /// Implicitly and automatically acquire a read lock.
  ACE_Read_Guard (ACE_LOCK& m);

  /// Implicitly and automatically acquire (or try to acquire) a read
  /// lock.
  ACE_Read_Guard (ACE_LOCK &m, int block);

  // = Lock accessors.

  /// Explicitly acquire the read lock.
  int acquire_read (void);

  /// Explicitly acquire the read lock.
  int acquire (void);

  /// Conditionally acquire the read lock (i.e., won't block).
  int tryacquire_read (void);

  /// Conditionally acquire the read lock (i.e., won't block).
  int tryacquire (void);

  // = Utility methods.

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))

#define ACE_TSS_Guard ACE_Guard
#define ACE_TSS_Write_GUARD ACE_Write_Guard
#define ACE_TSS_Read_GUARD ACE_Read_Guard

#else
 /* ACE platform supports some form of threading and
  thread-specific storage. */

/**
 * @class ACE_TSS_Guard
 *
 * @brief This data structure is meant to be used within a method or
 * function...  It performs automatic aquisition and release of
 * a synchronization object.  Moreover, it ensures that the lock
 * is released even if a thread exits via <thr_exit>!
 */
template <class ACE_LOCK>
class ACE_TSS_Guard
{
public:
  // = Initialization and termination methods.

  /// Implicitly and automatically acquire the thread-specific lock.
  ACE_TSS_Guard (ACE_LOCK &lock, int block = 1);

  /// Implicitly release the thread-specific lock.
  ~ACE_TSS_Guard (void);

  // = Lock accessors.

  /// Explicitly acquire the thread-specific lock.
  int acquire (void);

  /// Conditionally acquire the thread-specific lock (i.e., won't
  /// block).
  int tryacquire (void);

  /// Explicitly release the thread-specific lock.
  int release (void);

  // = Utility methods.
  /// Explicitly release the thread-specific lock.
  int remove (void);

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  /// Helper, meant for subclass only.
  ACE_TSS_Guard (void);

  /// Initialize the key.
  void init_key (void);

  /// Called when thread exits to clean up the lock.
  static void cleanup (void *ptr);

  /// Thread-specific key...
  ACE_thread_key_t key_;

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_TSS_Guard<ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_TSS_Guard (const ACE_TSS_Guard<ACE_LOCK> &))
};

/**
 * @class ACE_TSS_Write_Guard
 *
 * @brief This class is similar to class ACE_TSS_Guard, though it
 * acquires/releases a write-lock automatically (naturally, the
 * ACE_LOCK it is instantiated with must support the appropriate
 * API).
 */
template <class ACE_LOCK>
class ACE_TSS_Write_Guard : public ACE_TSS_Guard<ACE_LOCK>
{
public:
  // = Initialization method.

  /// Implicitly and automatically acquire the thread-specific write lock.
  ACE_TSS_Write_Guard (ACE_LOCK &lock, int block = 1);

  // = Lock accessors.

  /// Explicitly acquire the thread-specific write lock.
  int acquire_write (void);

  /// Explicitly acquire the thread-specific write lock.
  int acquire (void);

  /// Conditionally acquire the thread-specific write lock (i.e., won't block).
  int tryacquire_write (void);

  /// Conditionally acquire the thread-specific write lock (i.e., won't block).
  int tryacquire (void);

  // = Utility methods.

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

/**
 * @class ACE_TSS_Read_Guard
 *
 * @brief This class is similar to class <ACE_TSS_Guard>, though it
 * acquires/releases a read lock automatically (naturally, the
 * <ACE_LOCK> it is instantiated with must support the
 * appropriate API).
 */
template <class ACE_LOCK>
class ACE_TSS_Read_Guard : public ACE_TSS_Guard<ACE_LOCK>
{
public:
  // = Initialization method.
  /// Implicitly and automatically acquire the thread-specific read lock.
  ACE_TSS_Read_Guard (ACE_LOCK &lock, int block = 1);

  // = Lock accessors.
  /// Explicitly acquire the thread-specific read lock.
  int acquire_read (void);

  /// Explicitly acquire the thread-specific read lock.
  int acquire (void);

  /// Conditionally acquire the thread-specific read lock (i.e., won't
  /// block).
  int tryacquire_read (void);

  /// Conditionally acquire the thread-specific read lock (i.e., won't
  /// block).
  int tryacquire (void);

  // = Utility methods.
  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};
#endif /* !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))) */

#if defined (ACE_HAS_THREADS) /* ACE platform supports some form of threading. */

/**
 * @class ACE_Condition
 *
 * @brief ACE_Condition variable wrapper, which allows threads to block
 * until shared data changes state.
 *
 * A condition variable enables threads to atomically block and
 * test the condition under the protection of a mutual exclu-
 * sion lock (mutex) until the condition is satisfied.  That is,
 * the mutex must have been held by the thread before calling
 * wait or signal on the condition.  If the condition is false,
 * a thread blocks on a condition variable and atomically
 * releases the mutex that is waiting for the condition to
 * change.  If another thread changes the condition, it may wake
 * up waiting threads by signaling the associated condition
 * variable.  The waiting threads, upon awakening, reacquire the
 * mutex and re-evaluate the condition.
 * Note, you can only parameterize <ACE_Condition> with
 * <ACE_Thread_Mutex> or <ACE_Null_Mutex>.
 */
template <class MUTEX>
class ACE_Condition
{
public:
  // = Initialiation and termination methods.
  /// Initialize the condition variable.
  ACE_Condition (MUTEX &m, int type = USYNC_THREAD,
                 const ACE_TCHAR *name = 0, void *arg = 0);

  /// Implicitly destroy the condition variable.
  ~ACE_Condition (void);

  // = Lock accessors.
  /**
   * Block on condition, or until absolute time-of-day has passed.  If
   * abstime == 0 use "blocking" <wait> semantics.  Else, if <abstime>
   * != 0 and the call times out before the condition is signaled
   * <wait> returns -1 and sets errno to ETIME.
   */
  int wait (const ACE_Time_Value *abstime);

  /// Block on condition.
  int wait (void);

  /**
   * Block on condition or until absolute time-of-day has passed.  If
   * abstime == 0 use "blocking" wait() semantics on the <mutex>
   * passed as a parameter (this is useful if you need to store the
   * <Condition> in shared memory).  Else, if <abstime> != 0 and the
   * call times out before the condition is signaled <wait> returns -1
   * and sets errno to ETIME.
   */
  int wait (MUTEX &mutex, const ACE_Time_Value *abstime = 0);

  /// Signal one waiting thread.
  int signal (void);

  /// Signal *all* waiting threads.
  int broadcast (void);

  // = Utility methods.
  /// Explicitly destroy the condition variable.
  int remove (void);

  /// Returns a reference to the underlying mutex_;
  MUTEX &mutex (void);

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
#if defined (CHORUS)
  /// This condition resides in shared memory.
  ACE_cond_t *process_cond_;

  /**
   * Remember the name of the condition if we created it so we can
   * unlink it when we go away (only the actor that initialized the
   * memory can destroy it).
   */
  const ACE_TCHAR *condname_;
#endif /* CHORUS */

  /// Condition variable.
  ACE_cond_t cond_;

  /// Reference to mutex lock.
  MUTEX &mutex_;

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Condition<MUTEX> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Condition (const ACE_Condition<MUTEX> &))
};

/**
 * @class ACE_Thread_Condition
 *
 * @brief ACE_Condition variable wrapper that works within processes.
 *
 * A condition variable enables threads to atomically block and
 * test the condition under the protection of a mutual exclu-
 * sion lock (mutex) until the condition is satisfied.  That is,
 * the mutex must have been held by the thread before calling
 * wait or signal on the condition.  If the condition is false,
 * a thread blocks on a condition variable and atomically
 * releases the mutex that is waiting for the condition to
 * change.  If another thread changes the condition, it may wake
 * up waiting threads by signaling the associated condition
 * variable.  The waiting threads, upon awakening, reacquire the
 * mutex and re-evaluate the condition.
 */
template <class MUTEX>
class ACE_Thread_Condition : public ACE_Condition<MUTEX>
{
public:
  // = Initialization method.
  ACE_Thread_Condition (MUTEX &m, const ACE_TCHAR *name = 0, void *arg = 0);

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#endif /* ACE_HAS_THREADS */

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

/**
 * @class ACE_NULL_SYNCH
 *
 * @brief Implement a do nothing Synchronization wrapper that
 * typedefs the <ACE_Condition> and <ACE_Mutex> to the Null* versions.
 */
class ACE_Export ACE_NULL_SYNCH
{
public:
  typedef ACE_Null_Mutex MUTEX;
  typedef ACE_Null_Mutex NULL_MUTEX;
  typedef ACE_Null_Mutex PROCESS_MUTEX;
  typedef ACE_Null_Mutex RECURSIVE_MUTEX;
  typedef ACE_Null_Mutex RW_MUTEX;
  typedef ACE_Null_Condition CONDITION;
  typedef ACE_Null_Semaphore SEMAPHORE;
  typedef ACE_Null_Mutex NULL_SEMAPHORE;
};

#if defined (ACE_HAS_THREADS)

class ACE_Process_Mutex;

/**
 * @class ACE_MT_SYNCH
 *
 * @brief Implement a default thread safe synchronization wrapper that
 * typedefs the <ACE_Condition> and <ACE_Mutex> to the
 * <ACE_Condition> and <ACE_Mutex> versions.  Note that this
 * should be a template, but SunC++ 4.0.1 complains about
 * this...
 */
class ACE_Export ACE_MT_SYNCH
{
public:
  typedef ACE_Thread_Mutex MUTEX;
  typedef ACE_Null_Mutex NULL_MUTEX;
  typedef ACE_Process_Mutex PROCESS_MUTEX;
  typedef ACE_Recursive_Thread_Mutex RECURSIVE_MUTEX;
  typedef ACE_RW_Thread_Mutex RW_MUTEX;
  typedef ACE_Condition_Thread_Mutex CONDITION;
  typedef ACE_Thread_Semaphore SEMAPHORE;
  typedef ACE_Null_Semaphore NULL_SEMAPHORE;
};

#endif /* ACE_HAS_THREADS */

#define ACE_SYNCH_MUTEX ACE_SYNCH::MUTEX
#define ACE_SYNCH_NULL_MUTEX ACE_SYNCH::NULL_MUTEX
#define ACE_SYNCH_RECURSIVE_MUTEX ACE_SYNCH::RECURSIVE_MUTEX
#define ACE_SYNCH_RW_MUTEX ACE_SYNCH::RW_MUTEX
#define ACE_SYNCH_CONDITION ACE_SYNCH::CONDITION
#define ACE_SYNCH_NULL_SEMAPHORE ACE_SYNCH::NULL_SEMAPHORE
#define ACE_SYNCH_SEMAPHORE ACE_SYNCH::SEMAPHORE

#else /* !ACE_HAS_TEMPLATE_TYPEDEFS */

#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
#define ACE_NULL_SYNCH ACE_Null_Mutex, ACE_Null_Condition, ACE_Null_Mutex
#define ACE_MT_SYNCH ACE_Thread_Mutex, ACE_Condition_Thread_Mutex, ACE_Thread_Semaphore
#else
#define ACE_NULL_SYNCH ACE_Null_Mutex, ACE_Null_Condition
#define ACE_MT_SYNCH ACE_Thread_Mutex, ACE_Condition_Thread_Mutex
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */

#if defined (ACE_HAS_THREADS)

#define ACE_SYNCH_MUTEX ACE_Thread_Mutex
#define ACE_SYNCH_NULL_MUTEX  ACE_Null_Mutex
#define ACE_SYNCH_RECURSIVE_MUTEX ACE_Recursive_Thread_Mutex
#define ACE_SYNCH_RW_MUTEX ACE_RW_Thread_Mutex
#define ACE_SYNCH_CONDITION ACE_Condition_Thread_Mutex
#define ACE_SYNCH_SEMAPHORE ACE_Thread_Semaphore
#define ACE_SYNCH_NULL_SEMAPHORE  ACE_Null_Semaphore

#else /* ACE_HAS_THREADS */

#define ACE_SYNCH_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_NULL_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_RECURSIVE_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_RW_MUTEX ACE_Null_Mutex
#define ACE_SYNCH_CONDITION ACE_Null_Condition
#define ACE_SYNCH_SEMAPHORE ACE_Null_Semaphore
#define ACE_SYNCH_NULL_SEMAPHORE ACE_Null_Mutex

#endif /* ACE_HAS_THREADS */
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

// These are available on *all* platforms
#define ACE_SYNCH_PROCESS_SEMAPHORE ACE_Process_Semaphore
#define ACE_SYNCH_PROCESS_MUTEX  ACE_Process_Mutex

#if defined (ACE_HAS_THREADS)
#define ACE_SYNCH ACE_MT_SYNCH
#else /* ACE_HAS_THREADS */
#define ACE_SYNCH ACE_NULL_SYNCH
#endif /* ACE_HAS_THREADS */

#if defined (__ACE_INLINE__)
#include "ace/Threads/Synch_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Threads/Synch_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Synch_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_SYNCH_T_H */
