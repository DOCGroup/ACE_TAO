// -*- C++ -*-
#include "Foo_i.h"


Foo_i::Foo_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Foo_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,"SUCCESS: server received expected shutdown\n"));
  this->orb_->shutdown (false);
}
