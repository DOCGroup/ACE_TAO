// $Id$

#ifndef BARRIER_H
#define BARRIER_H

#include "ace/Synch.h"

/* Barrier is a simple wrapper for the ACE_Barrier synchronization
   class.  The ACE_Barrier is already pretty easy to use but I thought
   I'd wrap it up to create just a bit more abstraction at the
   application level.  */

class Barrier
{
public:
  // Basic constructor and destructor.  If you only need to synch the
  // start of your threads, you can safely delete your Barrier object
  // after invoking done().  Of course, you should be careful to only
  // delete the object once!
  Barrier (void);
  ~Barrier (void);

  // Set and get the number of threads that the barrier will manage.
  // If you add or remove threads to your application at run-time you
  // can use the mutator to reflect that change.  Note, however, that
  // you can only do that from the thread which first created the
  // Barrier.  (This is a limitation of my Barrier object, not the
  // ACE_Barrier.)  The optional _wait parameter will cause wait() to
  // be invoked if there is already a valid threads value.
  int threads (u_int threads, int wait = 0);
  u_int threads (void);

  // Wait for all threads to reach the point where this is invoked.
  // Because of the snappy way in which ACE_Barrier is implemented,
  // you can invoke these back-to-back with no ill-effects.
  int wait (void);

  // done() will invoke wait().  Before returning though, it will
  // delete the barrier_ pointer below to reclaim some memory.
  int done (void);

  // Reset the owning thread of the barrier.
  void owner( ACE_thread_t _owner );

protected:
  // The number of threads we're synching
  ACE_Atomic_Op<ACE_Mutex, u_int> threads_;

  // The ACE_Barrier that does all of the work
  ACE_Barrier *barrier_;

  // If we mutate the number of threads we have to do some black magic 
  // to make sure there isn't a memory leak.  These two member
  // variables are a part of that magic.
  ACE_Barrier *new_barrier_;
  ACE_Mutex    barrier_mutex_;

  // The thread which created the Barrier in the first place.  Only
  // this thread can change the threads_ value.
  ACE_thread_t owner_;

  // An internal method that constructs the barrier_ as needed.
  int make_barrier (int wait);
};
    
#endif /* BARRIER_H */
