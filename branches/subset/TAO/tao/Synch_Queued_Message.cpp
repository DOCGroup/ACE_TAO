// "$Id$"

#include "Synch_Queued_Message.h"
#include "debug.h"
#include "ace/Malloc_T.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"


ACE_RCSID (tao,
           Synch_Queued_Message,
           "$Id$")


TAO_Synch_Queued_Message::
    TAO_Synch_Queued_Message (const ACE_Message_Block *contents,
                              ACE_Allocator *alloc,
                              int is_heap_allocated)
  : TAO_Queued_Message (alloc, is_heap_allocated)
  , contents_ (ACE_const_cast (ACE_Message_Block*,contents))
  , current_block_ (contents_)
{
}

TAO_Synch_Queued_Message::~TAO_Synch_Queued_Message (void)
{

}

const ACE_Message_Block *
TAO_Synch_Queued_Message::current_block (void) const
{
  return this->current_block_;
}

size_t
TAO_Synch_Queued_Message::message_length (void) const
{
  if (this->current_block_ == 0)
    return 0;
  return this->current_block_->total_length ();
}

int
TAO_Synch_Queued_Message::all_data_sent (void) const
{
  return this->current_block_ == 0;
}

void
TAO_Synch_Queued_Message::fill_iov (int iovcnt_max,
                                    int &iovcnt,
                                    iovec iov[]) const
{
  ACE_ASSERT (iovcnt_max > iovcnt);

  for (const ACE_Message_Block *message_block = this->current_block_;
       message_block != 0 && iovcnt < iovcnt_max;
       message_block = message_block->cont ())
    {
      size_t message_block_length = message_block->length ();

      // Check if this block has any data to be sent.
      if (message_block_length > 0)
        {
              // Collect the data in the iovec.
          iov[iovcnt].iov_base = message_block->rd_ptr ();
          iov[iovcnt].iov_len  = message_block_length;

          // Increment iovec counter.
          iovcnt++;
        }
    }
}

void
TAO_Synch_Queued_Message::bytes_transferred (size_t &byte_count)
{
  this->state_changed_i (TAO_LF_Event::LFS_ACTIVE);

  while (this->current_block_ != 0 && byte_count > 0)
    {
      size_t l = this->current_block_->length ();

      if (byte_count < l)
        {
          this->current_block_->rd_ptr (byte_count);
          byte_count = 0;
          return;
        }
      byte_count -= l;
      this->current_block_->rd_ptr (l);
      this->current_block_ = this->current_block_->cont ();
      while (this->current_block_ != 0
             && this->current_block_->length () == 0)
        {
          this->current_block_ = this->current_block_->cont ();
        }
    }
  if (this->current_block_ == 0)
    this->state_changed (TAO_LF_Event::LFS_SUCCESS);
}

TAO_Queued_Message *
TAO_Synch_Queued_Message::clone (ACE_Allocator *alloc)
{
  TAO_Synch_Queued_Message *qm = 0;

  // Clone the message block.
  // NOTE: We wantedly do the cloning from <current_block_> instead of
  // starting from <contents_> since we dont want to clone blocks that
  // have already been sent on the wire. Waste of memory and
  // associated copying.
  ACE_Message_Block *mb =
    this->current_block_->clone ();

  if (alloc)
    {
      ACE_NEW_MALLOC_RETURN (qm,
                             ACE_static_cast (TAO_Synch_Queued_Message *,
                                 alloc->malloc (sizeof (TAO_Synch_Queued_Message))),
                             TAO_Synch_Queued_Message (mb,
                                                       alloc),
                             0);
    }
  else
    {
      // No allocator, so use the common heap!
      if (TAO_debug_level == 4)
        {
          // This debug is for testing purposes!
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Synch_Queued_Message::clone\n",
                      "Using global pool for allocation \n"));
        }

      ACE_NEW_RETURN (qm,
                      TAO_Synch_Queued_Message (mb),
                      0);
    }

  // Set the flag to indicate that <qm> is created on the heap.
  if (qm)
    qm->is_heap_created_ = 1;

  return qm;
}

void
TAO_Synch_Queued_Message::destroy (void)
{
  if (this->is_heap_created_)
    {
      ACE_Message_Block::release (this->contents_);
      this->current_block_ = 0;

      // If we have an allocator release the memory to the allocator
      // pool.
      if (this->allocator_)
        {
          ACE_DES_FREE (this,
                        this->allocator_->free,
                        TAO_Synch_Queued_Message);

        }
      else // global release..
        {
          delete this;
        }
    }
}
