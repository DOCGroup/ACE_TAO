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
  // Need to delete all the unused data-blocks
}

int
TAO_Incoming_Message_Queue::add_message (ACE_Message_Block *incoming,
                                         size_t missing_data,
                                         CORBA::Octet byte_order)

{
  // Allocate memory for TAO_Queued_Data
  TAO_Queued_Data *qd = this->get_node ();

  if (qd == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Could not make a node \n")));
        }
      return -1;
    }

  // Set the data block
  qd->msg_block_ = incoming;

  // Set the byte_order
  qd->byte_order_ = byte_order;

  qd->missing_data_ = missing_data;

  this->add_node (qd);

  // increment the size of the list
  ++this->size_;

  return 1;
}

void
TAO_Incoming_Message_Queue::copy_message (ACE_Message_Block &block)
{
  if (this->size_ > 0)
    {
      size_t n = 0;

      if (block.length () <= this->queued_data_->missing_data_)
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
}

ACE_Message_Block *
TAO_Incoming_Message_Queue::dequeue_head (CORBA::Octet &byte_order)
{
  TAO_Queued_Data *tmp =
    this->queued_data_->next_;

  if (tmp->missing_data_ != 0)
    return 0;

  ACE_Message_Block *db =
    tmp->msg_block_;

  this->queued_data_->next_ = tmp->next_;
  byte_order = tmp->byte_order_;

  delete tmp;

  // Decrease the size
  --this->size_;

 return db;
}

int
TAO_Incoming_Message_Queue::add_node (
    TAO_Incoming_Message_Queue::TAO_Queued_Data *nd)
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

  return 0;
}
