// $Id$

//===============================================================================
//
//
// = FILENAME
//     Foo_i.cpp
//
// = DESCRIPTION
//     This is a simple foo servant implementation
//
// = AUTHOR
//     Irfan Pyarali
//
//==================================================================================

#include "Foo_i.h"

// Constructor
Simple_Foo_i::Simple_Foo_i (CORBA::Long value)
  : value_ (value)
{
}

// Return this->value
CORBA::Long
Simple_Foo_i::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}

// Constructor
Foo_i::Foo_i (CORBA::Long value,
              PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

// Return this->value
CORBA::Long
Foo_i::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
Foo_i::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// Constructor
Outer_i::Foo_i::Foo_i (CORBA::Long value,
                       PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

// Return this->value
CORBA::Long
Outer_i::Foo_i::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
Outer_i::Foo_i::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// Constructor
Outer_i::Inner_i::Foo_i::Foo_i (CORBA::Long value,
                                PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

// Return this->value
CORBA::Long
Outer_i::Inner_i::Foo_i::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
Outer_i::Inner_i::Foo_i::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

