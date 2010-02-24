//
// $Id$
//
#include "Server_Task.h"

ACE_RCSID(AMI_Latency, Server_Task, "$Id$")

Server_Task::Server_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&){}
  return 0;
}
