#include "orbsvcs/Log_Macros.h"
#include "Activator_Loader.h"
#include "Activator_ORB_Runner.h"
#include "ace/Dynamic_Service.h"

ImR_Activator_Loader::~ImR_Activator_Loader ()
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
      this->runner_= std::make_unique<ImR_Activator_ORB_Runner> (*this);
      this->runner_->activate ();
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  return 0;
}

int
ImR_Activator_Loader::fini ()
{
  ACE_ASSERT (this->runner_.get() != 0);
  try
    {
      int ret = this->service_.fini ();

      this->runner_->wait ();
      this->runner_.reset (nullptr);
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
ImR_Activator_Loader::run ()
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
