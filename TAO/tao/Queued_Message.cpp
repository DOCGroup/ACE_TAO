// -*- C++ -*-
// $Id$

#include "Queued_Message.h"
#include "Message_Sent_Callback.h"

#if !defined (__ACE_INLINE__)
# include "Queued_Message.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Queued_Message, "$Id$")

TAO_Queued_Message::TAO_Queued_Message (ACE_Message_Block *contents,
                                        int own_contents,
                                        TAO_Message_Sent_Callback *callback)
  : contents_ (contents)
  , own_contents_ (own_contents)
  , callback_ (callback)
  , current_block_ (contents)
  , next_ (0)
  , prev_ (0)
{
}

TAO_Queued_Message::~TAO_Queued_Message (void)
{
  if (this->own_contents_)
    {
      ACE_Message_Block *i = this->contents_;
      while (i != 0)
        {
          ACE_Message_Block *cont = i->cont (); i->cont (0);
          ACE_Message_Block::release (i);
          i = cont;
        }
    }
}

void
TAO_Queued_Message::connection_closed (void)
{
  if (this->callback_ != 0)
    {
      if (this->done ())
        {
          this->callback_->connection_closed ();
        }
      else
        {
          this->callback_->send_failed ();
        }
    }
}

void
TAO_Queued_Message::destroy (void)
{
  delete this;
}

void
TAO_Queued_Message::bytes_transferred (size_t byte_count)
{
  while (!this->done () && byte_count > 0)
    {
      size_t l = this->current_block_->length ();
      if (byte_count < l)
        {
          this->current_block_->rd_ptr (byte_count);
          return;
        }
      byte_count -= l;
      this->current_block_ = this->current_block_->cont ();
    }
}

void
TAO_Queued_Message::remove_from_list (TAO_Queued_Message *&head,
                                      TAO_Queued_Message *&tail)
{
  if (this->prev_ != 0)
    this->prev_->next_ = this->next_;
  else
    head = this->next_;

  if (this->next_ != 0)
    this->next_->prev_ = this->prev_;
  else
    tail = this->prev_;

  this->next_ = 0;
  this->prev_ = 0;
}

void
TAO_Queued_Message::push_back (TAO_Queued_Message *&head,
                               TAO_Queued_Message *&tail)
{
  if (tail == 0)
    {
      tail = this;
      head = this;
      this->next_ = 0;
      this->prev_ = 0;
      return;
    }

  tail->next_ = this;
  this->prev_ = tail;
  this->next_ = 0;
  tail = this;
}
