
// $Id$

#ifndef BLOCK_H
#define BLOCK_H

#include "work.h"

/*
  We derive a Message_Block from ACE_Message_Block and teach it about
  our Unit_Of_Work object.  When our task's svc() method pulls a block
  out of the queue, it can then invoke the virtual methods of the work
  object safely.  In this implementation we've also retained the
  original ACE_Message_Block functionallity so that we can use the
  underlying ACE_Data_Block objects to store data other than our
  Unit_Of_Work.
*/
class Message_Block : public ACE_Message_Block
{
public:
    typedef ACE_Message_Block inherited;

     /*
       Construct our underlying ACE_Message_Block with the requested
       data size and initialize our Unit_Of_Work pointer with the
       given object instance.  Note that this Message_Block instance
       now assumes ownership of the Unit_Of_Work and will delete it
       when the Message_Block is deleted.
     */
    Message_Block( size_t size, Unit_Of_Work * _data )
            : inherited(size), data_(_data)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Message_Block ctor 0x%x for 0x%x\n", (void *) this, data_));
        }

        ~Message_Block(void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Message_Block dtor 0x%x for 0x%x\n", (void *) this, data_));
            delete data_;
        }

     /*
       Return the Unit_Of_Work so that the task can invoke methods on
       it.
     */
    Unit_Of_Work * data(void)
        {
            return this->data_;
        }

protected:
    Unit_Of_Work * data_;

     /*
       Disallow these very dangerous operations.
       If we were to copy a Message_Block object then the data_
       pointer would get copied and we would eventually end up
       deleting the same object multiple times!  That's not good.  By
       preventing the copy, we can avoid this.
     */
    Message_Block &operator= (const Message_Block &);
    Message_Block (const Message_Block &);
};

#endif
