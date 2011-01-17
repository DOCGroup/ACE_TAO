//$Id$

#include "Counter_i.h"

// Dtor-Ctor Implementation.

Counter_Servant::Counter_Servant (Policy_Tester *policy_tester)
  : count_ (0),
    policy_tester_ (policy_tester)
{
  // No-Op.
}

Counter_Servant::~Counter_Servant (void)
{
  // No-Op.
}

// Counter Interface Methods Implementation.

void
Counter_Servant::increment (/**/)
{
  ++this->count_;
}

CORBA::Long
Counter_Servant::get_count (/**/)
{
  return this->count_;
}

void
Counter_Servant::reset (/**/)
{
  this->count_ = 0;
}

void
Counter_Servant::shutdown (void)
{
  this->policy_tester_->shutdown ();
}
