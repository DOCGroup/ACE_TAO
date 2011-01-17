// $Id$

#include "tao/LF_Follower_Auto_Adder.h"

#if !defined (__ACE_INLINE__)
# include "tao/LF_Follower_Auto_Adder.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_Follower_Auto_Adder::~TAO_LF_Follower_Auto_Adder (void)
{
  this->leader_follower_.remove_follower (this->follower_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
