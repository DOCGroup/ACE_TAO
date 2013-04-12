// $Id$

#include "orbsvcs/Log_Macros.h"
#include "Activator_Loader.h"
#include "ace/Dynamic_Service.h"
#include "ace/Task.h"

class ImR_Activator_ORB_Runner : public ACE_Task_Base
{
  ImR_Activator_Loader& service_;
public:
  ImR_Activator_ORB_Runner (ImR_Activator_Loader& service)
    : service_ (service)
  {
  }
  virtual int svc ()
  {
    // Block until service_.fini() calls orb->destroy()
    this->service_.run ();
    return 0;
  }
};

ImR_Activator_Loader::ImR_Activator_Loader (void)
{
}

// For now, we will assume that it's sufficient to start
// the service in its own thread. Later, if necessary, we
// can add a command line option to allow the imr to use
// the same orb as other tao services, however the imr
// is currently written with the assumption that it's running
// in its own orb.
int
ImR_Activator_Loader::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      int err = this->opts_.init (argc, argv);
      if (err != 0)
        return -1;

      // Creates it's own internal orb, which we must run later
      err = this->service_.init (this->opts_);
      if (err != 0)
        return -1;

      // Create a thread in which to run the service
      ACE_ASSERT (this->runner_.get () == 0);
      this->runner_.reset (new ImR_Activator_ORB_Runner (*this));
      this->runner_->activate ();
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  return 0;
}

int
ImR_Activator_Loader::fini (void)
{
  ACE_ASSERT (this->runner_.get() != 0);
  try
    {
      int ret = this->service_.fini ();

      this->runner_->wait ();
      this->runner_.reset (0);
      return ret;
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
}

CORBA::Object_ptr
ImR_Activator_Loader::create_object (CORBA::ORB_ptr,
                                  int,
                                  ACE_TCHAR **)
{
  throw CORBA::NO_IMPLEMENT ();
}

int
ImR_Activator_Loader::run (void)
{
  try
    {
      return this->service_.run ();
    }
  catch (...)
    {
      ORBSVCS_ERROR ((LM_ERROR, "Exception in ImR_Locator_ORB_Runner()\n"));
      return -1;
    }
}

ACE_FACTORY_DEFINE (Activator, ImR_Activator_Loader)
