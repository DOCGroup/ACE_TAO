// $Id$

ACE_INLINE int
TAO_LF_Event::bind (TAO_LF_Follower *follower)
{
  if (this->follower_ != 0)
    return -1;
  this->follower_ = follower;
  return 0;
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
