// -*- C++ -*-
// $Id$
ACE_INLINE int
TAO_LF_Event::is_state_final (void)
{
  if (this->state_ == TAO_LF_Event::LFS_TIMEOUT ||
      this->state_ == TAO_LF_Event::LFS_FAILURE ||
      this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED)
    return 1;

  return 0;
}
