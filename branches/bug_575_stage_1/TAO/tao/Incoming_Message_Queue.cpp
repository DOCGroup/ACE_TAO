#include "Incoming_Message_Queue.h"
#include "ORB_Core.h"
#include "debug.h"
#include "Pluggable_Messaging_Utils.h"

#if !defined (__ACE_INLINE__)
# include "Incoming_Message_Queue.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, Incoming_Message_Queue, "$Id$")


TAO_Incoming_Message_Queue::TAO_Incoming_Message_Queue (TAO_ORB_Core *orb_core)
  : queued_data_ (0),
    size_ (0),
    orb_core_ (orb_core)
{
}

TAO_Incoming_Message_Queue::~TAO_Incoming_Message_Queue (void)
{
  // Need to delete all the unused data-blocks
}

size_t
TAO_Incoming_Message_Queue::copy_message (ACE_Message_Block &block)
{
  size_t n = 0;

  if (this->size_ > 0)
    {
      if ((CORBA::Long)block.length () <= this->queued_data_->missing_data_)
        {
          n = block.length ();
        }
      else
        {
          n = this->queued_data_->missing_data_;
        }

      this->queued_data_->msg_block_->copy (block.rd_ptr (),
                                            n);

      this->queued_data_->missing_data_ -= n;
    }

  return n;
}

TAO_Queued_Data *
TAO_Incoming_Message_Queue::dequeue_head (void)
{
  // Get the node on the head of the queue...
  TAO_Queued_Data *tmp =
    this->queued_data_->next_;

  // Reset the head node..
  this->queued_data_->next_ = tmp->next_;

  // Decrease the size
  --this->size_;

 return tmp;
}

TAO_Queued_Data *
TAO_Incoming_Message_Queue::dequeue_tail (void)
{
  // This is a bit painful stuff...
  if (this->size_ == 0)
    return 0;

  // Get the node on the head of the queue...
  TAO_Queued_Data *tmp =
    this->queued_data_->next_;

  while (tmp->next_ != this->queued_data_)
    {
      tmp = tmp->next_;
    }

  // Put the head in tmp.
  tmp->next_ = this->queued_data_->next_;

  TAO_Queued_Data *ret_qd = this->queued_data_;

  this->queued_data_ = tmp;

  // Decrease the size
  --this->size_;

 return ret_qd;
}


int
TAO_Incoming_Message_Queue::enqueue_tail (TAO_Queued_Data *nd)
{
  if (this->size_ == 0)
    {
      this->queued_data_ = nd;
      this->queued_data_->next_ = this->queued_data_;
    }
  else
    {
      nd->next_ = this->queued_data_->next_;
      this->queued_data_->next_ = nd;
      this->queued_data_ = nd;
    }

  ++ this->size_;
  return 0;
}


/************************************************************************/

TAO_Queued_Data::TAO_Queued_Data (void)
  : msg_block_ (0),
    missing_data_ (0),
    byte_order_ (0),
    major_version_ (0),
    minor_version_ (0),
    msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR),
    next_ (0)
{
}
