// $Id$

#include "tao/Reactor_Registry.h"
#include "tao/ORB_Core.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/Reactor_Registry.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Reactor_Registry, "$Id$")

TAO_Reactor_Registry::TAO_Reactor_Registry (void)
  :  orb_core_ (0)
  ,  lf_strategy_ (0)
{
}

void
TAO_Reactor_Registry::open (TAO_ORB_Core* orb_core)
{
  this->orb_core_ = orb_core;

  this->lf_strategy_ = orb_core->resource_factory ()->create_lf_strategy ();
}

TAO_Reactor_Registry::~TAO_Reactor_Registry (void)
{
  delete this->lf_strategy_;
}


TAO_LF_Strategy &
TAO_Reactor_Registry::lf_strategy (void)
{
  return *this->lf_strategy_;
}
