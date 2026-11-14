/* -*- C++ -*-  */

#include "MessengerShutdownTimer.h"

MessengerShutdownTimer::MessengerShutdownTimer (CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

int MessengerShutdownTimer::handle_timeout (
  ACE_Time_Value const &,
  void const *)
{
  ACE_DEBUG((LM_DEBUG, "In MessengerShutdownTimer::handle_timeout\n"));
  orb_->shutdown (false);
  return 0;
}
