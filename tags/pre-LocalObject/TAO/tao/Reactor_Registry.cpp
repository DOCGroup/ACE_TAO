// $Id$

#include "tao/Reactor_Registry.h"

#if !defined (__ACE_INLINE__)
# include "tao/Reactor_Registry.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Reactor_Registry, "$Id$")

TAO_Reactor_Registry::TAO_Reactor_Registry (void)
  :  orb_core_ (0)
{
}

void
TAO_Reactor_Registry::open (TAO_ORB_Core* orb_core)
{
  this->orb_core_ = orb_core;
}

TAO_Reactor_Registry::~TAO_Reactor_Registry (void)
{
}
