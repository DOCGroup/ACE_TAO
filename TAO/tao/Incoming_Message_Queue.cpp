#include "Incoming_Message_Queue.h"
#include "debug.h"

#include "ace/Log_Msg.h"
#include "ace/Malloc_Base.h"


#if !defined (__ACE_INLINE__)
# include "Incoming_Message_Queue.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           Incoming_Message_Queue,
           "$Id$")

TAO_Incoming_Message_Queue::TAO_Incoming_Message_Queue (TAO_ORB_Core *orb_core)
  : last_added_ (0),
    size_ (0),
    orb_core_ (orb_core)
{
}

TAO_Incoming_Message_Queue::~TAO_Incoming_Message_Queue (void)
{
  const int sz = this->size_;

  // Delete all the nodes left behind
  for (int i = 0;
       i < sz;
       ++i)
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
      if ((CORBA::Long)block.length () < this->last_added_->missing_data_)
        {
          n = block.length ();
        }
      else
        {
          n = this->last_added_->missing_data_;
        }

      // Do the copy
      this->last_added_->msg_block_->copy (block.rd_ptr (),
                                            n);

      // Decerement the missing data
      this->last_added_->missing_data_ -= n;
    }

  return n;
}

TAO_Queued_Data *
TAO_Incoming_Message_Queue::dequeue_head (void)
{
  if (this->size_ == 0)
    return 0;

  // Get the node on the head of the queue...
  TAO_Queued_Data *head = this->last_added_->next_;

  // Reset the head node..
  this->last_added_->next_ = head->next_;

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
  TAO_Queued_Data *head =
    this->last_added_->next_;

  while (head->next_ != this->last_added_)
    {
      head = head->next_;
    }

  // Put the head in tmp.
  head->next_ = this->last_added_->next_;

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
      this->last_added_->next_ = this->last_added_;
    }
  else
    {
      nd->next_ = this->last_added_->next_;
      this->last_added_->next_ = nd;
      this->last_added_ = nd;
    }

  ++ this->size_;
  return 0;
}

TAO_Queued_Data *
TAO_Incoming_Message_Queue::find_fragment_chain (CORBA::Octet major,
                                                 CORBA::Octet minor) const
{
  TAO_Queued_Data *found = 0;
  if (this->last_added_ != 0)
    {
      TAO_Queued_Data *qd = this->last_added_->next_;

      do {
        if (qd->more_fragments_ &&
            qd->major_version_ == major && qd->minor_version_ == minor)
          {
            found = qd;
          }
        else
          {
            qd = qd->next_;
          }
      } while (found == 0 && qd != this->last_added_->next_);
    }

  return found;
}

TAO_Queued_Data *
TAO_Incoming_Message_Queue::find_fragment_chain (CORBA::ULong request_id) const
{
  TAO_Queued_Data *found = 0;
  if (this->last_added_ != 0)
    {
      TAO_Queued_Data *qd = this->last_added_->next_;

      do {
        if (qd->more_fragments_ && qd->request_id_ == request_id)
          {
            found = qd;
          }
        else
          {
            qd = qd->next_;
          }
      } while (found == 0 && qd != this->last_added_->next_);
    }

  return found;
}


/************************************************************************/
// Methods  for TAO_Queued_Data
/************************************************************************/

/*!
  \brief Allocate and return a new empty message block of size \a new_size mimicking parameters of \a mb.

  This function allocates a new aligned message block using the same
  allocators and flags as found in \a mb.  The size of the new message
  block is at least \a new_size; the size may be adjusted up in order
  to accomodate alignment requirements and still fit \a new_size bytes
  into the aligned buffer.

  \param mb message block whose parameters should be mimicked
  \param new_size size of the new message block (will be adjusted for proper alignment)
  \return an aligned message block with rd_ptr sitting at correct alignment spot, 0 on failure

  \author Thanks to Rich Seibel for helping implement the public API for ACE_Message_Block!
 */
static ACE_Message_Block*
clone_mb_nocopy_size (ACE_Message_Block *mb, size_t span_size)
{
  // Calculate the required size of the cloned block with alignment
  size_t aligned_size = ACE_CDR::first_size (span_size + ACE_CDR::MAX_ALIGNMENT);

  // Get the allocators
  ACE_Allocator *data_allocator;
  ACE_Allocator *data_block_allocator;
  ACE_Allocator *message_block_allocator;
  mb->access_allocators (data_allocator,
                         data_block_allocator,
                         message_block_allocator);

  // Create a new Message Block
  ACE_Message_Block *nb;
  ACE_NEW_MALLOC_RETURN (nb,
                         static_cast<ACE_Message_Block*> (
                                         message_block_allocator->malloc (
                                           sizeof (ACE_Message_Block))),
                         ACE_Message_Block(aligned_size,
                                           mb->msg_type(),
                                           mb->cont(),
                                           0, //we want the data block created
                                           data_allocator,
                                           mb->locking_strategy(),
                                           mb->msg_priority(),
                                           mb->msg_execution_time (),
                                           mb->msg_deadline_time (),
                                           data_block_allocator,
                                           message_block_allocator),
                         0);

  ACE_CDR::mb_align (nb);

  // Copy the flags over, but be SURE to clear the DONT_DELETE flag, since
  // we just dynamically allocated the two things.
  nb->set_flags (mb->flags());
  nb->clr_flags (ACE_Message_Block::DONT_DELETE);

  return nb;
}

TAO_Queued_Data::TAO_Queued_Data (ACE_Allocator *alloc)
  : msg_block_ (0),
    missing_data_ (0),
    byte_order_ (0),
    major_version_ (0),
    minor_version_ (0),
    more_fragments_ (0),
    request_id_ (0),
    msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR),
    next_ (0),
    allocator_ (alloc)
{
}

TAO_Queued_Data::TAO_Queued_Data (ACE_Message_Block *mb,
                                  ACE_Allocator *alloc)
  : msg_block_ (mb),
    missing_data_ (0),
    byte_order_ (0),
    major_version_ (0),
    minor_version_ (0),
    more_fragments_ (0),
    request_id_ (0),
    msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR),
    next_ (0),
    allocator_ (alloc)
{
}

TAO_Queued_Data::TAO_Queued_Data (const TAO_Queued_Data &qd)
  : msg_block_ (qd.msg_block_->duplicate ()),
    missing_data_ (qd.missing_data_),
    byte_order_ (qd.byte_order_),
    major_version_ (qd.major_version_),
    minor_version_ (qd.minor_version_),
    more_fragments_ (qd.more_fragments_),
    request_id_ (qd.request_id_),
    msg_type_ (qd.msg_type_),
    next_ (0),
    allocator_ (qd.allocator_)
{
}

/*static*/
TAO_Queued_Data *
TAO_Queued_Data::make_queued_data (ACE_Allocator *alloc)
{
  TAO_Queued_Data *qd = 0;

  if (alloc)
    {
      ACE_NEW_MALLOC_RETURN (qd,
                             static_cast<TAO_Queued_Data *> (
                               alloc->malloc (sizeof (TAO_Queued_Data))),
                             TAO_Queued_Data (alloc),
                             0);

      return qd;
    }

  // No allocator, so use the global pool!
  // @@ TODO: We should be removing this at some point of time!
  if (TAO_debug_level == 4)
    {
      // This debug is for testing purposes!
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Queued_Data::get_queued_data\n"
                  "Using global pool for allocation \n"));
    }

  ACE_NEW_RETURN (qd,
                  TAO_Queued_Data,
                  0);

  return qd;
}

/*static*/
void
TAO_Queued_Data::release (TAO_Queued_Data *qd)
{
  //// TODO
  ACE_Message_Block::release (qd->msg_block_);

  if (qd->allocator_)
    {
      ACE_DES_FREE (qd,
                    qd->allocator_->free,
                    TAO_Queued_Data);

      return;
    }

  // @@todo: Need to be removed at some point of time!
  if (TAO_debug_level == 4)
    {
      // This debug is for testing purposes!
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Queued_Data[%d]::release\n",
                  "Using global pool for releasing \n"));
    }
  delete qd;

}


TAO_Queued_Data *
TAO_Queued_Data::duplicate (TAO_Queued_Data &sqd)
{
  // Check to see if the underlying block is on the stack. If not it
  // is fine. If the datablock is on stack, try to make a copy of that
  // before doing a duplicate.
  // @@ todo: Theoretically this should be within the Message Block,
  // but we dont have much scope to do this in that mess. Probably in
  // the next stage of MB rewrite we should be okay
  ACE_Message_Block::Message_Flags fl =
    sqd.msg_block_->self_flags ();

  if (ACE_BIT_ENABLED (fl,
                       ACE_Message_Block::DONT_DELETE))
    (void) TAO_Queued_Data::replace_data_block (*sqd.msg_block_);


  TAO_Queued_Data *qd = 0;

  if (sqd.allocator_)
    {
      ACE_NEW_MALLOC_RETURN (qd,
                             static_cast<TAO_Queued_Data *> (
                               sqd.allocator_->malloc (sizeof (TAO_Queued_Data))),
                             TAO_Queued_Data (sqd),
                             0);

      return qd;
    }

  // No allocator, so use the global pool!
  // @@ TODO: We should be removing this at some point of time!
  if (TAO_debug_level == 4)
    {
      // This debug is for testing purposes!
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Queued_Data[%d]::duplicate\n",
                  "Using global pool for allocation \n"));
    }

  ACE_NEW_RETURN (qd,
                  TAO_Queued_Data (sqd),
                  0);

  return qd;
}

void
TAO_Queued_Data::consolidate (void)
{
  // Is this a chain of fragments?
  if (this->more_fragments_ && this->msg_block_->cont () != 0)
    {
      // Create a message block big enough to hold the entire chain
      ACE_Message_Block *dest = clone_mb_nocopy_size (
                                      this->msg_block_,
                                      this->msg_block_->total_length ());

      // Reset the cont() parameter.  We have cloned the message
      // block but not the chain as we will no longer have chain.
      dest->cont (0);

      // Use ACE_CDR to consolidate the chain for us
      ACE_CDR::consolidate (dest, this->msg_block_);

      // free the original message block chain
      this->msg_block_->release ();

      // Set the message block to the new consolidated message block
      this->msg_block_ = dest;
      this->more_fragments_ = 0;
    }
}

