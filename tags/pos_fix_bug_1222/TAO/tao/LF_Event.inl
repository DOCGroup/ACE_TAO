// $Id$

ACE_INLINE void
TAO_LF_Event::reset_state (int new_state)
{
  this->state_ = new_state;
}

ACE_INLINE int
TAO_LF_Event::bind (TAO_LF_Follower *follower)
{
  if (this->follower_ != 0)
    return -1;
  this->follower_ = follower;
  return 0;
}

ACE_INLINE int
TAO_LF_Event::is_state_final (void)
{
  if (this->state_ == TAO_LF_Event::LFS_TIMEOUT ||
      this->state_ == TAO_LF_Event::LFS_FAILURE)
    return 1;

  return 0;
}

ACE_INLINE void
TAO_LF_Event::set_state (int new_state)
{
  this->state_ = new_state;
}

ACE_INLINE int
TAO_LF_Event::unbind (void)
{
  if (this->follower_ == 0)
    return -1;
  this->follower_ = 0;
  return 0;
}

ACE_INLINE int
TAO_LF_Event::keep_waiting (void)
{
  return (this->successful () == 0) && (this->error_detected () == 0);
}
