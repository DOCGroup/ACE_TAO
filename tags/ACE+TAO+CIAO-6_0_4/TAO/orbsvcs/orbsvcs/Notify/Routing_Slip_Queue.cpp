// $Id$

#include "orbsvcs/Notify/Routing_Slip_Queue.h"

#include "tao/debug.h"
#include "ace/Dynamic_Service.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  Routing_Slip_Queue::Routing_Slip_Queue (size_t allowed)
    : allowed_ (allowed)
    , active_ (0)
  {
  }

  Routing_Slip_Queue::~Routing_Slip_Queue ()
  {
  }

  void
  Routing_Slip_Queue::add (const Routing_Slip_Ptr & routing_slip)
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
    if (this->allowed_ == 0)
    {
      ++this->active_;
      guard.release ();
      routing_slip->at_front_of_persist_queue ();
    }
    else
    {
      this->queue_.enqueue_tail (routing_slip);
      dispatch (guard);
    }
  }

  void Routing_Slip_Queue::complete ()
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
    ACE_ASSERT (this->active_ > 0);
    --this->active_;
    dispatch (guard);
  }

  void
  Routing_Slip_Queue::dispatch (Guard & guard)
  {
    // we start out pretty nice,
    // but the more work we do for other people
    // the less nice we get.
    size_t nice = this->allowed_ + 1;
    while (nice > 0 && (this->active_ < this->allowed_))
    {
      if (dispatch_one (guard))
      {
        --nice;
      }
      else
      {
        // that's about as nice as I get.
        nice = 0;
      }
    }
  }

  bool
  Routing_Slip_Queue::dispatch_one (Guard & guard)
  {
    bool ok = false;
    Routing_Slip_Ptr routing_slip;
    if (this->queue_.dequeue_head (routing_slip) == 0)
    {
      ++this->active_;
      guard.release ();
      routing_slip->at_front_of_persist_queue ();
      guard.acquire ();
    }
    return ok;
  }

  void
  Routing_Slip_Queue::set_allowed (size_t allowed)
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
    size_t const allowed_was = this->allowed_;
    this->allowed_ = allowed;
    if (allowed == 0 && allowed_was != 0)
    {
      while (dispatch_one (guard))
      {
        ; // work happens in dispatc_one
      }
    }
    else
    {
      dispatch (guard);
    }
  }
} // namespace

TAO_END_VERSIONED_NAMESPACE_DECL
