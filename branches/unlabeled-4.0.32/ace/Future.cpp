// Future.cpp
// $Id$

#define ACE_BUILD_DLL

#if !defined (ACE_FUTURE_CPP)
#define ACE_FUTURE_CPP

#include "ace/Future.h"

#if defined (ACE_HAS_THREADS)

// Dump the state of an object.

template <class T> void 
ACE_Future_Rep<T>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
	      "ref_count_ = %d\n",
	      this->ref_count_));
  ACE_DEBUG ((LM_INFO,"value_: \n"));
  if (this->value_)
    ACE_DEBUG ((LM_DEBUG," (NON-NULL)\n"));
  else
    ACE_DEBUG ((LM_DEBUG," (NULL)\n"));
  ACE_DEBUG ((LM_INFO,"value_ready_: \n"));
  this->value_ready_.dump ();
  ACE_DEBUG ((LM_INFO,"value_ready_mutex_: \n"));
  this->value_ready_mutex_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class T> 
ACE_Future_Rep<T>::ACE_Future_Rep (void)
  : ref_count_ (0),
    value_ (0),
    value_ready_ (this->value_ready_mutex_)
{
}

template <class T> 
ACE_Future_Rep<T>::~ACE_Future_Rep (void)
{
  delete this->value_;
  this->value_ = 0;
}

template <class T> int
ACE_Future_Rep<T>::ready (void)
{
  return this->value_ != 0;
}

template <class T> int
ACE_Future_Rep<T>::set (const T &r)
{
  // If the value is already produced, ignore it...
  if (this->value_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->value_ready_mutex_, -1));
      // Otherwise, create a new result value.  Note the use of the
      // Double-checked locking pattern to avoid multiple allocations.

      if (this->value_ == 0)
	ACE_NEW_RETURN (this->value_, T (r), -1);

      // Signal all the waiting threads.
      return this->value_ready_.broadcast ();

      // Destructor releases the lock.
    }
  return 0;
}

template <class T> int
ACE_Future_Rep<T>::get (T &value, ACE_Time_Value *tv)
{
  // If the value is already produced, return it.
  if (this->value_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->value_ready_mutex_, -1));

      // If the value is not yet defined we must block until the
      // producer writes to it.

      while (this->value_ == 0) 
	{
	  // Perform a timed wait.
	  if (this->value_ready_.wait (tv) == -1)
	    return -1;
	}
      // Destructor releases the lock.
    }

  value = *this->value_;
  return 0;
}

template <class T>
ACE_Future_Rep<T>::operator T ()
{
  // If the value is already produced, return it.
  if (this->value_ == 0)
    {
      // constructor of ace_mon acquires the mutex
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->value_ready_mutex_, 0));

      // If the value is not yet defined we must block until the
      // producer writes to it.

      while (this->value_ == 0) 
	{
	  // wait forever
	  if (this->value_ready_.wait () == -1)
	    return 0;
	}

      // Destructor releases the mutex

    }

  return *this->value_;
}


template <class T> 
ACE_Future<T>::ACE_Future (void)
  : future_rep_ (0)
{
}

template <class T>
ACE_Future<T>::ACE_Future (const ACE_Future<T> &r)
{

  // copy constructor:
  //
  // bind <this> to the same <ACE_Future_Rep> as <r>.

  // @@ not really clear if this is needed... after all this
  // ACE_Future is just being instantiated...
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_));

  // acquire the mutex on <r>. We have to make sure
  // that <r> does not delete its future_rep_...

  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, r_mon, (ACE_Thread_Mutex &) r.mutex_));

  // Check if 'r' has already a ACE_Future_rep bound to it.
  if (r.future_rep_ == 0) 
    this->future_rep_ = r.create_rep_i ();
  else 
    // ACE_Future_rep exists already, we can just link to it.
    this->future_rep_ = r.future_rep_;

  this->future_rep_->ref_count_++;

}

template <class T>
ACE_Future<T>::ACE_Future (const T &r)
{
  ACE_DEBUG ((LM_DEBUG," (%t) funny constructor\n"));
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_));
  this->create_rep_i ()->set (r);
}

template <class T> 
ACE_Future<T>::~ACE_Future (void)
{
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_));

  if (this->future_rep_)
    {
      this->future_rep_->ref_count_--;

      if (this->future_rep_->ref_count_ == 0)
	{
	  delete this->future_rep_;
	  this->future_rep_ = 0;
	}
    }

}

template <class T> int 
ACE_Future<T>::operator== (const ACE_Future<T> &r) const
{
  return r.future_rep_ == this->future_rep_;
}

template <class T> int 
ACE_Future<T>::operator!= (const ACE_Future<T> &r) const
{
  return r.future_rep_ != this->future_rep_;
}

template <class T> int
ACE_Future<T>::cancel (const T &r)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->mutex_, -1));

  // If this ACE_Future is already attached to a ACE_Future_Rep,
  // detach it (maybe delete the ACE_Future_Rep).
  if (this->future_rep_) 
    {
      this->future_rep_->ref_count_--;

      if (this->future_rep_->ref_count_ == 0)
	delete this->future_rep_;
    }

  // Make a new ACE_Future_Rep and immediately assign 
  // the new value to it.
  this->create_rep_i ();
  return this->future_rep_->set (r);
}

template <class T> int
ACE_Future<T>::set (const T &r) 
{
  if (this->future_rep_)
    // Give the pointer to the result to the ACE_Future_Rep.
    return this->future_rep_->set (r);
  else
    // @@ Maybe this should return a special value to indicate that
    // there's no <ACE_Future_Rep> yet?
    return 0; 
}

template <class T> ACE_Future_Rep<T> * 
ACE_Future<T>::create_rep_i (void) const
{
  // Should only be called internally with locks held.
  ACE_NEW_RETURN (((ACE_Future<T> *) this)->future_rep_, ACE_Future_Rep<T>, 0);
  this->future_rep_->ref_count_ = 1;
  return this->future_rep_;
}

template <class T> int
ACE_Future<T>::ready (void)
{
  // We're ready if the ACE_Future_rep is ready...
  if (this->future_rep_)
    return this->future_rep_->ready ();
  else
    return 0;
}

template <class T> int
ACE_Future<T>::get (T &value, ACE_Time_Value *tv)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->mutex_, -1));

  if (this->future_rep_ == 0) 
    // Oops, we have to create a ACE_Future_Rep first.
    this->create_rep_i ();

  // We return the ACE_Future_rep.
  return this->future_rep_->get (value, tv);
}

template <class T>
ACE_Future<T>::operator T ()
{
  // note that this will fail (and COREDUMP!) 
  // if future_rep_ == 0 !
  //
  // but...
  // this is impossible unless somebody is so stupid to
  // try something like this:
  //
  // Future<T> futT;
  // T t;
  // t = futT;

  // perform type conversion on Future_Rep.
  return *future_rep_;
}

template <class T> void
ACE_Future<T>::operator = (const ACE_Future<T> &r) 
{
  // assignment:
  //
  //  bind <this> to the same <ACE_Future_Rep> as <r>.

  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->mutex_));

  // if there is already a <ACE_Future_Rep> we have
  // to disconnect from it...
  if (this->future_rep_) 
    {
      // Disconnect from the <ACE_Future_Rep>.
      this->future_rep_->ref_count_--;

      if (this->future_rep_->ref_count_ == 0)
	delete this->future_rep_;
    }

  // Acquire the mutex on <r>. We have to make sure
  // that <r> does not delete it's future_rep_...

  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, r_mon, (ACE_Thread_Mutex &) r.mutex_));

  // Check if 'r' has already a ACE_Future_rep bound to it.
  if (r.future_rep_ == 0) 
    this->future_rep_ = r.create_rep_i ();
  else 
    // ACE_Future_rep exists already, we can just link to it.
    this->future_rep_ = r.future_rep_;

  this->future_rep_->ref_count_++;
}

template <class T> void 
ACE_Future<T>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  if (this->future_rep_)
    this->future_rep_->dump ();

  this->mutex_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class T> void *
ACE_Future<T>::operator new (size_t nbytes)
{
  return 0;
}

template <class T> void 
ACE_Future<T>::operator delete (void *)
{
}

template <class T> void
ACE_Future<T>::operator &()
{
}

#endif /* ACE_HAS_THREADS */
#endif /* ACE_FUTURE_CPP */
