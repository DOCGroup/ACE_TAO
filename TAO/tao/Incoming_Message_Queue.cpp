#include "Incoming_Message_Queue.h"
#include "ORB_Core.h"
#include "debug.h"
#include "ace/Malloc_T.h"
#include "ace/Message_Block.h"

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
  int sz = this->size_;

  // Delete all the nodes left behind
  for (int i = 0;
       i < sz;
       i++)
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
      if (block.length () <= this->queued_data_->missing_data_bytes_)
        {
          n = block.length ();
        }
      else
        {
          n = this->queued_data_->missing_data_bytes_;
        }

      // Do the copy
      this->queued_data_->msg_block_->copy (block.rd_ptr (),
                                            n);

      // Decerement the missing data
      this->queued_data_->missing_data_bytes_ -= n;
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

TAO_Queued_Data::TAO_Queued_Data (ACE_Allocator *alloc)
  : msg_block_ (0)
  , current_state_ (INVALID)
  , missing_data_bytes_ (0)
  , byte_order_ (0)
  , major_version_ (0)
  , minor_version_ (0)
  , more_fragments_ (0)
  , msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR)
  , next_ (0)
  , allocator_ (alloc)
{
}

TAO_Queued_Data::TAO_Queued_Data (ACE_Message_Block *mb,
                                  ACE_Allocator *alloc)
  : msg_block_ (mb)
  , current_state_ (INVALID)
  , missing_data_bytes_ (0)
  , byte_order_ (0)
  , major_version_ (0)
  , minor_version_ (0)
  , more_fragments_ (0)
  , msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR)
  , next_ (0)
  , allocator_ (alloc)
{
}

TAO_Queued_Data::TAO_Queued_Data (const TAO_Queued_Data &qd)
  : msg_block_ (qd.msg_block_->duplicate ())
  , current_state_ (qd.current_state_)
  , missing_data_bytes_ (qd.missing_data_bytes_)
  , byte_order_ (qd.byte_order_)
  , major_version_ (qd.major_version_)
  , minor_version_ (qd.minor_version_)
  , more_fragments_ (qd.more_fragments_)
  , msg_type_ (qd.msg_type_)
  , next_ (0)
  , allocator_ (qd.allocator_)
{
}


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

  \author Thanks to Rich Seibel for helping implement with the public API for ACE_Message_Block!
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
                         ACE_static_cast(ACE_Message_Block*,
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

  // Do whatever with the flags
  nb->set_flags (mb->flags());
  nb->set_self_flags (mb->self_flags());
  //  nb->clr_flags (mask);

  return nb;
}

/*!
  \brief Copy data from \a src->rd_ptr to \a dst->wr_ptr, of at most \a span_size bytes.

  (This is similar to memcpy, although with message blocks we can be a
  little smarter.)  This function assumes that \a dst has enough space
  for \a span_size bytes, and that \a src has at least \a span_size
  bytes available to copy.  When everything is copied \a dst->wr_ptr
  gets updated accordingly, but \a src->rd_ptr is left to the caller
  to update.

  \param dst the destination message block
  \param src the source message block
  \param span_size size of the maximum span of bytes to be copied
  \return 0 on failure, otherwise \a dst
 */
static ACE_Message_Block*
copy_mb_span (ACE_Message_Block *dst, ACE_Message_Block *src, size_t span_size)
{
  // @todo check for enough space in dst, and src contains at least span_size

  if (src == 0 || dst == 0)
    return 0;

  if (span_size == 0)
    return dst;

  dst->copy (src->rd_ptr (), span_size);
  return dst;
}

/*static*/
TAO_Queued_Data *
TAO_Queued_Data::make_uncompleted_message (ACE_Message_Block *mb,
                                           TAO_Pluggable_Messaging &msging_obj,
                                           ACE_Allocator *alloc)
{
  TAO_Queued_Data *new_qd = 0;
  const size_t HDR_LEN = msging_obj.header_length ();

  // Validate arguments.
  if (mb == 0)
    goto failure;

  new_qd = make_queued_data (alloc);
  if (new_qd == 0)
    goto failure;

  // do we have enough bytes to make a complete header?
  if (mb->length() >= msging_obj.header_length ())
    {
      // Since we have enough bytes to make a complete header,
      // the header needs to be valid.  Check that now, and punt
      // if it's not valid.
      if (! msging_obj.check_for_valid_header (*mb))
        {
          goto failure;
        }
      else
        {
          new_qd->current_state_ = WAITING_TO_COMPLETE_PAYLOAD;
          msging_obj.set_queued_data_from_message_header (new_qd, *mb);

          // missing_data_bytes_ now has the full GIOP message size, so we allocate
          // a new message block of that size, plus the header.
          new_qd->msg_block_ = clone_mb_nocopy_size  (mb,
                                                      new_qd->missing_data_bytes_ +
                                                      HDR_LEN);
          // Of course, we don't have the whole message (if we did, we
          // wouldn't be here!), so we copy only what we've got, i.e., whatever's
          // in the message block.
          if (copy_mb_span (new_qd->msg_block_, mb, mb->length ()) == 0)
            goto failure;

          // missing_data_bytes_ now has the full GIOP message size, but
          // there might still be stuff in mb.  Therefore, we have to adjust
          // missing_data_bytes_, i.e., decrease it by the number of "actual
          // payload bytes" in mb.
          //
          // "actual payload bytes" :== length of mb (which included the header) - header length
          new_qd->missing_data_bytes_ -= (mb->length () - HDR_LEN);
          mb->rd_ptr (mb->length ());
        }
    }
  else
    {
      new_qd->current_state_ = WAITING_TO_COMPLETE_HEADER;
      new_qd->msg_block_ = clone_mb_nocopy_size (mb, HDR_LEN);
      if (new_qd->msg_block_ == 0 ||
          copy_mb_span (new_qd->msg_block_, mb, mb->length ()) == 0)
        goto failure;
      new_qd->missing_data_bytes_ = msging_obj.header_length () - mb->length ();
      mb->rd_ptr (mb->length ());
    }

  ACE_ASSERT (new_qd->current_state_ != INVALID);
  if (TAO_debug_level > 7)
    {
      const char* s = "?unk?";
      switch (new_qd->current_state_)
        {
        case WAITING_TO_COMPLETE_HEADER: s = "WAITING_TO_COMPLETE_HEADER"; break;
        case WAITING_TO_COMPLETE_PAYLOAD: s = "WAITING_TO_COMPLETE_PAYLOAD"; break;
        case INVALID: s = "INVALID"; break;
        case COMPLETED: s = "COMPLETED"; break;
        }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Queued_Data::make_uncompleted_message: ")
                  ACE_TEXT ("made uncompleted message from %u bytes into qd=%-08x:")
                  ACE_TEXT ("state=%s,missing_data_bytes=%u\n"),
                  new_qd->msg_block_->length(), new_qd, s, new_qd->missing_data_bytes_));
    }
  return new_qd;

failure:
  if (TAO_debug_level > 7)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Queued_Data::make_uncompleted_message: ")
                  ACE_TEXT ("failed to make uncompleted message: mb=%-08x, qd=%-08x\n"),
                  mb, new_qd));
    }
  TAO_Queued_Data::release (new_qd);
  return 0;
}

#if 0
/*!
  \brief Act like ACE_Message_Block::clone, but only clone the part btw. rd_ptr and wr_ptr.
 */
// Follow #def's swiped from Message_Block.cpp
#if defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
#define ACE_EXECUTION_TIME this->execution_time_
#define ACE_DEADLINE_TIME this->deadline_time_
#else
#define ACE_EXECUTION_TIME ACE_Time_Value::zero
#define ACE_DEADLINE_TIME ACE_Time_Value::max_time
#endif /* ACE_HAS_TIMED_MESSAGE_BLOCKS */

static ACE_Data_Block*
clone_span_nocopy (/*const*/ ACE_Data_Block *the_db,
                   const char* beg, size_t span,
                   ACE_Message_Block::Message_Flags mask = 0)
{
  // Allocate a new data block through the same allocator as 'the_db'
  const ACE_Message_Block::Message_Flags always_clear =
    ACE_Message_Block::DONT_DELETE;

  ACE_Data_Block *nb;
  ACE_Allocator *db_allocator = the_db->data_block_allocator ();

  ACE_NEW_MALLOC_RETURN (nb,
                         ACE_static_cast(ACE_Data_Block*,
                           db_allocator->malloc (sizeof (ACE_Data_Block))),
                         ACE_Data_Block (span, // size
                                         the_db->msg_type (),     // type
                                         0,               // data
                                         the_db->allocator_strategy (), // allocator
                                         the_db->locking_strategy (), // locking strategy
                                         the_db->flags (),  // flags
                                         db_allocator),
                         0);


  // Set new flags minus the mask...
  nb->clr_flags (mask | always_clear);
  
  // Copy in the data, and set the pointer
  //  ACE_OS::memcpy (nb->base (), beg, span);

  return nb;
}

static ACE_Message_Block*
clone_span (/*const*/ ACE_Message_Block *the_mb, size_t span_size, ACE_Message_Block::Message_Flags mask = 0)
{
  // Get a pointer to a "cloned" <ACE_Data_Block> (will copy the
  // values rather than increment the reference count).
  size_t aligned_size = ACE_CDR::first_size (span_size + ACE_CDR::MAX_ALIGNMENT);
  ACE_Data_Block *db = clone_span_nocopy (the_mb->data_block (), the_mb->rd_ptr (), aligned_size, mask);
  if (db == 0)
    return 0;

  ACE_Message_Block *nb;

  if(the_mb->message_block_allocator_ == 0)
    {
      ACE_NEW_RETURN (nb,
                      ACE_Message_Block (0, // size
                                         ACE_Message_Block::ACE_Message_Type (0), // type
                                         0, // cont
                                         0, // data
                                         0, // allocator
                                         0, // locking strategy
                                         0, // flags
                                         the_mb->priority_, // priority
                                         ACE_EXECUTION_TIME, // execution time
                                         ACE_DEADLINE_TIME, // absolute time to deadline
                                         // Get a pointer to a
                                         // "duplicated" <ACE_Data_Block>
                                         // (will simply increment the
                                         // reference count).
                                         db,
                                         db->data_block_allocator (),
                                         the_mb->message_block_allocator_),
                      0);
    }
  else
    {
      // This is the ACE_NEW_MALLOC macro with the return check removed.
      // We need to do it this way because if it fails we need to release
      // the cloned data block that was created above.  If we used
      // ACE_NEW_MALLOC_RETURN, there would be a memory leak because the
      // above db pointer would be left dangling.
      nb = ACE_static_cast(ACE_Message_Block*,the_mb->message_block_allocator_->malloc (sizeof (ACE_Message_Block)));
      if(nb != 0)
        new (nb) ACE_Message_Block (0, // size
                                    ACE_Message_Block::ACE_Message_Type (0), // type
                                    0, // cont
                                    0, // data
                                    0, // allocator
                                    0, // locking strategy
                                    0, // flags
                                    the_mb->priority_, // priority
                                    ACE_EXECUTION_TIME, // execution time
                                    ACE_DEADLINE_TIME, // absolute time to deadline
                                    db,
                                    db->data_block_allocator (),
                                    the_mb->message_block_allocator_);
    }

  if (nb == 0)
    {
      db->release ();
      return 0;
    }

  ACE_CDR::mb_align (nb);
  nb->copy (the_mb->rd_ptr(), span_size);

  // Clone all the continuation messages if necessary.
  if (the_mb->cont () != 0
      && (nb->cont_ = the_mb->cont ()->clone (mask)) == 0)
    {
      nb->release ();
      return 0;
    }
  return nb;
}
#endif

/*static*/
TAO_Queued_Data *
TAO_Queued_Data::make_completed_message (ACE_Message_Block &mb,
                                         TAO_Pluggable_Messaging &msging_obj,
                                         ACE_Allocator *alloc)
{
  // THIS IMPLEMENTATION DOESN'T WORK THE SAME AS ITS USAGE!
  // WE CAN'T JUST ADOPT mb, BECAUSE IT MAY CONTAIN MORE THAN
  // ONE PROTOCOL MESSAGE.  WE THEREFORE NEED TO CLONE IT.  THIS
  // MEANS UPDATING THE DOCUMENTATION, AND IT ALSO MEANS THAT IT
  // BEHAVES DIFFERENTLY FROM make_uncompleted_message.

  // Validate arguments.
  if (mb.length() < msging_obj.header_length ())
    return 0;

  size_t total_msg_len = 0;
  TAO_Queued_Data *new_qd = make_queued_data (alloc);
  if (new_qd == 0)
    goto failure;

  // We can assume that there are enough bytes for a header, so
  // extract the header data.  Don't assume that there's enough for
  // the payload just yet.
  new_qd->current_state_ = WAITING_TO_COMPLETE_PAYLOAD;
  msging_obj.set_queued_data_from_message_header (new_qd, mb);
  if (new_qd->current_state_ == INVALID)
    goto failure;

  // new_qd_->missing_data_bytes_ + protocol header length should be
  // *at least* the length of the message.  Verify that we have that
  // many bytes in the message block and, if we don't, release the new
  // qd and fail.
  total_msg_len = new_qd->missing_data_bytes_ + msging_obj.header_length ();
  if (total_msg_len > mb.length ())
    goto failure;

  // Make a copy of the relevant portion of mb and hang on to it
  if ((new_qd->msg_block_ = clone_mb_nocopy_size (&mb, total_msg_len)) == 0)
    goto failure;

  if (copy_mb_span (new_qd->msg_block_, &mb, total_msg_len) == 0)
    goto failure;
  
  // Update missing data and the current state
  new_qd->missing_data_bytes_ = 0;
  new_qd->current_state_ = COMPLETED;

  // Advance the rd_ptr on the message block
  mb.rd_ptr (total_msg_len);

  if (TAO_debug_level > 7)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Queued_Data::make_complete_message: ")
                  ACE_TEXT ("extracted complete message (%u bytes incl hdr) from mblk=%-08x into qd=%-08x\n"),
                  total_msg_len, &mb, new_qd));
    }

  return new_qd;

failure:
  if (TAO_debug_level > 7)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Queued_Data::make_complete_message: ")
                  ACE_TEXT ("failed to find complete message in mblk=%-08x; leaving %u bytes in block\n"),
                  &mb, mb.length ()));
      if (TAO_debug_level >= 10)
        ACE_HEX_DUMP ((LM_DEBUG,
                       mb.rd_ptr (), mb.length (),
                       ACE_TEXT ("                residual bytes in buffer")));

    }
  TAO_Queued_Data::release (new_qd);
  return 0;
}

/*static*/
TAO_Queued_Data *
TAO_Queued_Data::make_queued_data (ACE_Allocator *alloc)
{
  TAO_Queued_Data *qd = 0;

  if (alloc)
    {
      ACE_NEW_MALLOC_RETURN (qd,
                            ACE_static_cast (TAO_Queued_Data *,
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
                  "TAO (%P|%t) - Queued_Data::get_queued_data\n",
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
                             ACE_static_cast(TAO_Queued_Data *,
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
