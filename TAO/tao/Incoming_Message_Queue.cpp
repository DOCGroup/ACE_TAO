#include "Incoming_Message_Queue.h"
#include "ORB_Core.h"
#include "GIOP_Message_State.h"

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
TAO_Incoming_Message_Queue::add_message (const ACE_Message_Block &block,
                                         const TAO_GIOP_Message_State &state)
{

  // Check whether the last message in the Queue has a half message
  if (this->size_ > 0 &&
      this->queued_data_->missing_data_ > 0)
    {
      // Create the message block
      ACE_Message_Block mb (this->queued_data_->data_block_);

      // Duplicate the message block so that the data block is not
      // deleted by the above message block
      this->queued_data_->data_block_->duplicate ();

      // Set the write pointer in the message block so that we can
      // copy in the message block. To do that we need to calculate
      // the write pointer position.
      size_t wr_pos =
        this->queued_data_->data_block_->size () -
        this->queued_data_->missing_data_;

      mb.wr_ptr (wr_pos);

      // If we have received more data than the missing data we copy
      // only the missing data. If we have received less then we copy
      // all the data. So we calculate how much of data to copy.
      size_t n = 0;

      if (block.length () >
          this->queued_data_->missing_data_)
        {
          n = this->queued_data_->missing_data_;
        }
      else
        {
          n = block.length ();
        }

      // Now do the copy of the missing data.
      mb.copy (block.rd_ptr (),
               n);

      // Now that we have copied n bytes, let us decrease the
      // <missing_data_> by n.
      this->queued_data_->missing_data_ -= n;

    }
  else
    {
      // Create a data block of size of the incoming message
      // @@Bala: Hard coding??
      ACE_Data_Block *db =
        this->orb_core_->data_block_for_message_block (state.message_size ());

      // Make a message block with the above data_block
      ACE_Message_Block mb (db);

      // Increment the ref count so that the data block is not deleted
      // when the message block goes out of scope. This is necessary as we
      // have created the message block on the stack
      db->duplicate ();

      // Do a copy of the message in to the above data block
      mb.copy (block.rd_ptr (),
               block.length ());

      // Allocate memory for TAO_Queued_Data
      TAO_Queued_Data *qd = this->get_node ();

      // Set the data block
      qd->data_block_ = db;

      // Set the byte_order
      qd->byte_order_ = state.byte_order ();

      if (state.message_size () <= block.length ())
        qd->missing_data_ = 0;
      else
        qd->missing_data_ = state.message_size () - block.length ();

      this->add_node (qd);

      // increment the size of the list
      ++this->size_;
    }

  return 1;
}

ACE_Data_Block *
TAO_Incoming_Message_Queue::get_current_message (CORBA::Octet &byte_order)
{
  TAO_Queued_Data *tmp =
    this->queued_data_->next_;

  if (tmp->missing_data_ != 0)
    return 0;

  ACE_Data_Block *db =
    tmp->data_block_;


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
