// $Id$

#include "test_i.h"

#if defined(ACE_HAS_XT)

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(FL_Cube, test_i, "$Id$")

Stopwatch_imp::Stopwatch_imp (CORBA::ORB_ptr orb, Timer_imp *timer)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    timer_ (timer)
{
}

void
Stopwatch_imp::start (CORBA::Environment&)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->timer_->start ();
}

void
Stopwatch_imp::stop (CORBA::Environment&)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->timer_->stop ();
}

void
Stopwatch_imp::shutdown (CORBA::Environment&)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}

#endif /* ACE_HAS_XT */
