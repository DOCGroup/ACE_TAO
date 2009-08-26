// $Id$

#include "AMI_perform_work.h"
#include "ace/OS_NS_unistd.h"

AMI_perform_work::AMI_perform_work (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int AMI_perform_work::svc ()
{
  try
    {
      printf ("AMI :\tPerform work started\n");
      while (1)
        {
          if (orb_->work_pending())
            {
              orb_->perform_work();
            }
          ACE_Time_Value tv (0, 1000);
          ACE_OS::sleep (tv);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  printf ("AMI :\t Exiting perform work");
  return 0;
}
