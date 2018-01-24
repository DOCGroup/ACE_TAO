#include "tao/LF_CH_Event.h"
#include "tao/LF_Follower.h"
#include "tao/debug.h"
#include "tao/Connection_Handler.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_CH_Event::TAO_LF_CH_Event (void)
  : TAO_LF_Event (),
    prev_state_ (TAO_LF_Event::LFS_IDLE)

{
}

TAO_LF_CH_Event::~TAO_LF_CH_Event (void)
{
}

int
TAO_LF_CH_Event::bind (TAO_LF_Follower *follower)
{
  return this->followers_.bind (follower, 0);
}

int
TAO_LF_CH_Event::unbind (TAO_LF_Follower *follower)
{
  return this->followers_.unbind (follower);
}

void
TAO_LF_CH_Event::state_changed_i (LFS_STATE new_state)
{
  if (this->state_ != new_state)
    {
      this->validate_state_change (new_state);

      if (TAO_debug_level > 9)
        {
          size_t id = 0;
          TAO_Connection_Handler *ch = 0;
          if ((ch = dynamic_cast<TAO_Connection_Handler *> (this))
              && ch->transport ())
            {
              id = ch->transport ()->id ();
            }

          TAOLIB_DEBUG ((LM_DEBUG, "TAO (%P|%t) - TAO_LF_CH_Event[%d]::"
                      "state_changed_i, state %C->%C\n",
                      id,
                      TAO_LF_Event::state_name(prev_state_),
                      TAO_LF_Event::state_name(state_)));
        }
    }

  ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->followers_.mutex ()));

  HASH_MAP::iterator end_it = this->followers_.end ();
  for (HASH_MAP::iterator it = this->followers_.begin (); it != end_it ; ++it)
    {
      it->ext_id_->signal ();
    }
}

void
TAO_LF_CH_Event::validate_state_change (LFS_STATE new_state)
{
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
  else if (this->state_ == TAO_LF_Event::LFS_TIMEOUT)
    {
      if (new_state == TAO_LF_Event::LFS_CONNECTION_CLOSED)
        {
          // Dont reset the previous state
          this->state_ = new_state;
        }
    }
  return;
}


bool
TAO_LF_CH_Event::successful_i (void) const
{
  if (this->prev_state_ == TAO_LF_Event::LFS_CONNECTION_WAIT)
    return this->state_ == TAO_LF_Event::LFS_SUCCESS;

  return this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED;
}

bool
TAO_LF_CH_Event::error_detected_i (void) const
{
  if (this->prev_state_ == TAO_LF_Event::LFS_CONNECTION_WAIT)
    return this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED;

  return this->state_ == TAO_LF_Event::LFS_TIMEOUT;
}

void
TAO_LF_CH_Event::set_state (LFS_STATE new_state)
{
  // @@ NOTE: Is this still required?
  if (!this->is_state_final ()
      && new_state == TAO_LF_Event::LFS_TIMEOUT)
    {
      this->state_ = new_state;
      if (TAO_debug_level > 9)
        {
          size_t id = 0;
          TAO_Connection_Handler *ch = 0;
          if ((ch = dynamic_cast<TAO_Connection_Handler *> (this)) &&
              ch->transport ())
            {
              id = ch->transport ()->id ();
            }
          TAOLIB_DEBUG ((LM_DEBUG, "TAO (%P|%t) - TAO_LF_CH_Event[%d]::set_state, "
                      "state_ is LFS_TIMEOUT\n", id));
        }
    }
}

bool
TAO_LF_CH_Event::is_state_final (void) const
{
  return this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED;
}

TAO_END_VERSIONED_NAMESPACE_DECL
