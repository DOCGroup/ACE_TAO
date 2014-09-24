// $Id$

#include "tao/LF_Multi_Event.h"
#include "tao/Connection_Handler.h"
#include "ace/OS_Memory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_LF_Multi_Event::TAO_LF_Multi_Event (void)
  : TAO_LF_Event (),
    events_ (0),
    winner_ (0)
{
}

TAO_LF_Multi_Event::~TAO_LF_Multi_Event (void)
{
  while (this->events_ != 0)
    {
      Event_Node *n = this->events_->next_;
      delete this->events_;
      this->events_ = n;
    }
}

int
TAO_LF_Multi_Event::bind (TAO_LF_Follower *follower)
{
  if (this->TAO_LF_Event::bind(follower) == -1)
    {
      return -1;
    }

  for (Event_Node *n = this->events_; n != 0; n = n->next_)
    if (n->ptr_->bind(follower) == -1)
      {
        return -1;
      }
  return 0;
}

int
TAO_LF_Multi_Event::unbind (TAO_LF_Follower *follower)
{
  if (this->TAO_LF_Event::unbind (follower) == -1)
    {
      return -1;
    }

  for (Event_Node *n = this->events_; n != 0; n = n->next_)
    if (n->ptr_->unbind (follower) == -1)
      {
        return -1;
      }
  return 0;
}


void
TAO_LF_Multi_Event::add_event (TAO_Connection_Handler *ev)
{
  Event_Node *node = 0;
  ACE_NEW (node, Event_Node);
  node->next_ = this->events_;
  node->ptr_ = ev;

  this->events_ = node;
}

TAO_Connection_Handler*
TAO_LF_Multi_Event::winner (void)
{
  return this->winner_;
}

TAO_Transport *
TAO_LF_Multi_Event::base_transport (void)
{
  return (this->events_ == 0) ? 0 : this->events_->ptr_->transport();
}

int
TAO_LF_Multi_Event::successful (void) const
{
  for (Event_Node *n = this->events_; n != 0; n = n->next_)
    if (n->ptr_->successful() == 1)
      {
        this->winner_ = n->ptr_;
        return 1;
      }
  return 0;
}

int
TAO_LF_Multi_Event::error_detected (void) const
{
  int result = 1;
  for (Event_Node *n = this->events_; n != 0; n = n->next_)
    if (n->ptr_->error_detected () == 0)
      result = 0;
  return result;
}

void
TAO_LF_Multi_Event::state_changed_i (int )
{
  // no-op
}

int
TAO_LF_Multi_Event::is_state_final (void)
{
  int result = 1;
  for (Event_Node *n = this->events_; n != 0; n = n->next_)
    if (n->ptr_->is_state_final () == 0)
      result = 0;
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
