// Synch_T.cpp
// $Id$

#if !defined (ACE_SYNCH_T_C)
#define ACE_SYNCH_T_C

#define ACE_BUILD_DLL
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Synch_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Synch_T.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Atomic_Op)

template <class LOCK, class TYPE>
ACE_Test_and_Set<LOCK, TYPE>::ACE_Test_and_Set (TYPE initial_value)
  : is_set_ (initial_value)
{
}

// Returns true if we are done, else false.
template <class LOCK, class TYPE> TYPE 
ACE_Test_and_Set<LOCK, TYPE>::is_set (void) const
{
  ACE_GUARD_RETURN (LOCK, ace_mon, (LOCK &) this->lock_, this->is_set_);
  return this->is_set_;
}

// Sets the <is_set_> status.
template <class LOCK, class TYPE> TYPE
ACE_Test_and_Set<LOCK, TYPE>::set (TYPE status)
{
  ACE_GUARD_RETURN (LOCK, ace_mon, this->lock_, this->is_set_);
  TYPE o_status = this->is_set_;
  this->is_set_ = status;
  return o_status;
}

template <class LOCK, class TYPE> int 
ACE_Test_and_Set<LOCK, TYPE>::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  this->set (1);
  return 0;
}

template <class LOCK, class TYPE> void
ACE_Atomic_Op<LOCK, TYPE>::dump (void) const
{
// ACE_TRACE ("ACE_Atomic_Op<LOCK, TYPE>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class LOCK, class TYPE>
ACE_Atomic_Op<LOCK, TYPE>::ACE_Atomic_Op (void)
  : value_ (0)
{ 
// ACE_TRACE ("ACE_Atomic_Op<LOCK, TYPE>::ACE_Atomic_Op");
}

template <class LOCK, class TYPE>
ACE_Atomic_Op<LOCK, TYPE>::ACE_Atomic_Op (TYPE c) 
{ 
// ACE_TRACE ("ACE_Atomic_Op<LOCK, TYPE>::ACE_Atomic_Op");
  this->value_ = c; 
}

ACE_ALLOC_HOOK_DEFINE(ACE_Guard)

template <class LOCK> void
ACE_Guard<LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_Guard<LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "lock_ = %x\n", this->lock_));
  ACE_DEBUG ((LM_DEBUG, "owner_ = %d\n", this->owner_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Write_Guard)

template <class LOCK> void
ACE_Write_Guard<LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_Write_Guard<LOCK>::dump");
  ACE_Guard<LOCK>::dump ();
}

ACE_ALLOC_HOOK_DEFINE(ACE_Read_Guard)

template <class LOCK> void
ACE_Read_Guard<LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_Read_Guard<LOCK>::dump");
  ACE_Guard<LOCK>::dump ();
}

#if defined (ACE_HAS_THREADS)

#if defined (__osf__) && ! defined (__GNUG__)
#pragma define_template ACE_Condition<ACE_Mutex>
#endif

ACE_ALLOC_HOOK_DEFINE(ACE_Condition)

template <class MUTEX> void
ACE_Condition<MUTEX>::dump (void) const
{
// ACE_TRACE ("ACE_Condition<MUTEX>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
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

template <class MUTEX>
ACE_Condition<MUTEX>::ACE_Condition (MUTEX &m, 
				     int type, 
				     LPCTSTR name,
				     void *arg)
  : mutex_ (m)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::ACE_Condition");
  if (ACE_OS::cond_init (&this->cond_, type, name, arg) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Condition::ACE_Condition"));
}

template <class MUTEX>
ACE_Condition<MUTEX>::~ACE_Condition (void)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::~ACE_Condition");
  if (this->remove () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Condition::~ACE_Condition"));
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
#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->keylock_.dump ();
  ACE_DEBUG ((LM_DEBUG, "key_ = %d\n", this->key_));
  ACE_DEBUG ((LM_DEBUG, "\nonce_ = %d", this->once_));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) */
}

#if !(defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE))
template <class TYPE> ACE_INLINE
ACE_TSS<TYPE>::ACE_TSS (TYPE *)
{
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_object (void) const
{ 
  return (TYPE *) &this->type_;
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_object (TYPE *) 
{ 
  return &this->type_;
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_get (void) const
{ 
  return (TYPE *) &this->type_;
}
#else
template <class TYPE> 
ACE_TSS<TYPE>::ACE_TSS (TYPE *ts_obj)
  : once_ (0),
    key_ (0)    
{
  // If caller has passed us a non-NULL TYPE *, then we'll just use
  // this to initialize the thread-specific value.  Thus, subsequent
  // calls to operator->() will return this value.  This is useful
  // since it enables us to assign objects to thread-specific data
  // that have arbitrarily complex constructors!

  if (ts_obj != 0)
    {
      ACE_ASSERT (this->once_ == 0);

      if (ACE_Thread::keycreate (&this->key_,
				 &ACE_TSS<TYPE>::cleanup,
				 (void *) this) != 0)
	{
	  int errnum = errno;
	  // What should we do if this call fails?!
	  ACE_OS::fprintf (stderr, "ACE_Thread::keycreate() failed!");
	  errno = errnum;
	  return;
	}

      this->once_ = 1;

      if (ACE_Thread::setspecific (this->key_, (void *) ts_obj) != 0)
	ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Thread::setspecific() failed!"));
    }
}

template <class TYPE> TYPE *
ACE_TSS<TYPE>::ts_get (void) const
{ 
  // Create and initialize thread-specific ts_obj.
  if (this->once_ == 0)
    {
      // Insure that we are serialized!
      ACE_GUARD_RETURN (ACE_Mutex, ace_mon_2, (ACE_Mutex &) this->keylock_, 0);

      // Use the Double-Check pattern to make sure we only create the
      // key once!
      if (this->once_ == 0)
	{
	  if (ACE_Thread::keycreate ((ACE_thread_key_t *) &this->key_,
					&ACE_TSS<TYPE>::cleanup,
					(void *) this) != 0)
	    return 0; // Major problems, this should *never* happen!
	  else
	    // This *must* come last to avoid race conditions!  Note
	    // that we need to "cast away const..."
	    *(int *) &this->once_ = 1;
	}
    }

  TYPE *ts_obj = 0;

  // Get the ts_obj from thread-specific storage.  Note that no locks
  // are required here...
  if (ACE_Thread::getspecific (this->key_, (void **) &ts_obj) == -1)
    return 0; // This should not happen!

  // Check to see if this is the first time in for this thread.
  if (ts_obj == 0)
    {
      // Allocate memory off the heap and store it in a pointer in
      // thread-specific storage (on the stack...).

      ts_obj = this->make_TSS_TYPE ();
	
      if (ts_obj == 0)
	return 0;

      // Store the dynamically allocated pointer in thread-specific
      // storage.
      if (ACE_Thread::setspecific (this->key_, (void *) ts_obj) != 0)
	{
	  delete ts_obj;
	  return 0; // Major problems, this should *never* happen!
	}
    }

  return ts_obj;
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

      if (ACE_Thread::getspecific (this->key_, (void **) &ts_obj) == -1)
	return 0; // This should not happen!
      else
	return ts_obj;
    }
}

template <class TYPE> TYPE *
ACE_TSS<TYPE>::ts_object (TYPE *new_ts_obj) 
{
  // Ensure that we are serialized!
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->keylock_, 0);

  if (this->once_ == 0) // Return 0 if we've never been initialized.
    return 0;
  else
    {
      TYPE *ts_obj = 0;

      if (ACE_Thread::getspecific (this->key_, (void **) &ts_obj) == -1)
	return 0; // This should not happen!
      if (ACE_Thread::setspecific (this->key_, (void *) new_ts_obj) == -1)
	return ts_obj; // This should not happen!
      else
	return ts_obj;
    }
}

/* static */
template <class TYPE> void
ACE_TSS<TYPE>::cleanup (void *ptr)
{
  // This cast is necessary to invoke the destructor (if necessary).
  delete (TYPE *) ptr;
}

ACE_ALLOC_HOOK_DEFINE(ACE_TSS_Guard)

template <class LOCK> void
ACE_TSS_Guard<LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "key_ = %d", this->key_));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class LOCK> void
ACE_TSS_Guard<LOCK>::init_key (void)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::init_key");

  this->key_ = 0;
  ACE_Thread::keycreate (&this->key_, 
			 &ACE_TSS_Guard<LOCK>::cleanup,
			 (void *) this);
}

template <class LOCK> 
ACE_TSS_Guard<LOCK>::ACE_TSS_Guard (void)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::ACE_TSS_Guard");
  this->init_key ();
}

template <class LOCK> int
ACE_TSS_Guard<LOCK>::release (void)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::release");

  ACE_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->release ();
}

template <class LOCK> int
ACE_TSS_Guard<LOCK>::remove (void)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::remove");

  ACE_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->remove ();
}

template <class LOCK> 
ACE_TSS_Guard<LOCK>::~ACE_TSS_Guard (void)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::~ACE_TSS_Guard");

  ACE_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  // Make sure that this pointer is NULL when we shut down...
  ACE_Thread::setspecific (this->key_, 0);
  ACE_Thread::keyfree (this->key_);
  // Destructor releases lock.
  delete guard;
}

template <class LOCK> void
ACE_TSS_Guard<LOCK>::cleanup (void *ptr)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::dump");

  // Destructor releases lock.
  delete (ACE_Guard<LOCK> *) ptr;
}

template <class LOCK> 
ACE_TSS_Guard<LOCK>::ACE_TSS_Guard (LOCK &lock, int block)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::ACE_TSS_Guard");

  this->init_key ();
  ACE_Guard<LOCK> *guard;
  ACE_NEW (guard, ACE_Guard<LOCK> (lock, block));
  ACE_Thread::setspecific (this->key_, (void *) guard);
}

template <class LOCK> int
ACE_TSS_Guard<LOCK>::acquire (void)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::acquire");

  ACE_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->acquire ();
}

template <class LOCK> int
ACE_TSS_Guard<LOCK>::tryacquire (void)
{
// ACE_TRACE ("ACE_TSS_Guard<LOCK>::tryacquire");

  ACE_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->tryacquire ();
}

template <class LOCK> 
ACE_TSS_Write_Guard<LOCK>::ACE_TSS_Write_Guard (LOCK &lock, int block)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<LOCK>::ACE_TSS_Write_Guard");

  this->init_key ();
  ACE_Guard<LOCK> *guard;
  ACE_NEW (guard, ACE_Write_Guard<LOCK> (lock, block));
  ACE_Thread::setspecific (this->key_, (void *) guard);
}

template <class LOCK> int
ACE_TSS_Write_Guard<LOCK>::acquire (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<LOCK>::acquire");

  ACE_Write_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->acquire_write ();
}

template <class LOCK> int
ACE_TSS_Write_Guard<LOCK>::tryacquire (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<LOCK>::tryacquire");

  ACE_Write_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->tryacquire_write ();
}

template <class LOCK> int
ACE_TSS_Write_Guard<LOCK>::acquire_write (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<LOCK>::acquire_write");

  return this->acquire ();
}

template <class LOCK> int
ACE_TSS_Write_Guard<LOCK>::tryacquire_write (void)
{
// ACE_TRACE ("ACE_TSS_Write_Guard<LOCK>::tryacquire_write");

  return this->tryacquire ();
}

template <class LOCK> void
ACE_TSS_Write_Guard<LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_TSS_Write_Guard<LOCK>::dump");
  ACE_TSS_Guard<LOCK>::dump ();
}

template <class LOCK> 
ACE_TSS_Read_Guard<LOCK>::ACE_TSS_Read_Guard (LOCK &lock, int block)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<LOCK>::ACE_TSS_Read_Guard");

  this->init_key ();
  ACE_Guard<LOCK> *guard;
  ACE_NEW (guard, ACE_Read_Guard<LOCK> (lock, block));
  ACE_Thread::setspecific (this->key_, (void *) guard);
}

template <class LOCK> int
ACE_TSS_Read_Guard<LOCK>::acquire (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<LOCK>::acquire");

  ACE_Read_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->acquire_read ();
}

template <class LOCK> int
ACE_TSS_Read_Guard<LOCK>::tryacquire (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<LOCK>::tryacquire");

  ACE_Read_Guard<LOCK> *guard = 0;
  ACE_Thread::getspecific (this->key_, (void **) &guard);
  return guard->tryacquire_read ();
}

template <class LOCK> int
ACE_TSS_Read_Guard<LOCK>::acquire_read (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<LOCK>::acquire_read");

  return this->acquire ();
}

template <class LOCK> int
ACE_TSS_Read_Guard<LOCK>::tryacquire_read (void)
{
// ACE_TRACE ("ACE_TSS_Read_Guard<LOCK>::tryacquire_read");

  return this->tryacquire ();
}

template <class LOCK> void
ACE_TSS_Read_Guard<LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_TSS_Read_Guard<LOCK>::dump");
  ACE_TSS_Guard<LOCK>::dump ();
}

#endif /* defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) */
#endif /* ACE_SYNCH_T_C */
