// -*- C++ -*-
// $Id$

#include "Queued_Message.h"
#include "Message_Sent_Callback.h"

#if !defined (__ACE_INLINE__)
# include "Queued_Message.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Queued_Message, "$Id$")

TAO_Queued_Message::TAO_Queued_Message (TAO_Message_Sent_Callback *callback)
  : data_sent_successfully_ (0)
  , connection_closed_ (0)
  , callback_ (callback)
  , next_ (0)
  , prev_ (0)
{
}

TAO_Queued_Message::~TAO_Queued_Message (void)
{
}

void
TAO_Queued_Message::connection_closed (void)
{
  this->connection_closed_ = 1;

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
