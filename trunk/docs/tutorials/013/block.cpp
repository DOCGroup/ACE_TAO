
// $Id$

#include "block.h"

/*
  Construct a Dat_Block to contain a unit of work.  Note the careful
  construction of the baseclass to set the block type and the locking strategy.
 */
Data_Block::Data_Block( Unit_Of_Work * _data )
   : inherited(0,ACE_Message_Block::MB_DATA,0,0,new Lock(),0,0)
    ,data_(_data)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Block ctor for 0x%x\n", (void *) this, (void*)data_));
}

/*
  The Lock object created in the constructor is stored in the
  baseclass and available through the locking_strategy() method.  We
  can cast it's value to our Lock object and invoke the destroy() to
  indicate that we want it to go away when the lock is released.
 */
Data_Block::~Data_Block(void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Block dtor for 0x%x\n", (void *) this, (void*)data_));
    ((Lock*)locking_strategy())->destroy();
    delete data_;
}

/*
  Return the data
*/
Unit_Of_Work * Data_Block::data(void)
{
    return this->data_;
}

Data_Block::Lock::Lock(void)
    : destroy_(0)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Lock ctor\n", (void *) this ));
}

Data_Block::Lock::~Lock(void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Lock dtor\n", (void *) this ));
}

/*
  Set our destroy_ flag so that the next lock release will cause us to 
  be deleted.
*/
int Data_Block::Lock::destroy(void)
{
    ++destroy_;
    return(0);
}

/*
  Mutexes have acquire() and release() methods.  We've overridden the
  latter so that when the object we're protecting goes away, we can
  make ourselves go away after the lock is released.
*/
int Data_Block::Lock::release(void)
{
    int rval = inherited::release();
    if( destroy_ )
    {
        delete this;
    }
    return rval;
}

/*
  Create an baseclas unit of work when we instantiate a hangup message
*/
Message_Block::Message_Block( void )
    : inherited( new Data_Block( new Unit_Of_Work() ) )
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Message_Block ctor for shutdown\n", (void *) this ));
    this->msg_type( MB_HANGUP );
}

/*
  Store the unit of work in a Data_Block and initialize the baseclass
  with that data.
*/
Message_Block::Message_Block( Unit_Of_Work * _data )
    : inherited( new Data_Block(_data) )
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Message_Block ctor for 0x%x\n", (void *) this, (void*)_data));
}

Message_Block::~Message_Block( void )
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Message_Block dtor\n", (void *) this ));
}
