// $Id$

//===============================================================================
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
//==================================================================================

#include "MyFooServant.h"

// Constructor
MyFooServant::MyFooServant (PortableServer::POA_ptr poa,
                            CORBA::Long value)
  : poa_ (PortableServer::POA::_duplicate (poa)),
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

// Return this->value
CORBA::Long
MyFooServant::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}
