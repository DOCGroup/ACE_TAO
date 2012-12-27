// $Id$

#include "test_i.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_i::~test_i (void)
{
}

void
test_i::hello (void)
{
  ACE_DEBUG ((LM_DEBUG, "Hello!\n"));
}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Shutting down ORB.\n"));

  this->orb_->shutdown (0);
}
