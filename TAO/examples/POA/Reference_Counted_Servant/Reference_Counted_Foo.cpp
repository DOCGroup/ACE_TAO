// $Id$

// ================================================================
//
// = FILENAME
//     Reference_Counted_Foo.cpp
//
// = DESCRIPTION
//     This is a simple foo servant implementation
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "Reference_Counted_Foo.h"

ACE_RCSID(Generic_Servant, Reference_Counted_Foo, "$Id$")

// Constructor
Reference_Counted_Foo::Reference_Counted_Foo (CORBA::ORB_ptr orb,
                                              PortableServer::POA_ptr poa,
                                              CORBA::Long value)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    value_ (value)
{
}

// Destructor
Reference_Counted_Foo::~Reference_Counted_Foo (void)
{
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
Reference_Counted_Foo::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Long
Reference_Counted_Foo::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}

void
Reference_Counted_Foo::simply_doit (CORBA::Environment &/*env*/)
{
}

void
Reference_Counted_Foo::shutdown (CORBA::Environment &)
{
  this->orb_->shutdown ();
}
