// $Id$

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

/* In order to implement a thread pool, we have to have an object that
   can create a thread.  The ACE_Task<> is the basis for doing just
   such a thing.  */
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* We need a forward reference for ACE_Event_Handler so that our
   enqueue() method can accept a pointer to one.  */
class ACE_Event_Handler;

/* Although we modified the rest of our program to make use of the
   thread pool implementation, if you look closely you'll see that the
   changes were rather minor.  The "ACE way" is generally to create a
   helper object that abstracts away the details not relevant to your
   application.  That's what I'm trying to do here by creating the
   Thread_Pool object.  */
class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>
{
public:
  typedef ACE_Task<ACE_MT_SYNCH> inherited;

  /* Provide an enumeration for the default pool size.  By doing this,
    other objects can use the value when they want a default.  */
  enum size_t
  {
    default_pool_size_ = 5
  };

  // Basic constructor
  Thread_Pool (void);

  /* Opening the thread pool causes one or more threads to be
    activated.  When activated, they all execute the svc() method
    declared below.  */
  int open (int pool_size = default_pool_size_);

  /* Some compilers will complain that our open() above attempts to
    override a virtual function in the baseclass.  We have no
    intention of overriding that method but in order to keep the
    compiler quiet we have to add this method as a pass-thru to the
    baseclass method.  */
  virtual int open (void *void_data)
  {
    return inherited::open (void_data); 
  }

  /*
   */
  virtual int close (u_long flags = 0);

  /* To use the thread pool, you have to put some unit of work into
    it.  Since we're dealing with event handlers (or at least their
    derivatives), I've chosen to provide an enqueue() method that
    takes a pointer to an ACE_Event_Handler.  The handler's
    handle_input() method will be called, so your object has to know
    when it is being called by the thread pool.  */
  int enqueue (ACE_Event_Handler *handler);

  /* Another handy ACE template is ACE_Atomic_Op<>.  When
    parameterized, this allows is to have a thread-safe counting
    object.  The typical arithmetic operators are all internally
    thread-safe so that you can share it across threads without
    worrying about any contention issues.  */
  typedef ACE_Atomic_Op<ACE_Mutex, int> counter_t;

protected:

  /* Our svc() method will dequeue the enqueued event handler objects
    and invoke the handle_input() method on each.  Since we're likely
    running in more than one thread, idle threads can take work from
    the queue while other threads are busy executing handle_input() on
    some object.  */
  int svc (void);

  /* We use the atomic op to keep a count of the number of threads in
    which our svc() method is running.  This is particularly important
    when we want to close() it down!  */
  counter_t active_threads_;
};

#endif /* THREAD_POOL_H */
