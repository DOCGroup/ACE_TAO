#include "worker.h"

ACE_RCSID(RT4332, worker, "$Id$")

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  ACE_DEBUG((LM_INFO,"(%P|%t) Running ORB in a separate thread\n"));
  try
    {
        this->orb_->run ();
//      do
//      {
//        ACE_DEBUG((LM_INFO,"(%P|%t) There is work pending\n"));
//        ACE_Time_Value tv(20);
//        this->orb_->run (tv);
//        ACE_TRY_CHECK;
//        ACE_DEBUG((LM_INFO,"(%P|%t) ORB is tired ...\n"));
//      }
//      while (this->orb_->work_pending ());
    }
  catch (const CORBA::Exception& ex)
    {
    }
  return 0;
}

