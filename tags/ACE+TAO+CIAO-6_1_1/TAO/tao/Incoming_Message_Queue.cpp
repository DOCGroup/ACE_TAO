// $Id$

#include "tao/Incoming_Message_Queue.h"
#include "tao/Queued_Data.h"
#include "tao/debug.h"

#include "ace/Log_Msg.h"
#include "ace/Malloc_Base.h"

#if !defined (__ACE_INLINE__)
# include "tao/Incoming_Message_Queue.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Incoming_Message_Queue::~TAO_Incoming_Message_Queue (void)
{
  CORBA::ULong const sz = this->size_;

  // Delete all the nodes left behind
  for (CORBA::ULong i = 0;
       i < sz;
       ++i)
    {
      TAO_Queued_Data *qd = this->dequeue_head ();
      TAO_Queued_Data::release (qd);
    }
}


TAO_Queued_Data *
TAO_Incoming_Message_Queue::dequeue_head (void)
{
  if (this->size_ == 0)
    return 0;

  // Get the node on the head of the queue...
  TAO_Queued_Data * const head = this->last_added_->next ();

  // Reset the head node..
  this->last_added_->next (head->next ());

  // Decrease the size and reset last_added_ if empty
  if (--this->size_ == 0)
    this->last_added_ = 0;

  return head;
}

TAO_Queued_Data *
TAO_Incoming_Message_Queue::dequeue_tail (void)
{
  // This is a bit painful stuff...
  if (this->size_ == 0)
    return 0;

  // Get the node on the head of the queue...
  TAO_Queued_Data *head = this->last_added_->next ();

  while (head->next () != this->last_added_)
    {
      head = head->next ();
    }

  // Put the head in tmp.
  head->next (this->last_added_->next ());

  TAO_Queued_Data *ret_qd = this->last_added_;

  this->last_added_ = head;

  // Decrease the size
  if (--this->size_ == 0)
    this->last_added_ = 0;

 return ret_qd;
}

int
TAO_Incoming_Message_Queue::enqueue_tail (TAO_Queued_Data *nd)
{
  if (this->size_ == 0)
    {
      this->last_added_ = nd;
      this->last_added_->next (this->last_added_);
    }
  else
    {
      nd->next (this->last_added_->next ());
      this->last_added_->next (nd);
      this->last_added_ = nd;
    }

  ++this->size_;
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
