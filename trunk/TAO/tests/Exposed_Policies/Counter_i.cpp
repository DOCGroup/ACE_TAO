//$Id$

#include "Counter_i.h"

// @@ Angelo, please add rcsid string here.
// @@ Marina DONE.

ACE_RCSID(tao, Counter_Servant, "$Id$")

// Dtor-Ctor Implementation

Counter_Servant::Counter_Servant (CORBA::ORB_ptr o)
  : count_ (0),
    orb_ (CORBA::ORB::_duplicate (o))
  // @@ Angelo, remove extra spaces in paranthesis.
  // @@ Marina Done.
{
}

Counter_Servant::~Counter_Servant (void)
{
}


// Counter Interface Methods Implementation
// @@ Angelo, please fix the class name.
// @@ Marina DONE.
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

void
Counter_Servant::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_->shutdown ();
}


