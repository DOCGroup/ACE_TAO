
// $Id$

#ifndef WORK_H
#define WORK_H

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/*
  We'll start by defining a basic unit of work that can be put into
  the message queue.  The threads in the pool will expect to find one
  of these in each message block and will invoke a method or two.
*/
class Unit_Of_Work
{
public:
    Unit_Of_Work (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unit_Of_Work ctor 0x%x\n", (void *) this));
        }
    virtual ~ Unit_Of_Work (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unit_Of_Work dtor 0x%x\n", (void *) this));
        }

    void who_am_i (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unit_Of_Work instance 0x%x\n", (void *) this));
        }

    virtual void what_am_i (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) I am a Unit_Of_Work object\n"));
        }

};

/*
  Now, we specialize the Unit_Of_Work object to do something
  different.  By overriding the virtual methods, we can do whatever
  "real work" is needed but the thread pool doesn't have to know the specifics.
*/
class Work : public Unit_Of_Work
{
public:
    Work (void)
            : message_ (-1)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Work ctor 0x%x\n", (void *) this));
        }

    Work (int message)
            : message_ (message)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Work ctor 0x%x for message %d\n", (void *) this, message_));
        }
    virtual ~ Work (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Work dtor 0x%x\n", (void *) this));
        }

    void what_am_i (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) I am a Work object for message %d\n", message_));
        }

protected:
    int message_;

};

#endif
