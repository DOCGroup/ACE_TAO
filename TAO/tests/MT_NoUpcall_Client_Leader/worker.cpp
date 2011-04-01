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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
        this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
//      do
//      {
//        ACE_DEBUG((LM_INFO,"(%P|%t) There is work pending\n"));
//        ACE_Time_Value tv(20);
//        this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
//        ACE_TRY_CHECK;
//        ACE_DEBUG((LM_INFO,"(%P|%t) ORB is tired ...\n"));
//      }
//      while (this->orb_->work_pending ());
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return 0;
}

