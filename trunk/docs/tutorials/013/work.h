
// $Id$

#ifndef WORK_H
#define WORK_H

#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "mld.h"

/*
   Our specilized message queue and thread pool will know how to do "work" on
   our Unit_Of_Work baseclass.
 */
class Unit_Of_Work
{
public:
    Unit_Of_Work (void);

    virtual ~ Unit_Of_Work (void);

        // Display the object instance value
    void who_am_i (void);

        // The baseclass can override this to show it's "type name"
    virtual void what_am_i (void);

        // This is where you do application level logic.  It will be
        // called once for each thread pool it passes through.  It
        // would typically implement a state machine and execute a
        // different state on each call.
    virtual int process (void);

        // This is called by the last Task in the series (see task.h)
        // in case our process() didn't get through all of it's states.
    virtual int fini (void);

protected:
    ACE_Atomic_Op < ACE_Mutex, int >state_;
    MLD;
};

/*
   A fairly trivial work derivative that implements an equally trivial state
   machine in process()
 */
class Work : public Unit_Of_Work
{
public:
    Work (void);

    Work (int message);

    virtual ~ Work (void);

    void what_am_i (void);

    int process (void);

    int fini (void);

protected:
    int message_;
    MLD;
};

#endif
