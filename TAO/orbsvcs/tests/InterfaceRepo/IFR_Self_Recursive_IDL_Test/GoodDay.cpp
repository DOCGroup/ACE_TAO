#include "GoodDay.h"

GoodDay::GoodDay (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  if (TAO_Requires_IFR_Client_Initializer == -1)
  {
    ACE_ERROR((LM_ERROR, ACE_TEXT("IFR Client failed to load\n")));
  }
}

char *
GoodDay::get_string ()
{
  return CORBA::string_dup ("Hello there!");
}

void
GoodDay::shutdown ()
{
  this->orb_->shutdown (false);
}


::Test::GoodDay::RoughWeek *
GoodDay::this_week ()
{
  // Not actually used.
  return 0;
}
