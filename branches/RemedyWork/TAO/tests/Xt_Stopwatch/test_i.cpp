// $Id$

#include "test_i.h"

Stopwatch_imp::Stopwatch_imp (CORBA::ORB_ptr orb, Timer_imp *timer)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    timer_ (timer)
{
}

void
Stopwatch_imp::start (void)
{
  this->timer_->start ();
}

void
Stopwatch_imp::stop (void)
{
  this->timer_->stop ();
}

void
Stopwatch_imp::shutdown (void)
{
  this->orb_->shutdown (0);
}

