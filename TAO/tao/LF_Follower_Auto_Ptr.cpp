// -*- C++ -*-
// $Id$

#include "tao/LF_Follower_Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/LF_Follower_Auto_Ptr.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, LF_Follower_Auto_Ptr, "$Id$")

TAO_LF_Follower_Auto_Ptr::~TAO_LF_Follower_Auto_Ptr (void)
{
  this->leader_follower_.release_follower (this->follower_);
}
