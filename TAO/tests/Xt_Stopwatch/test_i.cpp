#include "test_i.h"

Stopwatch_imp::Stopwatch_imp (CORBA::ORB_ptr orb, Timer_imp *timer)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    timer_ (timer)
{
}

void
Stopwatch_imp::start ()
{
  this->timer_->start ();
}

void
Stopwatch_imp::stop ()
{
  this->timer_->stop ();
}

void
Stopwatch_imp::shutdown ()
{
  this->orb_->shutdown (false);
}

