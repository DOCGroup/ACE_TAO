//
// $Id$
//

#include "worker.h"

Worker::Worker (CORBA::ORB_ptr orb, int time)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     orb_timeout_(time)
{
}

int
Worker::svc (void)
{

  try
    {
      ACE_Time_Value tv (orb_timeout_);

      // orb times out after <timeout> seconds
      this->orb_->run (tv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }
  return 0;
}
