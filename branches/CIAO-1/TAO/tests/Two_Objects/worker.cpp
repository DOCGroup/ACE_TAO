
//
// $Id$
//

#include "worker.h"

Worker::Worker (CORBA::ORB_ptr orb, int time)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     timeout_(time)
{
}

int
Worker::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      ACE_Time_Value tv (timeout_);

      ///orb times out after <timeout> seconds
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
    }
  ACE_ENDTRY;
  return 0;
}
