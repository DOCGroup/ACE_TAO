#include "Incoming_Message_Queue.h"
#include "ORB_Core.h"
#include "debug.h"


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
  // Delete the QD
  if (this->size_)
    {
      TAO_Queued_Data *qd = this->dequeue_head ();
      TAO_Queued_Data::release (qd);
    }
}

size_t
TAO_Incoming_Message_Queue::copy_tail (ACE_Message_Block &block)
{
  // The size of message that is copied
  size_t n = 0;

  if (this->size_ > 0)
    {
      // Check to see if the length of the incoming block is less than
      // that of the <missing_data_> of the tail.
      if ((CORBA::Long)block.length () <= this->queued_data_->missing_data_)
        {
          n = block.length ();
        }
      else
        {
          n = this->queued_data_->missing_data_;
        }

      // Do the copy
      this->queued_data_->msg_block_->copy (block.rd_ptr (),
                                            n);

      // Decerement the missing data
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
// Methods  for TAO_Queued_Data
/************************************************************************/


TAO_Queued_Data::TAO_Queued_Data (void)
  : msg_block_ (0),
    missing_data_ (0),
    byte_order_ (0),
    major_version_ (0),
    minor_version_ (0),
    more_fragments_ (0),
    msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR),
    next_ (0)
{
}

TAO_Queued_Data::TAO_Queued_Data (ACE_Message_Block *mb)
  : msg_block_ (mb),
    missing_data_ (0),
    byte_order_ (0),
    major_version_ (0),
    minor_version_ (0),
    more_fragments_ (0),
    msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR),
    next_ (0)
{
}

TAO_Queued_Data::TAO_Queued_Data (const TAO_Queued_Data &qd)
  : msg_block_ (qd.msg_block_->duplicate ()),
    missing_data_ (qd.missing_data_),
    byte_order_ (qd.byte_order_),
    major_version_ (qd.major_version_),
    minor_version_ (qd.minor_version_),
    more_fragments_ (qd.more_fragments_),
    msg_type_ (qd.msg_type_),
    next_ (0)
{
}
