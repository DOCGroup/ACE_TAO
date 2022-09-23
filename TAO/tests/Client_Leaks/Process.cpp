#include "Process.h"
#include "ace/OS_NS_unistd.h"

Process::Process (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Long
Process::get_process_id ()
{
  return ACE_OS::getpid ();
}

void
Process::shutdown ()
{
  this->orb_->shutdown (false);
}
