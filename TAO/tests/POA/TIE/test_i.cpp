
//=============================================================================
/**
 *  @file     test_i.cpp
 *
 *   This is a simple test servant implementation
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "test_i.h"

// Constructor
Tie_i::Tie_i (CORBA::Long value)
  : value_ (value)
{
}

// Destructor
Tie_i::~Tie_i ()
{
}

// Return this->value
CORBA::Long
Tie_i::doit ()
{
  return this->value_;
}

// Constructor
A_i::A_i (CORBA::Long value,
          PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

// Return this->value
CORBA::Long
A_i::doit ()
{
  return this->value_;
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
A_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// Constructor
Outer_i::B_i::B_i (CORBA::Long value,
                   PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

// Return this->value
CORBA::Long
Outer_i::B_i::doit ()
{
  return this->value_;
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
Outer_i::B_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// Constructor
Outer_i::Inner_i::C_i::C_i (CORBA::Long value,
                            PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

// Return this->value
CORBA::Long
Outer_i::Inner_i::C_i::doit ()
{
  return this->value_;
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
Outer_i::Inner_i::C_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
