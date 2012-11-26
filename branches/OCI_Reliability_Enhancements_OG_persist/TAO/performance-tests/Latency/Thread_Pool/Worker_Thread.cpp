//
// $Id$
//
#include "Worker_Thread.h"

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
