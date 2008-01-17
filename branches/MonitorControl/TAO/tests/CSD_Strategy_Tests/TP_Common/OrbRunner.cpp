// $Id$
#include "OrbRunner.h"
#include "OrbTask.h"
#include "TestAppExceptionC.h"


OrbRunner::OrbRunner(CORBA::ORB_ptr orb, unsigned num_orb_threads)
  : orb_(CORBA::ORB::_duplicate(orb)),
    num_orb_threads_(num_orb_threads)
{
}


OrbRunner::~OrbRunner()
{
}


void
OrbRunner::run(void)
{
  ACE_ASSERT(this->num_orb_threads_ > 0);

  // If the num_orb_threads_ is exactly one, then just use the current
  // (mainline) thread to run the ORB event loop.
  if (this->num_orb_threads_ == 1)
    {
      // Since the num_orb_threads_ is exactly one, we just use the current
      // (mainline) thread to run the ORB event loop.
      this->orb_->run();
    }
  else
    {
      // The num_orb_threads_ is greater than 1, so we will use an OrbTask
      // (active object) to run the ORB event loop in (num_orb_threads_ - 1)
      // threads.  We use the current (mainline) thread as the other thread
      // running the ORB event loop.
      OrbTask orb_task(this->orb_.in(), this->num_orb_threads_ - 1);

      // Activate the OrbTask worker threads
      if (orb_task.open(0) != 0)
        {
          ACE_ERROR((LM_ERROR,
                     "(%P|%t) Failed to open the OrbTask.\n"));
          throw TestAppException();
        }

      // This will use the current (mainline) thread to run the ORB event loop.
      this->orb_->run();

      // Now that the current thread has unblocked from running the orb,
      // make sure to wait for all of the worker threads to complete.
      orb_task.wait();
    }
}

