// -*- C++ -*-
#include "LF_Event.h"
#include "LF_Follower.h"
#include "Leader_Follower.h"
#include "ace/Guard_T.h"

#if !defined (__ACE_INLINE__)
# include "LF_Event.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_Event::TAO_LF_Event ()
  : state_ (TAO_LF_Event::LFS_IDLE)
  , follower_ (nullptr)
{
}

TAO_LF_Event::~TAO_LF_Event ()
{
}

void
TAO_LF_Event::state_changed (LFS_STATE new_state, TAO_Leader_Follower &lf)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, lf.lock ());

  if (!this->is_state_final ())
    {
      this->state_changed_i (new_state);

      /// Sort of double-checked optimization..
      if (this->follower_ != nullptr)
        this->follower_->signal ();
    }
}

bool
TAO_LF_Event::keep_waiting (TAO_Leader_Follower &lf) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, lf.lock (), false);

  return this->keep_waiting_i ();
}

bool
TAO_LF_Event::successful (TAO_Leader_Follower &lf) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, lf.lock (), false);

  return this->successful_i ();
}

bool
TAO_LF_Event::error_detected (TAO_Leader_Follower &lf) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, lf.lock (), true);

  return this->error_detected_i ();
}

void
TAO_LF_Event::set_state (LFS_STATE new_state)
{
  this->state_ = new_state;
}

TAO_END_VERSIONED_NAMESPACE_DECL
