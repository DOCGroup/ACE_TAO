// $Id$

#include "test_i.h"

#if defined(ACE_HAS_XT)

ACE_RCSID(FL_Cube, test_i, "$Id$")

Stopwatch_imp::Stopwatch_imp (CORBA::ORB_ptr orb, Timer_imp *timer)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    timer_ (timer)
{
}

void
Stopwatch_imp::start (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->timer_->start ();
}

void
Stopwatch_imp::stop (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->timer_->stop ();
}

void
Stopwatch_imp::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}

#endif /* ACE_HAS_XT */
