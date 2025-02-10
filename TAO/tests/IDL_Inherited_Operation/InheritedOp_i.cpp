#include "InheritedOp_i.h"

Derived_i::Derived_i (CORBA::ORB_ptr orb)
  : orb_ (orb)
{
}

void
Derived_i::BaseOp ()
{
  ACE_DEBUG ((LM_DEBUG, "BaseOp upcall successful\n"));
}

void
Derived_i::shutdown ()
{
  orb_->shutdown ();
}
