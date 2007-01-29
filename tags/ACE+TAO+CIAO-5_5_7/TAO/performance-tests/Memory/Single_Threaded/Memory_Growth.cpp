//
// $Id$
//
#include "Memory_Growth.h"

ACE_RCSID(Hello, Hello, "$Id$")

Memory_Growth::Memory_Growth (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Memory_Growth::ping (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

void
Memory_Growth::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
