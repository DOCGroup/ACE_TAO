#include "ace/Activation_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Activation_Queue.i"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Msg.h"
#include "ace/Malloc_Base.h"

ACE_RCSID (ace,
           Activation_Queue,
           "$Id$")


void
ACE_Activation_Queue::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("delete_queue_ = %d\n"),
              this->delete_queue_));
  ACE_DEBUG ((LM_INFO, ACE_LIB_TEXT ("queue_: \n")));
  if (this->queue_)
    this->queue_->dump();
  else
    ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("(NULL)\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Activation_Queue::ACE_Activation_Queue (ACE_Message_Queue<ACE_SYNCH> *new_queue,
                                            ACE_Allocator *alloc,
                                            ACE_Allocator *db_alloc)
  : delete_queue_ (0)
  , allocator_(alloc)
  , data_block_allocator_(db_alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (new_queue)
    this->queue_ = new_queue;
  else
    {
      ACE_NEW (this->queue_,
               ACE_Message_Queue<ACE_SYNCH>);
      this->delete_queue_ = 1;
    }
}

ACE_Activation_Queue::~ACE_Activation_Queue (void)
{
  if (this->delete_queue_ != 0)
    delete this->queue_;
}

ACE_Method_Request *
ACE_Activation_Queue::dequeue (ACE_Time_Value *tv)
{
  ACE_Message_Block *mb;

  // Dequeue the message.
  if (this->queue_->dequeue_head (mb, tv) != -1)
    {
      // Get the next <Method_Request>.
      ACE_Method_Request *mr =
        ACE_reinterpret_cast (ACE_Method_Request *,
                              mb->base ());
      // Delete the message block.
      mb->release ();
      return mr;
    }
  else
    return 0;
}

int
ACE_Activation_Queue::enqueue (ACE_Method_Request *mr,
                               ACE_Time_Value *tv)
{
  ACE_Message_Block *mb;

  // We pass sizeof (*mr) here so that flow control will work
  // correctly.  Since we also pass <mr> note that no unnecessary
  // memory is actually allocated -- just the size field is set.
  ACE_NEW_MALLOC_RETURN (mb,
                         ACE_static_cast(ACE_Message_Block *,
                                         this->allocator_->malloc (sizeof (ACE_Message_Block))),
                         ACE_Message_Block (sizeof (*mr),    // size
                                            ACE_Message_Block::MB_DATA, // type
                                            0,       // cont
                                            (char *) mr,    // data
                                            0,       // allocator
                                            0,       // locking strategy
                                            mr->priority (), // priority
                                            ACE_Time_Value::zero,     // execution time
                                            ACE_Time_Value::max_time, // absolute time of deadline
                                            this->data_block_allocator_,  // data_block allocator
                                            this->allocator_), // message_block allocator
                         -1);

  // Enqueue in priority order.
  int result = this->queue_->enqueue_prio (mb, tv);

  // Free ACE_Message_Block if enqueue_prio failed.
  if (result == -1)
      ACE_DES_FREE (mb, this->allocator_->free, ACE_Message_Block);

  return result;
}
