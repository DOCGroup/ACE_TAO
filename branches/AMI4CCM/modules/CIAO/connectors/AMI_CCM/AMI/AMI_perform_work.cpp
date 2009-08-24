#include "AMI_perform_work.h"

AMI_perform_work::AMI_perform_work (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int AMI_perform_work::svc ()
{
  try
    {
      while (1)
        {
          CORBA::Boolean pending = orb_->work_pending();

          if (pending)
            {
              orb_->perform_work();
            }
          ACE_OS::sleep (0.01);
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
