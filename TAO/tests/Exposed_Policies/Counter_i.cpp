#include "Counter_i.h"

Counter_Servant::Counter_Servant (Policy_Tester *policy_tester)
  : count_ (0),
    policy_tester_ (policy_tester)
{
}

// Counter Interface Methods Implementation.
void
Counter_Servant::increment ()
{
  ++this->count_;
}

CORBA::Long
Counter_Servant::get_count ()
{
  return this->count_;
}

void
Counter_Servant::reset ()
{
  this->count_ = 0;
}

void
Counter_Servant::shutdown ()
{
  this->policy_tester_->shutdown ();
}
