// -*- C++ -*-
// $Id$

#include "tao/LF_Event.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/LF_Event.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, LF_Event, "$Id$")

TAO_LF_Event::TAO_LF_Event (void)
  : state_ (TAO_LF_Event::LFS_IDLE)
  , follower_ (0)
{
}

TAO_LF_Event::~TAO_LF_Event (void)
{
}

void
TAO_LF_Event::state_changed (int new_state)
{
  if (this->follower_ == 0)
    {
      this->state_changed_i (new_state);
    }
  else
    {
      TAO_Leader_Follower &leader_follower =
        this->follower_->leader_follower ();

      ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, leader_follower.lock ());

      if (is_state_final ()== 0 &&
          this->follower_ != 0)
        {
          this->state_changed_i (new_state);

          this->follower_->signal ();
        }
    }
}

void
TAO_LF_Event::state_changed_i (int new_state)
{
  if (this->state_ == new_state)
    return;

  // Validate the state change
  if (this->state_ == TAO_LF_Event::LFS_IDLE)
    {
      // From the LFS_IDLE state we can only become active.
      if (new_state == TAO_LF_Event::LFS_ACTIVE
          || new_state == TAO_LF_Event::LFS_CONNECTION_CLOSED)
        this->state_ = new_state;
      return;
    }
  else if (this->state_ == TAO_LF_Event::LFS_ACTIVE)
    {
      // From LFS_ACTIVE we can only move to a few states
      if (new_state != TAO_LF_Event::LFS_IDLE)
        {
          if (new_state == TAO_LF_Event::LFS_CONNECTION_CLOSED)
            {
              this->state_ = TAO_LF_Event::LFS_FAILURE;
            }
          else
            {
              this->state_ = new_state;
            }
        }
      return;
    }
  else if (this->state_ == TAO_LF_Event::LFS_SUCCESS
           || this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED)
    {
      // From the two states above we can go back to ACTIVE, as when a
      // request is restarted.
      if (new_state == TAO_LF_Event::LFS_ACTIVE)
        {
          this->state_ = new_state;
        }
      return;
    }
  else /* if (this->state_ == TAO_LF_Event::LFS_TIMEOUT || FAILURE ) */
    {
      // Other states are final...
    }
}

int
TAO_LF_Event::successful (void) const
{
  return this->state_ == TAO_LF_Event::LFS_SUCCESS;
}

int
TAO_LF_Event::error_detected (void) const
{
  return (this->state_ == TAO_LF_Event::LFS_FAILURE
          || this->state_ == TAO_LF_Event::LFS_TIMEOUT
          || this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED);
}
