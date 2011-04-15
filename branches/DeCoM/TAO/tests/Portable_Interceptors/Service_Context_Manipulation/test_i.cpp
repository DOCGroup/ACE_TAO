// $Id$

#include "test_i.h"

Visual_i::Visual_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Visual_i::normal (CORBA::Long arg)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::normal called with %d\n", arg));
}

void
Visual_i::nothing (void)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::nothing\n"));
}

void
Visual_i::user (void)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing Silly\n"));
  throw Test_Interceptors::Silly ();
}

void
Visual_i::system (void)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing INV_OBJREF\n"));
  throw CORBA::INV_OBJREF ();
}

void
Visual_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
