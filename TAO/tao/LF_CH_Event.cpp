#include "LF_CH_Event.h"
#include "ace/Log_Msg.h"


#if !defined (__ACE_INLINE__)
# include "LF_CH_Event.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao,
          LF_Invocation_Event,
          "$Id$")

TAO_LF_CH_Event::TAO_LF_CH_Event (void)
  : TAO_LF_Event (),
    prev_state_ (TAO_LF_Event::LFS_IDLE)

{
}

TAO_LF_CH_Event::~TAO_LF_CH_Event (void)
{
}

void
TAO_LF_CH_Event::state_changed_i (int new_state)
{
  if (this->state_ == new_state)
    return;

    // Validate the state change
  if (this->state_ == TAO_LF_Event::LFS_IDLE)
    {
      // From the LFS_IDLE state we can only become active.
      if (new_state == TAO_LF_Event::LFS_CONNECTION_WAIT)
        {
          this->prev_state_ = this->state_;
          this->state_ = new_state;
        }
      return;
    }
  else if (this->state_ == TAO_LF_Event::LFS_CONNECTION_WAIT)
    {
      // Only a few states are possible from CONNECTION_WAIT states
      if (new_state == TAO_LF_Event::LFS_CONNECTION_CLOSED
          || new_state == TAO_LF_Event::LFS_SUCCESS)
        {
          this->prev_state_ = this->state_;
          this->state_ = new_state;
        }

      return;
    }
  else if (this->state_ == TAO_LF_Event::LFS_SUCCESS)
    {
      if (new_state == TAO_LF_Event::LFS_CONNECTION_CLOSED)
        {
          this->prev_state_ = this->state_;
          this->state_ = new_state;
        }
      return;
    }

  return;
}


int
TAO_LF_CH_Event::successful (void) const
{
  if (this->prev_state_ == TAO_LF_Event::LFS_CONNECTION_WAIT)
    return this->state_ == TAO_LF_Event::LFS_SUCCESS;
  else if (this->prev_state_ == TAO_LF_Event::LFS_SUCCESS)
    return this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED;

  return 0;
}

int
TAO_LF_CH_Event::error_detected (void) const
{
  if (this->prev_state_ == TAO_LF_Event::LFS_CONNECTION_WAIT)
    return this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED;
  else if (this->prev_state_ == TAO_LF_Event::LFS_SUCCESS)
    return (this->state_ != TAO_LF_Event::LFS_CONNECTION_CLOSED);

  return 0;
}

void
TAO_LF_CH_Event::set_state (int new_state)
{
  this->prev_state_ = this->state_;
  this->state_ = new_state;
}


int
TAO_LF_CH_Event::is_state_final (void)
{
  return this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED;
}
