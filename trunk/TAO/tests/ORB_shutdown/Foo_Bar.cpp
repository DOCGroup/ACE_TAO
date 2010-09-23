// $Id$

#include "Foo_Bar.h"

Foo_Bar::Foo_Bar (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Foo_Bar::~Foo_Bar (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Shutting down the ORB again\n"));

  try
    {
      this->orb_->shutdown (1);
    }
  catch (...)
    {
      // Don't allow exceptions to leave the destructor.
    }
}
