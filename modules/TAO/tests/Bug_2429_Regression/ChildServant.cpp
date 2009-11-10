// $Id$

#include "ChildServant.h"

ChildServant::ChildServant(CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate(orb))
{
}

void
ChildServant::childMethod ()
{
   // No-op
}

void
ChildServant::parentMethod ()
{
   // Just throw an exception
   throw CORBA::INTERNAL ();
}

void
ChildServant::shutdown ()
{
   this->orb_->shutdown(0);
}
