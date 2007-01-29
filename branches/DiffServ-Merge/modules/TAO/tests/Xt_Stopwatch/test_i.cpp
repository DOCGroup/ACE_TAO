// $Id$

#include "test_i.h"

ACE_RCSID(Xt_Stopwatch, test_i, "$Id$")

Stopwatch_imp::Stopwatch_imp (CORBA::ORB_ptr orb, Timer_imp *timer)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    timer_ (timer)
{
}

void
Stopwatch_imp::start (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->timer_->start ();
}

void
Stopwatch_imp::stop (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->timer_->stop ();
}

void
Stopwatch_imp::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}

