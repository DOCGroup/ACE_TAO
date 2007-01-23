// $Id$

#include "Notify_Service.h"
#include "orbsvcs/orbsvcs/Notify/Properties.h"

// Must include this file to get a static initializer
#include "tao/Valuetype/Valuetype_Adapter_Impl.h"

#include "ace/OS_main.h"

#include "orbsvcs/Shutdown_Utilities.h"
#include "tao/debug.h"


class Notify_Service_Shutdown_Functor
  : public Shutdown_Functor
{
public:
  Notify_Service_Shutdown_Functor (TAO_Notify_Service_Driver& svc);

  void operator() (int which_signal);

private:
  TAO_Notify_Service_Driver&        svc_;
};

Notify_Service_Shutdown_Functor::Notify_Service_Shutdown_Functor (TAO_Notify_Service_Driver& svc)
  : svc_ (svc)
{
}

void
Notify_Service_Shutdown_Functor::operator() (int which_signal)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "shutting down on signal %d\n", which_signal));
  (void) this->svc_.shutdown ();
}

// Driver function for the TAO Notify Service.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_Notify_Service_Driver notify_service;

  Notify_Service_Shutdown_Functor killer (notify_service);
  Service_Shutdown kill_contractor (killer);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY_NEW_ENV
    {
      // Set the properties instance before initializing the notify
      // service.  Using the static singleton can cause static
      // destruction issues between the properties instance and
      // statically allocated type code structures.
      TAO_Notify_Properties properties;
      TAO_Notify_Properties::instance (&properties);

      int result = notify_service.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

       if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("Failed to initialize the Notification Service.\n")),
                          1);

      notify_service.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT("Failed to run the Notification Service\n"));
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
