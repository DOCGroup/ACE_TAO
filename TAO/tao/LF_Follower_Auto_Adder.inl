// $Id$

ACE_INLINE
TAO_LF_Follower_Auto_Adder::
    TAO_LF_Follower_Auto_Adder (TAO_Leader_Follower &lf,
                                TAO_Follower *follower)
  : leader_follower_ (lf)
  , follower_ (follower)
{
  (void) this->leader_follower_.add_follower (this->follower_);
}

ACE_INLINE TAO_Follower *
TAO_LF_Follower_Auto_Adder::get (void)
{
  return this->follower_;
}

ACE_INLINE TAO_Follower *
TAO_LF_Follower_Auto_Adder::operator-> (void)
{
  return this->follower_;
}

ACE_INLINE
TAO_LF_Follower_Auto_Adder::operator TAO_Follower * (void)
{
  return this->follower_;
}
