//$Id$

#include "Counter_i.h"

ACE_RCSID(tao, Counter_Servant, "$Id$")

// Dtor-Ctor Implementation

Counter_Servant::Counter_Servant (CORBA::ORB_ptr o)
  : count_ (0),
    orb_ (CORBA::ORB::_duplicate (o))
{
}

Counter_Servant::~Counter_Servant (void)
{
}

// Counter Interface Methods Implementation

// @@ Angelo, you are going to get warnings about unused environment
// parameter in the methods below.

void
Counter_Servant::increment (CORBA::Environment &ACE_TRY_ENV)
{
  ++this->count_;
}

CORBA::Long
Counter_Servant::get_count (CORBA::Environment &ACE_TRY_ENV)
{
  return this->count_;
}

// @@ Angelo, it is important that thowspecs in .h file match
// throwspecs in .cpp file, which is not the case for the method
// below.  You will get warnings here.
void
Counter_Servant::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_->shutdown ();
}

// @@ Angelo, please *use* environment parameter to catch exceptions,
// i.e., ORB::shutdown takes environment argument - use it!
