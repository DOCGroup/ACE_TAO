// -*- C++ -*-
// $Id$

#include "tao/LF_Event.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "ace/Guard_T.h"

#if !defined (__ACE_INLINE__)
# include "tao/LF_Event.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_Event::TAO_LF_Event (void)
  : state_ (TAO_LF_Event::LFS_IDLE)
  , follower_ (0)
{
}

TAO_LF_Event::~TAO_LF_Event (void)
{
}

void
TAO_LF_Event::state_changed (int new_state, TAO_Leader_Follower &lf)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, lf.lock ());

  if (this->is_state_final () == 0)
    {
      this->state_changed_i (new_state);

      /// Sort of double-checked optimization..
      if (this->follower_ != 0)
        this->follower_->signal ();
    }
}

void
TAO_LF_Event::set_state (int new_state)
{
  this->state_ = new_state;
}

TAO_END_VERSIONED_NAMESPACE_DECL
