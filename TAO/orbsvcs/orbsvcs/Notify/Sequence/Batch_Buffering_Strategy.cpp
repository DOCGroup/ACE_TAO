// $Id$

#include "Batch_Buffering_Strategy.h"
#include "../Method_Request_Event.h"

#if ! defined (__ACE_INLINE__)
#include "Batch_Buffering_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Batch_Buffering_Strategy, "$id$")

TAO_NS_Batch_Buffering_Strategy::TAO_NS_Batch_Buffering_Strategy (TAO_NS_Message_Queue& msg_queue, TAO_NS_AdminProperties_var& admin_properties, CORBA::Long batch_size)
  :TAO_NS_Buffering_Strategy (msg_queue, admin_properties, batch_size)
{
}

TAO_NS_Batch_Buffering_Strategy::~TAO_NS_Batch_Buffering_Strategy ()
{
}

int
TAO_NS_Batch_Buffering_Strategy::dequeue_batch (CosNotification::EventBatch& event_batch)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, -1);

  // if batch_size is infinite, simply dequeue everything available.

  int pending = 0; // not used.

  if (this->batch_size_ == 0)
    {
      return this->dequeue_available (event_batch, pending);
    }
  else
    {
      // block till batch size of events are available.
      while (this->msg_queue_.message_count () < this->batch_size_)
        {
          if (this->shutdown_ == 1) // if we're shutdown, don't play this silly game.
            return -1;

          this->batch_size_reached_condition_.wait ();
        }

      return this->dequeue_i (this->batch_size_, event_batch);
    }
}

int
TAO_NS_Batch_Buffering_Strategy::dequeue_available (CosNotification::EventBatch& event_batch, int &pending)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, -1);

  int deq_count = this->msg_queue_.message_count ();

  if (this->batch_size_ != 0 && deq_count > this->batch_size_) // Restrict upto batch size.
    deq_count = this->batch_size_;

  pending = this->msg_queue_.message_count () - deq_count;

  return this->dequeue_i (deq_count, event_batch);
}

int
TAO_NS_Batch_Buffering_Strategy::dequeue_i (int max_deq_count, CosNotification::EventBatch& event_batch)
{
  ACE_Message_Block *mb;

  int deq_count = 0;

  event_batch.length (max_deq_count);

  for (; deq_count < max_deq_count; ++deq_count)
    {
      if (this->msg_queue_.dequeue (mb) == -1)
        break; // error, simply return what we could extract so far.

      --this->global_queue_length_;

      TAO_NS_Method_Request_Event* mre = ACE_dynamic_cast (TAO_NS_Method_Request_Event*, mb);

      mre->event ()->convert (event_batch[deq_count]);

      ACE_Message_Block::release (mb);
    }

  event_batch.length (deq_count);

  this->global_queue_not_full_condition_.signal ();
  this->local_queue_not_full_condition_.signal ();

  return deq_count;
}
