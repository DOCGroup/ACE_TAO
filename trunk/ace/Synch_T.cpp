// $Id$

#if !defined (ACE_SYNCH_T_C)
#define ACE_SYNCH_T_C

#define ACE_BUILD_DLL
#include "ace/Thread.h"
#include "ace/Synch_T.h"
#include "ace/Log_Msg.h"

ACE_RCSID(ace, Synch_T, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Synch_T.i"
// On non-Win32 platforms, this code will be treated as normal code.
#if !defined (ACE_WIN32)
#include "ace/Atomic_Op.i"
#endif /* !ACE_WIN32 */
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Atomic_Op)

template <class ACE_LOCK, class TYPE>
ACE_Test_and_Set<ACE_LOCK, TYPE>::ACE_Test_and_Set (TYPE initial_value)
  : is_set_ (initial_value)
{
}

// Returns true if we are done, else false.
template <class ACE_LOCK, class TYPE> TYPE
ACE_Test_and_Set<ACE_LOCK, TYPE>::is_set (void) const
{
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->lock_, this->is_set_);
  return this->is_set_;
}

// Sets the <is_set_> status.
template <class ACE_LOCK, class TYPE> TYPE
ACE_Test_and_Set<ACE_LOCK, TYPE>::set (TYPE status)
{
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, this->is_set_);
  TYPE o_status = this->is_set_;
  this->is_set_ = status;
  return o_status;
}

template <class ACE_LOCK, class TYPE> int
ACE_Test_and_Set<ACE_LOCK, TYPE>::handle_signal (int, siginfo_t *, ucontext_t *)
{
  // By setting this to 1, we are "signaling" to anyone calling
  // <is_set> or or <set> that the "test and set" object is in the
  // "signaled" state, i.e., it's "available" to be set back to 0.
  this->set (1);
  return 0;
}

template <class ACE_LOCK, class TYPE> ACE_LOCK &
ACE_Atomic_Op<ACE_LOCK, TYPE>::mutex (void)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::lock");
  return this->mutex_;
}

template <class ACE_LOCK, class TYPE> void
ACE_Atomic_Op<ACE_LOCK, TYPE>::dump (void) const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->mutex_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class ACE_LOCK, class TYPE>
ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op (void)
  : value_ (0)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op");
}

template <class ACE_LOCK, class TYPE>
ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op (const TYPE &c)
  : value_ (c)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Guard)

template <class ACE_LOCK> void
ACE_Guard<ACE_LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_Guard<ACE_LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("mutex_ = %x\n"), this->lock_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("owner_ = %d\n"), this->owner_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Write_Guard)

template <class ACE_LOCK> void
ACE_Write_Guard<ACE_LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_Write_Guard<ACE_LOCK>::dump");
  ACE_Guard<ACE_LOCK>::dump ();
}

ACE_ALLOC_HOOK_DEFINE(ACE_Read_Guard)

template <class ACE_LOCK> void
ACE_Read_Guard<ACE_LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_Read_Guard<ACE_LOCK>::dump");
  ACE_Guard<ACE_LOCK>::dump ();
}

#if defined (ACE_HAS_THREADS)

ACE_ALLOC_HOOK_DEFINE(ACE_Condition)

template <class MUTEX> void
ACE_Condition<MUTEX>::dump (void) const
{
// ACE_TRACE ("ACE_Condition<MUTEX>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class MUTEX>
ACE_Thread_Condition<MUTEX>::ACE_Thread_Condition (MUTEX &m,
                                                   LPCTSTR name,
                                                   void *arg)
  : ACE_Condition<MUTEX> (m, USYNC_THREAD, name, arg)
{
// ACE_TRACE ("ACE_Thread_Condition<MUTEX>::ACE_Thread_Condition");
}

template <class MUTEX> void
ACE_Thread_Condition<MUTEX>::dump (void) const
{
// ACE_TRACE ("ACE_Thread_Condition<MUTEX>::dump");

  ACE_Condition<MUTEX>::dump ();
}

template <class MUTEX>
ACE_Condition<MUTEX>::ACE_Condition (MUTEX &m,
                                     int type,
                                     LPCTSTR name,
                                     void *arg)
  : mutex_ (m)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::ACE_Condition");

  if (ACE_OS::cond_init (&this->cond_, type, name, arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Condition::ACE_Condition")));
}

template <class MUTEX>
ACE_Condition<MUTEX>::~ACE_Condition (void)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::~ACE_Condition");

  if (this->remove () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Condition::~ACE_Condition")));
}

template <class MUTEX> int
ACE_Condition<MUTEX>::wait (void)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::wait");
  return ACE_OS::cond_wait (&this->cond_, &this->mutex_.lock_);
}

template <class MUTEX> int
ACE_Condition<MUTEX>::wait (MUTEX &mutex,
                            const ACE_Time_Value *abstime)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::wait");
  if (abstime == 0)
    return this->wait ();
  else
    return ACE_OS::cond_timedwait (&this->cond_,
                                   &mutex.lock_,
                                   (ACE_Time_Value *) abstime);
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

ACE_ALLOC_HOOK_DEFINE(ACE_TSS)

template <class TYPE>
ACE_TSS<TYPE>::~ACE_TSS (void)
{
  // We can't call <ACE_OS::thr_keyfree> until *all* of the threads
  // that are using that key have done an <ACE_OS::thr_key_detach>.
  // Otherwise, we'll end up with "dangling TSS pointers."
  ACE_OS::thr_key_detach (this);
}

template <class TYPE> TYPE *
ACE_TSS<TYPE>::operator-> () const
{
  return this->ts_get ();
}

template <class TYPE>
ACE_TSS<TYPE>::operator TYPE *(void) const
{
  return this->ts_get ();
}

template <class TYPE> TYPE *
ACE_TSS<TYPE>::make_TSS_TYPE (void) const
{
  return new TYPE;
}

template <class TYPE> void
ACE_TSS<TYPE>::dump (void) const
{
// ACE_TRACE ("ACE_TSS<TYPE>::dump");
#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->keylock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("key_ = %d\n"), this->key_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nonce_ = %d"), this->once_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) */
}

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))
#if defined (ACE_HAS_THR_C_DEST)
extern "C" void ACE_TSS_C_cleanup(void *); // defined in Synch.cpp
#endif /* ACE_HAS_THR_C_DEST */

template <class TYPE> void
ACE_TSS<TYPE>::cleanup (void *ptr)
{
  // Cast this to the concrete TYPE * so the destructor gets called.
  delete (TYPE *) ptr;
}

template <class TYPE> int
ACE_TSS<TYPE>::ts_init (void) const
{
  // Insure that we are serialized!
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->keylock_, 0);

  // Use the Double-Check pattern to make sure we only create the key
  // once!
  if (this->once_ == 0)
    {
      if (ACE_Thread::keycreate (&this->key_,
#if defined (ACE_HAS_THR_C_DEST)
                                 &ACE_TSS_C_cleanup,
#else
                                 &ACE_TSS<TYPE>::cleanup,
#endif /* ACE_HAS_THR_C_DEST */
                                 (void *) this) != 0)
        return -1; // Major problems, this should *never* happen!
      else
        {
          // This *must* come last to avoid race conditions!  Note that
          // we need to "cast away const..."
          // BTW, if this code doesn't compile, please try the following instead. 
          // *(int *) &this->once_ = 1;
          ACE_const_cast (int, this->once_) = 1;
          return 0;
        }
    }
  else
    return -1;
}

template <class TYPE>
ACE_TSS<TYPE>::ACE_TSS (TYPE *ts_obj)
  : once_ (0),
    key_ (ACE_OS::NULL_key)
{
  // If caller has passed us a non-NULL TYPE *, then we'll just use
  // this to initialize the thread-specific value.  Thus, subsequent
  // calls to operator->() will return this value.  This is useful
  // since it enables us to assign objects to thread-specific data
  // that have arbitrarily complex constructors!

  if (ts_obj != 0)
    {
      if (this->ts_init () == -1)
        {
          int errnum = errno;
          // What should we do if this call fails?!
#if defined (ACE_HAS_WINCE)
          ::MessageBox (NULL, L"ACE_Thread::keycreate() failed!",
                        L"ACE_TSS::ACE_TSS", MB_OK);
#else
          ACE_OS::fprintf (stderr,
                           "ACE_Thread::keycreate() failed!");
#endif /* ACE_HAS_WINCE */
          errno = errnum;
          return;
        }

#if defined (ACE_HAS_THR_C_DEST)
      // Encapsulate a ts_obj and it's destructor in an
      // ACE_TSS_Adapter.
      ACE_TSS_Adapter *tss_adapter;
      ACE_NEW (tss_adapter,
               ACE_TSS_Adapter ((void *) ts_obj,
                                ACE_TSS<TYPE>::cleanup));

      // Put the adapter in thread specific storage
      if (ACE_Thread::setspecific (this->key_,
                                   (void *) tss_adapter) != 0)
        {
          delete tss_adapter;
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("%p\n"),
                      ASYS_TEXT ("ACE_Thread::setspecific() failed!")));
        }
#else
      if (ACE_Thread::setspecific (this->key_,
                                   (void *) ts_obj) != 0)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("%p\n"),
                    ASYS_TEXT ("ACE_Thread::setspecific() failed!")));
#endif /* ACE_HAS_THR_C_DEST */
    }
}

template <class TYPE> TYPE *
ACE_TSS<TYPE>::ts_get (void) const
{
  if (this->once_ == 0)
    // Create and initialize thread-specific ts_obj.
    this->ts_init ();

  TYPE *ts_obj = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;

  // Get the adapter from thread-specific storage
  if (ACE_Thread::getspecific (this->key_,
                               (void **) &tss_adapter) == -1)
    return 0; // This should not happen!

  // Check to see if this is the first time in for this thread.
  if (tss_adapter == 0)
#else
  // Get the ts_obj from thread-specific storage.  Note that no locks
  // are required here...
  if (ACE_Thread::getspecific (this->key_,
                               (void **) &ts_obj) == -1)
    return 0; // This should not happen!

  // Check to see if this is the first time in for this thread.
  if (ts_obj == 0)
#endif /* ACE_HAS_THR_C_DEST */
    {
      // Allocate memory off the heap and store it in a pointer in
      // thread-specific storage (on the stack...).

      ts_obj = this->make_TSS_TYPE ();

      if (ts_obj == 0)
        return 0;

#if defined (ACE_HAS_THR_C_DEST)
      // Encapsulate a ts_obj and it's destructor in an
      // ACE_TSS_Adapter.
      ACE_NEW_RETURN (tss_adapter,
                      ACE_TSS_Adapter (ts_obj,
                                       ACE_TSS<TYPE>::cleanup), 0);

      // Put the adapter in thread specific storage
      if (ACE_Thread::setspecific (this->key_,
                                   (void *) tss_adapter) != 0)
        {
          delete tss_adapter;
          delete ts_obj;
          return 0; // Major problems, this should *never* happen!
        }
#else
      // Store the dynamically allocated pointer in thread-specific
      // storage.
      if (ACE_Thread::setspecific (this->key_,
                                   (void *) ts_obj) != 0)
        {
          delete ts_obj;
          return 0; // Major problems, this should *never* happen!
        }
#endif /* ACE_HAS_THR_C_DEST */
    }

#if defined (ACE_HAS_THR_C_DEST)
  // Return the underlying ts object.
  return (TYPE *) tss_adapter->ts_obj_;   
#else
  return ts_obj;
#endif /* ACE_HAS_THR_C_DEST */
}

// Get the thread-specific object for the key associated with this
// object.  Returns 0 if the ts_obj has never been initialized,
// otherwise returns a pointer to the ts_obj.

template <class TYPE> TYPE *
ACE_TSS<TYPE>::ts_object (void) const
{
  // Ensure that we are serialized!
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->keylock_, 0);

  if (this->once_ == 0) // Return 0 if we've never been initialized.
    return 0;
  else
    {
      TYPE *ts_obj = 0;
#if defined (ACE_HAS_THR_C_DEST)
      ACE_TSS_Adapter *tss_adapter = 0;

      // Get the tss adapter from thread-specific storage
      if (ACE_Thread::getspecific (this->key_,
                                   (void **) &tss_adapter) == -1)
        return 0; // This should not happen!
      else if (tss_adapter != 0)
        // Extract the real TS object.
        ts_obj = (TYPE *) tss_adapter->ts_obj_;
#else
      if (ACE_Thread::getspecific (this->key_,
                                   (void **) &ts_obj) == -1)
        return 0; // This should not happen!
#endif /* ACE_HAS_THR_C_DEST */
      return ts_obj;
    }
}

template <class TYPE> TYPE *
ACE_TSS<TYPE>::ts_object (TYPE *new_ts_obj)
{
  // Ensure that we are serialized!
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->keylock_, 0);

  if (this->once_ == 0)
    // Create and initialize thread-specific ts_obj.
    this->ts_init ();

  TYPE *ts_obj = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;

  if (ACE_Thread::getspecific (this->key_,
                               (void **) &tss_adapter) == -1)
    return 0; // This should not happen!

  if (tss_adapter != 0)
    {
      ts_obj = (TYPE *) tss_adapter->ts_obj_;
      delete tss_adapter;       // don't need this anymore
    }

  ACE_NEW_RETURN (tss_adapter,
                  ACE_TSS_Adapter ((void *) new_ts_obj,
                                   ACE_TSS<TYPE>::cleanup),
                  0);

  if (ACE_Thread::setspecific (this->key_,
                               (void *) tss_adapter) == -1)
    {
      delete tss_adapter;
      return ts_obj; // This should not happen!
    }
#else
  if (ACE_Thread::getspecific (this->key_,
                               (void **) &ts_obj) == -1)
    return 0; // This should not happen!
  if (ACE_Thread::setspecific (this->key_,
                               (void *) new_ts_obj) == -1)
    return ts_obj; // This should not happen!
#endif /* ACE_HAS_THR_C_DEST */
  else
    return ts_obj;
}

ACE_ALLOC_HOOK_DEFINE(ACE_TSS_Guard)

template <class ACE_LOCK> void
ACE_TSS_Guard<ACE_LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("key_ = %d"), this->key_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class ACE_LOCK> void
ACE_TSS_Guard<ACE_LOCK>::init_key (void)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::init_key");

  this->key_ = ACE_OS::NULL_key;
  ACE_Thread::keycreate (&this->key_,
#if defined (ACE_HAS_THR_C_DEST)
                         &ACE_TSS_C_cleanup,
#else
                         &ACE_TSS_Guard<ACE_LOCK>::cleanup,
#endif /* ACE_HAS_THR_C_DEST */
                         (void *) this);
}

template <class ACE_LOCK>
ACE_TSS_Guard<ACE_LOCK>::ACE_TSS_Guard (void)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::ACE_TSS_Guard");
  this->init_key ();
}

template <class ACE_LOCK> int
ACE_TSS_Guard<ACE_LOCK>::release (void)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::release");

  ACE_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *)tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->release ();
}

template <class ACE_LOCK> int
ACE_TSS_Guard<ACE_LOCK>::remove (void)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::remove");

  ACE_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->remove ();
}

template <class ACE_LOCK>
ACE_TSS_Guard<ACE_LOCK>::~ACE_TSS_Guard (void)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::~ACE_TSS_Guard");

  ACE_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  // Make sure that this pointer is NULL when we shut down...
  ACE_Thread::setspecific (this->key_, 0);
  ACE_Thread::keyfree (this->key_);
  // Destructor releases lock.
  delete guard;
}

template <class ACE_LOCK> void
ACE_TSS_Guard<ACE_LOCK>::cleanup (void *ptr)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::dump");

  // Destructor releases lock.
  delete (ACE_Guard<ACE_LOCK> *) ptr;
}

template <class ACE_LOCK>
ACE_TSS_Guard<ACE_LOCK>::ACE_TSS_Guard (ACE_LOCK &lock, int block)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::ACE_TSS_Guard");

  this->init_key ();
  ACE_Guard<ACE_LOCK> *guard;
  ACE_NEW (guard, ACE_Guard<ACE_LOCK> (lock, block));

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter;
  ACE_NEW (tss_adapter,
           ACE_TSS_Adapter ((void *) guard,
                            ACE_TSS_Guard<ACE_LOCK>::cleanup));
  ACE_Thread::setspecific (this->key_,
                           (void *) tss_adapter);
#else
  ACE_Thread::setspecific (this->key_,
                           (void *) guard);
#endif /* ACE_HAS_THR_C_DEST */
}

template <class ACE_LOCK> int
ACE_TSS_Guard<ACE_LOCK>::acquire (void)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::acquire");

  ACE_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->acquire ();
}

template <class ACE_LOCK> int
ACE_TSS_Guard<ACE_LOCK>::tryacquire (void)
{
// ACE_TRACE ("ACE_TSS_Guard<ACE_LOCK>::tryacquire");

  ACE_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->tryacquire ();
}

template <class ACE_LOCK>
ACE_TSS_Write_Guard<ACE_LOCK>::ACE_TSS_Write_Guard (ACE_LOCK &lock,
                                                    int block)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<ACE_LOCK>::ACE_TSS_Write_Guard");

  this->init_key ();
  ACE_Guard<ACE_LOCK> *guard;
  ACE_NEW (guard, ACE_Write_Guard<ACE_LOCK> (lock, block));

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter;
  ACE_NEW (tss_adapter,
           ACE_TSS_Adapter ((void *) guard,
                            ACE_TSS_Guard<ACE_LOCK>::cleanup));
  ACE_Thread::setspecific (this->key_,
                           (void *) tss_adapter);
#else
  ACE_Thread::setspecific (this->key_,
                           (void *) guard);
#endif /* ACE_HAS_THR_C_DEST */
}

template <class ACE_LOCK> int
ACE_TSS_Write_Guard<ACE_LOCK>::acquire (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<ACE_LOCK>::acquire");

  ACE_Write_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->acquire_write ();
}

template <class ACE_LOCK> int
ACE_TSS_Write_Guard<ACE_LOCK>::tryacquire (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<ACE_LOCK>::tryacquire");

  ACE_Write_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->tryacquire_write ();
}

template <class ACE_LOCK> int
ACE_TSS_Write_Guard<ACE_LOCK>::acquire_write (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<ACE_LOCK>::acquire_write");

  return this->acquire ();
}

template <class ACE_LOCK> int
ACE_TSS_Write_Guard<ACE_LOCK>::tryacquire_write (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<ACE_LOCK>::tryacquire_write");

  return this->tryacquire ();
}

template <class ACE_LOCK> void
ACE_TSS_Write_Guard<ACE_LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_TSS_Write_Guard<ACE_LOCK>::dump");
  ACE_TSS_Guard<ACE_LOCK>::dump ();
}

template <class ACE_LOCK>
ACE_TSS_Read_Guard<ACE_LOCK>::ACE_TSS_Read_Guard (ACE_LOCK &lock, int block)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<ACE_LOCK>::ACE_TSS_Read_Guard");

  this->init_key ();
  ACE_Guard<ACE_LOCK> *guard;
  ACE_NEW (guard, ACE_Read_Guard<ACE_LOCK> (lock, block));

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter;
  ACE_NEW (tss_adapter,
           ACE_TSS_Adapter ((void *)guard,
                            ACE_TSS_Guard<ACE_LOCK>::cleanup));
  ACE_Thread::setspecific (this->key_,
                           (void *) tss_adapter);
#else
  ACE_Thread::setspecific (this->key_,
                           (void *) guard);
#endif /* ACE_HAS_THR_C_DEST */
}

template <class ACE_LOCK> int
ACE_TSS_Read_Guard<ACE_LOCK>::acquire (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<ACE_LOCK>::acquire");

  ACE_Read_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->acquire_read ();
}

template <class ACE_LOCK> int
ACE_TSS_Read_Guard<ACE_LOCK>::tryacquire (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<ACE_LOCK>::tryacquire");

  ACE_Read_Guard<ACE_LOCK> *guard = 0;

#if defined (ACE_HAS_THR_C_DEST)
  ACE_TSS_Adapter *tss_adapter = 0;
  ACE_Thread::getspecific (this->key_,
                           (void **) &tss_adapter);
  guard = (ACE_Guard<ACE_LOCK> *) tss_adapter->ts_obj_;
#else
  ACE_Thread::getspecific (this->key_,
                           (void **) &guard);
#endif /* ACE_HAS_THR_C_DEST */

  return guard->tryacquire_read ();
}

template <class ACE_LOCK> int
ACE_TSS_Read_Guard<ACE_LOCK>::acquire_read (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<ACE_LOCK>::acquire_read");

  return this->acquire ();
}

template <class ACE_LOCK> int
ACE_TSS_Read_Guard<ACE_LOCK>::tryacquire_read (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<ACE_LOCK>::tryacquire_read");

  return this->tryacquire ();
}

template <class ACE_LOCK> void
ACE_TSS_Read_Guard<ACE_LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_TSS_Read_Guard<ACE_LOCK>::dump");
  ACE_TSS_Guard<ACE_LOCK>::dump ();
}

#endif /* defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) */
#endif /* ACE_SYNCH_T_C */
