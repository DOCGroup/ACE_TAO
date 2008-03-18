//
// $Id$
//
#include "tao/Monitor/Monitor_Impl.h"

ACE_RCSID(Hello, Hello, "$Id$")

Monitor_Impl::Monitor_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

