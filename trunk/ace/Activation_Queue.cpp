// Activation_Queue.cpp
// $Id$

#define ACE_BUILD_DLL

#include "ace/Activation_Queue.h"

// Dump the state of an object.

void 
ACE_Activation_Queue::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
	      "delete_queue_ = %d\n",
	      this->delete_queue_));
  ACE_DEBUG ((LM_INFO,"queue_: \n"));
  if (this->queue_)
    this->queue_->dump();
  else
    ACE_DEBUG ((LM_DEBUG,"(NULL)\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Activation_Queue::ACE_Activation_Queue (ACE_Message_Queue<ACE_SYNCH> *new_queue) 
  : delete_queue_ (0)
{
  if (new_queue)
    this->queue_ = new_queue;
  else
    {
      ACE_NEW (this->queue_, ACE_Message_Queue<ACE_SYNCH>);
      this->delete_queue_ = 1;
    }
}

ACE_Activation_Queue::~ACE_Activation_Queue (void) 
{
  if (this->delete_queue_ != 0)
    delete this->queue_;
}

ACE_Method_Object *
ACE_Activation_Queue::dequeue (ACE_Time_Value *tv)
{
  ACE_Message_Block *mb;
  ACE_Method_Object *mo;

  // Dequeue the message.
  this->queue_->dequeue_head (mb, tv);

  // Get the method object.
  mo = (ACE_Method_Object *) mb->base ();

  // Delete the message block.
  delete mb;
  return mo;
}

int 
ACE_Activation_Queue::enqueue (ACE_Method_Object *mo,
			       ACE_Time_Value *tv)
{
  ACE_Message_Block *mb;

  ACE_NEW_RETURN (mb, ACE_Message_Block ((char *) mo), -1);

  return this->queue_->enqueue (mb, tv);
}

