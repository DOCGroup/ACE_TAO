//
// $Id$
//
#include "Sleep_Service.h"

ACE_RCSID(MT_Timeout, Sleep_Service, "$Id$")

Sleep_Service::Sleep_Service (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Sleep_Service::go_to_sleep (CORBA::ULong microseconds
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong  secs = microseconds / 1000000;
  CORBA::ULong usecs = microseconds % 1000000;
  ACE_Time_Value tv (secs, usecs);

  ACE_OS::sleep (tv);
}

void
Sleep_Service::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Shutting down the ORB\n"));
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
