// $Id$

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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int err = this->opts_.init (argc, argv);
      if (err != 0)
        return -1;

      err = this->service_.init (this->opts_ ACE_ENV_ARG_PARAMETER);
      if (err != 0)
        return -1;

      // Create a thread in which to run the service
      ACE_ASSERT(this->runner_.get () == 0);
      this->runner_.reset(new ImR_Locator_ORB_Runner (*this));
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
ImR_Locator_Loader::fini (void)
{
  ACE_ASSERT(this->runner_.get () != 0);
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int ret = this->service_.fini ();

      this->runner_->wait ();
      this->runner_.reset (0);
      return ret;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Object_ptr
ImR_Locator_Loader::create_object (CORBA::ORB_ptr,
                                  int,
                                  ACE_TCHAR**
                                  ACE_ENV_ARG_DECL)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT(), CORBA::Object::_nil ());
}

int
ImR_Locator_Loader::run(void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      return this->service_.run ();
    }
  ACE_CATCHALL
    {
      ACE_ERROR((LM_ERROR, "Exception in ImR_Locator_ORB_Runner ()\n"));
      return -1;
    }
  ACE_ENDTRY;
}


ACE_FACTORY_DEFINE (Locator, ImR_Locator_Loader)
