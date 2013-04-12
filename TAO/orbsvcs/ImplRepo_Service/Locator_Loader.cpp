// $Id$

#include "orbsvcs/Log_Macros.h"
#include "Locator_Loader.h"
#include "ace/Dynamic_Service.h"
#include "ace/Task.h"

class ImR_Locator_ORB_Runner : public ACE_Task_Base
{
public:
  ImR_Locator_ORB_Runner (ImR_Locator_Loader& service)
    : service_ (service)
  {
  }
  virtual int svc ()
  {
    // Block until service_.fini () calls orb->destroy ()
    this->service_.run ();
    return 0;
  }
private:
  ImR_Locator_Loader& service_;
};

ImR_Locator_Loader::ImR_Locator_Loader()
{
}

int
ImR_Locator_Loader::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      int err = this->opts_.init (argc, argv);
      if (err != 0)
        return -1;

      err = this->service_.init (this->opts_);
      if (err != 0)
        return -1;

      // Create a thread in which to run the service
      ACE_ASSERT(this->runner_.get () == 0);
      this->runner_.reset(new ImR_Locator_ORB_Runner (*this));
      this->runner_->activate ();
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  return 0;
}

int
ImR_Locator_Loader::fini (void)
{
  ACE_ASSERT(this->runner_.get () != 0);
  try
    {
      int ret = this->service_.fini ();

      this->runner_->wait ();
      this->runner_.reset (0);
      return ret;
    }
  catch (const CORBA::Exception&)
    {
    }
  return -1;
}

CORBA::Object_ptr
ImR_Locator_Loader::create_object (CORBA::ORB_ptr,
                                  int,
                                  ACE_TCHAR**)
{
  throw CORBA::NO_IMPLEMENT();
}

int
ImR_Locator_Loader::run(void)
{
  try
    {
      return this->service_.run ();
    }
  catch (...)
    {
      ORBSVCS_ERROR((LM_ERROR, "Exception in ImR_Locator_ORB_Runner ()\n"));
      return -1;
    }
}


ACE_FACTORY_DEFINE (Locator, ImR_Locator_Loader)
