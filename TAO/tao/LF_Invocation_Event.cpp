// -*- C++ -*-
#include "LF_Invocation_Event.h"


#if !defined (__ACE_INLINE__)
# include "tao/LF_Invocation_Event.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao,
          LF_Invocation_Event,
          "$Id$")

TAO_LF_Invocation_Event::TAO_LF_Invocation_Event (void)
  : TAO_LF_Event ()
{
}

TAO_LF_Invocation_Event::~TAO_LF_Invocation_Event (void)
{
}

void
TAO_LF_Invocation_Event::state_changed_i (int new_state)
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
  else/* if (this->state_ == TAO_LF_Event::LFS_TIMEOUT || FAILURE */
    {
      // Other states are final..
    }

}

int
TAO_LF_Invocation_Event::successful (void) const
{
  return this->state_ == TAO_LF_Event::LFS_SUCCESS;
}

int
TAO_LF_Invocation_Event::error_detected (void) const
{
  return (this->state_ == TAO_LF_Event::LFS_FAILURE
          || this->state_ == TAO_LF_Event::LFS_TIMEOUT
          || this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED);
}

int
TAO_LF_Invocation_Event::is_state_final (void)
{
  if (this->state_ == TAO_LF_Event::LFS_TIMEOUT ||
      this->state_ == TAO_LF_Event::LFS_FAILURE)
    return 1;

  return 0;
}
