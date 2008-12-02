//
// $Id$
//
#include "GoodDay.h"

GoodDay::GoodDay (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
GoodDay::get_string (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Hello there!");
}

void
GoodDay::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}


::Test::GoodDay::RoughWeek *
GoodDay::this_week (void)
  ACE_THROW_SPEC ((::CORBA::SystemException))
{
  // Not actually used.
  return 0;
}
