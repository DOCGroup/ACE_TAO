#include "INS_i.h"

// Constructor

INS_i::INS_i ()
{
}

// Destructor
INS_i::~INS_i ()
{
}

// Set the ORB pointer.
void
INS_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

char *
INS_i::test_ins ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Inside Operation\n"));
  return CORBA::string_dup ("Success");
}
