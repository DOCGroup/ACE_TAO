
// $Id$

#ifndef BLOCK_H
#define BLOCK_H

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "mld.h"
#include "work.h"

/*
   In this Tutorial, we derive from ACE_Data_Block for our special data.  With
   the possiblilty that our Task object may forward the unit of work on to
   another thread pool, we have to make sure that the data object doesn't go
   out of scope unexpectedly.  An ACE_Message_Block will be deleted as soon as
   it's release() method is called but the ACE_Data_Blocks it uses are
   reference counted and only delete when the last reference release()es the
   block.  We use that trait to simplify our object memory management.
 */
class Data_Block : public ACE_Data_Block
{
public:
    typedef ACE_Data_Block inherited;

        // Create a data block with a unit of work to be done
    Data_Block (Unit_Of_Work * _data);

    ~Data_Block (void);

        // Returns the work pointer
    Unit_Of_Work *data (void);

protected:
    Unit_Of_Work * data_;
    MLD;                        // Our memory leak detector

        // The ACE_Data_Block allows us to choose a locking strategy
        // for making the reference counting thread-safe.  The
        // ACE_Lock_Adaptor<> template adapts the interface of a
        // number of lock objects so that the ACE_Message_Block will
        // have an interface it can use.
    class Lock : public ACE_Lock_Adapter < ACE_Mutex >
    {
    public:
        typedef ACE_Lock_Adapter < ACE_Mutex > inherited;

        Lock (void);
        ~Lock (void);

            // destroy() will be called to explicitly delete the
            // lock when we no longer need it.  The method will then
            // cleanup to prevent any memory leaks.
        int destroy (void);

    protected:
        MLD;
    };
};

/*
   This simple derivative of ACE_Message_Block will construct our Data_Block
   object to contain a unit of work.
 */
class Message_Block : public ACE_Message_Block
{
public:
    typedef ACE_Message_Block inherited;

    Message_Block (void);
    Message_Block (Unit_Of_Work * _data);

    ~Message_Block (void);

protected:
    MLD;
};

#endif
