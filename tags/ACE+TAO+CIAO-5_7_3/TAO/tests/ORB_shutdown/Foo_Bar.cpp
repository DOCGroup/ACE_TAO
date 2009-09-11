//
// $Id$
//
#include "Foo_Bar.h"

ACE_RCSID(Foo_Bar, Foo_Bar, "$Id$")

Foo_Bar::Foo_Bar (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Foo_Bar::~Foo_Bar (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Shutting down the ORB again\n"));

  this->orb_->shutdown (1);
}
