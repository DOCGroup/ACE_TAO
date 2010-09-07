// $Id$

#include "tao/Queued_Message.h"

#if !defined (__ACE_INLINE__)
# include "tao/Queued_Message.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Queued_Message::TAO_Queued_Message (TAO_ORB_Core *oc,
                                        ACE_Allocator *alloc,
                                        bool is_heap_allocated)
  : allocator_ (alloc)
  , is_heap_created_ (is_heap_allocated)
  , orb_core_ (oc)
  , next_ (0)
  , prev_ (0)
{
}

TAO_Queued_Message::~TAO_Queued_Message (void)
{
}

void
TAO_Queued_Message::remove_from_list (TAO_Queued_Message *&head,
                                      TAO_Queued_Message *&tail)
{
  if (this->prev_ != 0)
    {
      this->prev_->next_ = this->next_;
    }
  else if(head == this)
    {
      head = this->next_;
    }

  if (this->next_ != 0)
    {
      this->next_->prev_ = this->prev_;
    }
  else if(tail == this)
    {
      tail = this->prev_;
    }

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
    }
  else
    {
      tail->next_ = this;
      this->prev_ = tail;
      this->next_ = 0;
      tail = this;
    }
}

void
TAO_Queued_Message::push_front (TAO_Queued_Message *&head,
                                TAO_Queued_Message *&tail)
{
  if (head == 0)
    {
      tail = this;
      head = this;
      this->next_ = 0;
      this->prev_ = 0;
    }
  else
    {
      head->prev_ = this;
      this->next_ = head;
      this->prev_ = 0;
      head = this;
    }
}

bool
TAO_Queued_Message::is_expired (const ACE_Time_Value &) const
{
  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
