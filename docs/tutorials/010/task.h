// $Id$

#ifndef TASK_H
#define TASK_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* Like the thread-pool server tutorial, we'll derive from ACE_Task<>.
  Our goal here is to show off the ACE_Message_Queue and the best way
  to do that is to use one to pass data between threads.  The easiest
  way to create threads is with ACE_Task<> */
class Task : public ACE_Task <ACE_MT_SYNCH>
{
public:

  typedef ACE_Task <ACE_MT_SYNCH> inherited;

  /* The constructor/destructor are simple but take care of some
    necessary housekeeping.  */
  Task (size_t n_threads);
  ~Task (void);

  /* open() will kick off our thread pool for us.  */
  int open (void * = 0);

  /* Our worker method */
  int svc (void);

  /* All we'll do here is print a message to the user.  */
  int close (u_long flags = 0);

protected:
  /* Just to be clever, I'll use an ACE_Barrier to cause the threads
    to sync in svc() before doing any real work.  */
  ACE_Barrier barrier_;

  size_t n_threads_;
  // Number of threads in the pool.
};

#endif /* TASK_H */
