// $Id$

ACE_INLINE
TAO_LF_Follower_Auto_Adder::
    TAO_LF_Follower_Auto_Adder (TAO_Leader_Follower &lf,
                                TAO_LF_Follower *follower)
  : leader_follower_ (lf)
  , follower_ (follower)
{
  (void) this->leader_follower_.add_follower (this->follower_);
}
