// Activation_Queue.cpp
// $Id$

#define ACE_BUILD_DLL

#include "ace/Activation_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Activation_Queue.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Activation_Queue, "$Id$")

// Dump the state of an object.

void 
ACE_Activation_Queue::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("delete_queue_ = %d\n"),
	      this->delete_queue_));
  ACE_DEBUG ((LM_INFO, ASYS_TEXT ("queue_: \n")));
  if (this->queue_)
    this->queue_->dump();
  else
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(NULL)\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Activation_Queue::ACE_Activation_Queue (ACE_Message_Queue<ACE_SYNCH> *new_queue) 
  : delete_queue_ (0)
{
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
      ACE_Method_Request *mo =
        ACE_reinterpret_cast (ACE_Method_Request *,
                              mb->base ());

      // Delete the message block.
      mb->release ();
      return mo;
    }
  else
    return 0;
}

int 
ACE_Activation_Queue::enqueue (ACE_Method_Request *mo,
			       ACE_Time_Value *tv)
{
  ACE_Message_Block *mb;

  ACE_NEW_RETURN (mb,
                  ACE_Message_Block ((char *) mo,
                                     0,
                                     mo->priority ()),
                  -1);

  // Enqueue in priority order.
  return this->queue_->enqueue_prio (mb, tv);
}
