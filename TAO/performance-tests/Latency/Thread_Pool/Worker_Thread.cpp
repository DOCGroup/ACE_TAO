//
// $Id$
//
#include "Worker_Thread.h"

ACE_RCSID(Thread_Pool_Latency, Worker_Thread, "$Id$")

Worker_Thread::Worker_Thread (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker_Thread::svc (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&){}
  return 0;
}
