
// $Id$

#ifndef TASK_H
#define TASK_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "mld.h"

/*
   This is much like the Task we've used in the past for implementing a  thread
   pool.  This time, however, I've made the Task an element in a  singly-linked
   list.  As the svc() method finishes the process() on a  unit of work, it
   will enqueue that unit of work to the next_ Task if  there is one.  If the
   Task does not have a next_ Task, it will invoke the unit of work object's
   fini() method after invoking process().
 */
class Task : public ACE_Task < ACE_MT_SYNCH >
{
public:

    typedef ACE_Task < ACE_MT_SYNCH > inherited;

        // Construct ourselves and an optional number of subtasks
        // chained beyond us.
    Task (int sub_tasks = 0);
    ~Task (void);

        /*
          I really wanted this to be called open() but that was already
          claimed by the Task framework.  start() will kick off our thread
          pool for us.
        */
    int start (int threads = 1);

        // Take Unit_Of_Work objects from the thread pool and invoke
        // their process() and/or fini() as appropriate.
    int svc (void);

        // Shut down the thread pool and it's associated subtasks
    int close (u_long flags = 0);

        // Wait for the pool and subtasks to close
    int wait (void);

protected:
    ACE_Barrier * barrier_;
    Task *next_;
    MLD;
};

#endif
