//
// $Id$
//
#include "Process.h"

ACE_RCSID(Client_Leaks, Process, "$Id$")

Process::Process (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Long
Process::get_process_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ACE_OS::getpid ();
}

void
Process::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
  ACE_CHECK;
}
