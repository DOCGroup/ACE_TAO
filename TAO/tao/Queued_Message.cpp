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
  , next_ (nullptr)
  , prev_ (nullptr)
{
}

TAO_Queued_Message::~TAO_Queued_Message ()
{
}

void
TAO_Queued_Message::remove_from_list (TAO_Queued_Message *&head,
                                      TAO_Queued_Message *&tail)
{
  if (this->prev_ != nullptr)
    {
      this->prev_->next_ = this->next_;
    }
  else if(head == this)
    {
      head = this->next_;
    }

  if (this->next_ != nullptr)
    {
      this->next_->prev_ = this->prev_;
    }
  else if(tail == this)
    {
      tail = this->prev_;
    }

  this->next_ = nullptr;
  this->prev_ = nullptr;
}

void
TAO_Queued_Message::push_back (TAO_Queued_Message *&head,
                               TAO_Queued_Message *&tail)
{
  if (tail == nullptr)
    {
      tail = this;
      head = this;
      this->next_ = nullptr;
      this->prev_ = nullptr;
    }
  else
    {
      tail->next_ = this;
      this->prev_ = tail;
      this->next_ = nullptr;
      tail = this;
    }
}

void
TAO_Queued_Message::push_front (TAO_Queued_Message *&head,
                                TAO_Queued_Message *&tail)
{
  if (head == nullptr)
    {
      tail = this;
      head = this;
      this->next_ = nullptr;
      this->prev_ = nullptr;
    }
  else
    {
      head->prev_ = this;
      this->next_ = head;
      this->prev_ = nullptr;
      head = this;
    }
}

bool
TAO_Queued_Message::is_expired (const ACE_Time_Value &) const
{
  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
