// We're doing this because we need to extend ACE_Message_Block, but
// that interface is currently "frozen".  Therefore, I'm using static
// functions in this compilation unit that I would normally add as part
// of the interface.
#define private public
#define protected public
#include "ace/Message_Block.h"
#undef private
#undef public

#include "Incoming_Message_Queue.h"
#include "ORB_Core.h"
#include "debug.h"
#include "ace/Malloc_T.h"

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

/*static*/
TAO_Queued_Data *
TAO_Queued_Data::make_uncompleted_message (ACE_Message_Block *mb,
                                           TAO_Pluggable_Messaging &msging_obj,
                                           ACE_Allocator *alloc)
{
  TAO_Queued_Data *new_qd = 0;

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
          new_qd->msg_block_ = mb;
          msging_obj.set_queued_data_from_message_header (new_qd, *mb);
          // missing_data_bytes_ now has the full GIOP message size, but
          // there might still be stuff in mb.  Therefore, we have to adjust
          // missing_data_bytes_, i.e., decrease it by the number of "actual
          // payload bytes" in mb.
          //
          // "actual payload bytes" :== length of mb (which included the header) - header length
          new_qd->missing_data_bytes_ -= (mb->length () - msging_obj.header_length ());
          //???          mb->rd_ptr (msging_obj.header_length ());
        }
    }
  else
    {
      new_qd->current_state_ = WAITING_TO_COMPLETE_HEADER;
      new_qd->msg_block_ = mb;
      new_qd->missing_data_bytes_ = msging_obj.header_length () - mb->length ();
    }

  ACE_ASSERT (new_qd->current_state_ != INVALID);
  if (TAO_debug_level > 7)
    {
      const char* s = "?unk?";
      switch (new_qd->current_state_) {
      case WAITING_TO_COMPLETE_HEADER: s = "WAITING_TO_COMPLETE_HEADER"; break;
      case WAITING_TO_COMPLETE_PAYLOAD: s = "WAITING_TO_COMPLETE_PAYLOAD"; break;
      case INVALID: s = "INVALID"; break;
      case COMPLETED: s = "COMPLETED"; break;
      }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Queued_Data::make_uncompleted_message: ")
                  ACE_TEXT ("made uncompleted message from %u bytes into qd=%-08x:")
                  ACE_TEXT ("state=%s,missing_data_bytes=%u\n"),
                  mb->length(), new_qd, s, new_qd->missing_data_bytes_));
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
  if ((new_qd->msg_block_ = clone_span (&mb, total_msg_len)) == 0)
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
