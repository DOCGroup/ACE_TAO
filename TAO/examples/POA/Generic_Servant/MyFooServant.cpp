// $Id$

// ================================================================
//
//
// = FILENAME
//     MyFooServant.cpp
//
// = DESCRIPTION
//     This is a simple foo servant implementation
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "MyFooServant.h"

ACE_RCSID(Generic_Servant, MyFooServant, "$Id$")

// Constructor
MyFooServant::MyFooServant (CORBA::ORB_ptr orb,
			    PortableServer::POA_ptr poa,
                            CORBA::Long value)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    value_ (value)
{
}

// Destructor
MyFooServant::~MyFooServant (void)
{
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
MyFooServant::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Long
MyFooServant::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}

void
MyFooServant::simply_doit (CORBA::Environment &/*env*/)
{
}

void
MyFooServant::shutdown (CORBA::Environment &env)
{
  this->orb_->shutdown ();
}
