// -*- C++ -*-
// $Id$

#include "Synch_Queued_Message.h"

ACE_RCSID(tao, Synch_Queued_Message, "$Id$")

TAO_Synch_Queued_Message::
    TAO_Synch_Queued_Message (const ACE_Message_Block *contents)
  : contents_ (ACE_const_cast (ACE_Message_Block*,contents))
  , current_block_ (contents_)
{
}

TAO_Synch_Queued_Message::~TAO_Synch_Queued_Message (void)
{
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

int
TAO_Synch_Queued_Message::bytes_transferred (size_t &byte_count)
{
  while (this->current_block_ != 0 && byte_count > 0)
    {
      size_t l = this->current_block_->length ();
      if (byte_count < l)
        {
          this->current_block_->rd_ptr (byte_count);
          return 0;
        }
      byte_count -= l;
      this->current_block_ = this->current_block_->cont ();
    }
  return (this->current_block_ == 0);
}

void
TAO_Synch_Queued_Message::destroy (void)
{
}
