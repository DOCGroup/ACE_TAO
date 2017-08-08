// -*- C++ -*-
#include "tao/LF_Invocation_Event.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_Invocation_Event::TAO_LF_Invocation_Event (void)
  : TAO_LF_Event ()
{
}

TAO_LF_Invocation_Event::~TAO_LF_Invocation_Event (void)
{
}

void
TAO_LF_Invocation_Event::state_changed_i (LFS_STATE new_state)
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

bool
TAO_LF_Invocation_Event::successful_i (void) const
{
  return this->state_ == TAO_LF_Event::LFS_SUCCESS;
}

bool
TAO_LF_Invocation_Event::error_detected_i (void) const
{
  return (this->state_ == TAO_LF_Event::LFS_FAILURE
          || this->state_ == TAO_LF_Event::LFS_TIMEOUT
          || this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED);
}

bool
TAO_LF_Invocation_Event::is_state_final (void) const
{
  if (this->state_ == TAO_LF_Event::LFS_TIMEOUT ||
      this->state_ == TAO_LF_Event::LFS_FAILURE)
    return true;

  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
