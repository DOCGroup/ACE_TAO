// $Id$

#include "ChildServant.h"

ChildServant::ChildServant(CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate(orb))
{
}

void
ChildServant::childMethod ()
ACE_THROW_SPEC ((CORBA::SystemException))
{
   // No-op
}

void
ChildServant::parentMethod ()
ACE_THROW_SPEC((CORBA::SystemException))
{
   // Just throw an exception
   ACE_THROW (CORBA::INTERNAL ());
}

void
ChildServant::shutdown ()
ACE_THROW_SPEC((CORBA::SystemException))
{
   this->orb_->shutdown(0 ACE_ENV_ARG_PARAMETER);
}
