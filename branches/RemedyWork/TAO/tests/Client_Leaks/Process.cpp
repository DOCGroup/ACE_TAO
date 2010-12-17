//
// $Id$
//
#include "Process.h"
#include "ace/OS_NS_unistd.h"

Process::Process (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Long
Process::get_process_id (void)
{
  return ACE_OS::getpid ();
}

void
Process::shutdown (void)
{
  this->orb_->shutdown (0);
}
