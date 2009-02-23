// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
TAO_LF_Event::bind (TAO_LF_Follower *follower)
{
  if (this->follower_ != 0)
    return -1;
  this->follower_ = follower;
  return 0;
}

ACE_INLINE int
TAO_LF_Event::unbind (TAO_LF_Follower *)
{
  if (this->follower_ == 0)
    return -1;
  this->follower_ = 0;
  return 0;
}

ACE_INLINE void
TAO_LF_Event::reset_state (int new_state)
{
  this->state_ = new_state;
}

ACE_INLINE int
TAO_LF_Event::keep_waiting (void)
{
  return (this->successful () == 0) && (this->error_detected () == 0);
}

ACE_INLINE const char *
TAO_LF_Event::state_name (int st)
{
#define TAO_LF_EVENT_ENTRY(X) case X: return #X
  switch (st)
    {
      TAO_LF_EVENT_ENTRY (LFS_IDLE);
      TAO_LF_EVENT_ENTRY (LFS_ACTIVE);
      TAO_LF_EVENT_ENTRY (LFS_CONNECTION_WAIT);
      TAO_LF_EVENT_ENTRY (LFS_SUCCESS);
      TAO_LF_EVENT_ENTRY (LFS_FAILURE);
      TAO_LF_EVENT_ENTRY (LFS_TIMEOUT);
      TAO_LF_EVENT_ENTRY (LFS_CONNECTION_CLOSED);
    }
  return "***Unknown enum value, update TAO_LF_Event::state_name()";
#undef TAO_CACHE_INTID_ENTRY
}

TAO_END_VERSIONED_NAMESPACE_DECL
