#include "tao/LF_Multi_Event.h"
#include "tao/Connection_Handler.h"

#include <cerrno>
#include <new>
#include <utility>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_Multi_Event::TAO_LF_Multi_Event () = default;

TAO_LF_Multi_Event::~TAO_LF_Multi_Event ()
{
  while (this->events_)
    {
      std::unique_ptr<Event_Node> event = std::move (this->events_);
      this->events_ = std::move (event->next_);
    }
}

int
TAO_LF_Multi_Event::bind (TAO_LF_Follower *follower)
{
  if (this->TAO_LF_Event::bind (follower) == -1)
    {
      return -1;
    }

  for (Event_Node *n = this->events_.get (); n != nullptr; n = n->next_.get ())
    if (n->ptr_->bind (follower) == -1)
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

  for (Event_Node *n = this->events_.get (); n != nullptr; n = n->next_.get ())
    if (n->ptr_->unbind (follower) == -1)
      {
        return -1;
      }
  return 0;
}

void
TAO_LF_Multi_Event::add_event (TAO_Connection_Handler *ev)
{
  std::unique_ptr<Event_Node> node (new (std::nothrow) Event_Node);
  if (!node)
    {
      errno = ENOMEM;
      return;
    }

  node->ptr_ = ev;
  node->next_ = std::move (this->events_);

  this->events_ = std::move (node);
}

TAO_Connection_Handler *
TAO_LF_Multi_Event::winner ()
{
  return this->winner_;
}

TAO_Transport *
TAO_LF_Multi_Event::base_transport ()
{
  return this->events_ ? this->events_->ptr_->transport () : nullptr;
}

bool
TAO_LF_Multi_Event::successful_i () const
{
  for (Event_Node *n = this->events_.get (); n != nullptr; n = n->next_.get ())
    if (n->ptr_->successful_i ())
      {
        this->winner_ = n->ptr_;
        return true;
      }
  return false;
}

bool
TAO_LF_Multi_Event::error_detected_i () const
{
  for (Event_Node *n = this->events_.get (); n != nullptr; n = n->next_.get ())
    {
      if (!n->ptr_->error_detected_i ())
        {
          return false;
        }
    }
  return true;
}

void
TAO_LF_Multi_Event::state_changed_i (LFS_STATE)
{
}

bool
TAO_LF_Multi_Event::is_state_final () const
{
  for (Event_Node *n = this->events_.get (); n != nullptr; n = n->next_.get ())
    {
      if (!n->ptr_->is_state_final ())
        {
          return false;
        }
    }
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
