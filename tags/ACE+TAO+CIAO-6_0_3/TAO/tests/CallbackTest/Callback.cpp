//
// $Id$
//
#include "Callback.h"

Callback::Callback (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Boolean
Callback::are_you_there (CORBA::String_out answer)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Callback - we are here!\n"));

  answer = CORBA::string_dup ("Yes, I am!");

  return 1;
}

void
Callback::test_oneway (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Callback - test_oneway!\n"));
}

void
Callback::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Callback - shutdown!\n"));

  this->orb_->shutdown (0);
}
