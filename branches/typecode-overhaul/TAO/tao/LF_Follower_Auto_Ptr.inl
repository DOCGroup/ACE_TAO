// $Id$

ACE_INLINE
TAO_LF_Follower_Auto_Ptr::TAO_LF_Follower_Auto_Ptr (TAO_Leader_Follower &lf)
  : leader_follower_ (lf)
  , follower_ (leader_follower_.allocate_follower ())
{
}

ACE_INLINE TAO_LF_Follower *
TAO_LF_Follower_Auto_Ptr::get (void)
{
  return this->follower_;
}

ACE_INLINE TAO_LF_Follower *
TAO_LF_Follower_Auto_Ptr::operator-> (void)
{
  return this->follower_;
}

ACE_INLINE
TAO_LF_Follower_Auto_Ptr::operator TAO_LF_Follower * (void)
{
  return this->follower_;
}
