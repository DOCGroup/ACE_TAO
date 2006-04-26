// $Id$

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
ImR_Activator_Loader::init (int argc, ACE_TCHAR *argv[] ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      int err = this->opts_.init (argc, argv);
      if (err != 0)
        return -1;

      // Creates it's own internal orb, which we must run later
      err = this->service_.init (this->opts_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (err != 0)
        return -1;

      // Create a thread in which to run the service
      ACE_ASSERT (this->runner_.get () == 0);
      this->runner_.reset (new ImR_Activator_ORB_Runner (*this));
      this->runner_->activate ();
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

int
ImR_Activator_Loader::fini (void)
{
  ACE_ASSERT (this->runner_.get() != 0);
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int ret = this->service_.fini (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->runner_->wait ();
      this->runner_.reset (0);
      return ret;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
}

CORBA::Object_ptr
ImR_Activator_Loader::create_object (CORBA::ORB_ptr,
                                  int,
                                  ACE_TCHAR **
                                  ACE_ENV_ARG_DECL)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN(CORBA::NO_IMPLEMENT (), CORBA::Object::_nil ());
}

int
ImR_Activator_Loader::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      return this->service_.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR, "Exception in ImR_Locator_ORB_Runner()\n"));
      return -1;
    }
  ACE_ENDTRY;
}

ACE_FACTORY_DEFINE (Activator, ImR_Activator_Loader)
