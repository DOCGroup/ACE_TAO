
// $Id$

#ifndef BLOCK_H
#define BLOCK_H

#include "ace/Message_Block.h"
#include "ace/Synch.h"
#include "mld.h"
#include "work.h"

/*
  In this Tutorial, we derive from ACE_Data_Block for our special
  data.  With the possiblilty that our Task object may forward the
  unit of work on to another thread pool, we have to make sure that
  the data object doesn't go out of scope unexpectedly.  An
  ACE_Message_Block will be deleted as soon as it's release() method
  is called but the ACE_Data_Blocks it uses are reference counted and
  only delete when the last reference release()es the block.  We use
  that trait to simply our object memory management.
 */
class Data_Block : public ACE_Data_Block
{
public:
    typedef ACE_Data_Block inherited;

        // Create a data block with a unit of work to be done
    Data_Block( Unit_Of_Work * _data );
    
    ~Data_Block(void);

        // Returns the work pointer
    Unit_Of_Work * data(void);
    
protected:
    Unit_Of_Work * data_;
    MLD;    // Our memory leak detector

        // The ACE_Data_Block allows us to choose a locking strategy
        // for making the reference counting thread-safe.  The
        // ACE_Lock_Adaptor<> template adapts the interface of a
        // number of lock objects so thatthe ACE_Message_Block will
        // have an interface it can use.
    class Lock : public ACE_Lock_Adapter<ACE_Mutex>
    {
    public:
        typedef ACE_Lock_Adapter<ACE_Mutex> inherited;

        Lock(void);
        ~Lock(void);

            // When the Data_Block is destroyed, the Message_Block is
            // holding a lock with this object.  If we were to destroy 
            // the Lock with the Data_Block, we would have a
            // segfault.  Instead, the Data_Block invokes destroy() to 
            // mark the object as un-needed so that when the
            // Message_Block invokes release() to drop the lock, the
            // Lock can delete itself.
        int destroy(void);
        int release(void);
    protected:
        int destroy_;
        MLD;
    };
};

/*
  This simple derivative of ACE_Message_Block will construct our
  Data_Block object to contain a unit of work.
 */
class Message_Block : public ACE_Message_Block
{
public:
    typedef ACE_Message_Block inherited;
    
    Message_Block( void );
    Message_Block( Unit_Of_Work * _data );
    
    ~Message_Block( void );

protected:
    MLD;
};
            
#endif
