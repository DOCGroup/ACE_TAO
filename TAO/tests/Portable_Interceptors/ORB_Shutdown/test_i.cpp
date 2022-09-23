#include "test_i.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_i::~test_i ()
{
}

void
test_i::hello ()
{
  ACE_DEBUG ((LM_DEBUG, "Hello!\n"));
}

void
test_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Shutting down ORB.\n"));

  this->orb_->shutdown (false);
}
