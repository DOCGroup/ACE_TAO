// -*- C++ -*-
// $Id$

#include "Asynch_Queued_Message.h"

ACE_RCSID(tao, Asynch_Queued_Message, "$Id$")

TAO_Asynch_Queued_Message::
    TAO_Asynch_Queued_Message (const ACE_Message_Block *contents)
  : offset_ (0)
{
  this->size_ = contents->total_length ();
  // @@ Use a pool for these guys!!
  ACE_NEW (this->buffer_, char[this->size_]);

  size_t copy_offset = 0;
  for (const ACE_Message_Block *i = contents;
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (this->buffer_ + copy_offset,
                      i->rd_ptr (),
                      i->length ());
      copy_offset += i->length ();
    }
}

TAO_Asynch_Queued_Message::~TAO_Asynch_Queued_Message (void)
{
  // @@ Use a pool for these guys!
  delete[] this->buffer_;
}

size_t
TAO_Asynch_Queued_Message::message_length (void) const
{
  return this->size_ - this->offset_;
}

int
TAO_Asynch_Queued_Message::all_data_sent (void) const
{
  return this->size_ == this->offset_;
}

void
TAO_Asynch_Queued_Message::fill_iov (int iovcnt_max,
                                     int &iovcnt,
                                     iovec iov[]) const
{
  ACE_ASSERT (iovcnt_max > iovcnt);
  ACE_UNUSED_ARG (iovcnt_max); // not used if ACE_ASSERT() is empty

  iov[iovcnt].iov_base = this->buffer_ + this->offset_;
  iov[iovcnt].iov_len  = this->size_ - this->offset_;
  iovcnt++;
}

void
TAO_Asynch_Queued_Message::bytes_transferred (size_t &byte_count)
{
  this->state_changed_i (TAO_LF_Event::LFS_ACTIVE);

  size_t remaining_bytes = this->size_ - this->offset_;
  if (byte_count > remaining_bytes)
    {
      this->offset_ = this->size_;
      byte_count -= remaining_bytes;
      return;
    }
  this->offset_ += byte_count;
  byte_count = 0;

  if (this->all_data_sent ())
    this->state_changed (TAO_LF_Event::LFS_SUCCESS);
}

void
TAO_Asynch_Queued_Message::destroy (void)
{
  // @@ Maybe it comes from a pool, we should do something about it.
  delete this;
}
