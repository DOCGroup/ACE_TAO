#include "RTEvent_Logging_Service.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "ace/OS_main.h"

#include "orbsvcs/Shutdown_Utilities.h"
#include "tao/debug.h"

ACE_RCSID (RTEvent_Logging_Service,
           RTEvent_Logging_Server,
           "$Id$")


class Logging_Svc_Shutdown
  : public Shutdown_Functor
{
public:
  Logging_Svc_Shutdown (RTEvent_Logging_Service& svc);

  void operator() (int which_signal);

private:
  RTEvent_Logging_Service&        svc_;
};

Logging_Svc_Shutdown::Logging_Svc_Shutdown (RTEvent_Logging_Service& svc)
  : svc_ (svc)
{
}

void
Logging_Svc_Shutdown::operator() (int which_signal)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "RTEvent_Logging_Service: shutting down on signal %d\n",
                which_signal));

  ACE_DECLARE_NEW_CORBA_ENV;
  (void) this->svc_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Driver function for the RTEvent_Logging_Service

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  RTEvent_Logging_Service service;

  Logging_Svc_Shutdown killer (service);
  Service_Shutdown kill_contractor (killer);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int rc;

      rc = service.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (rc == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to initialize the Telecom Log Service.\n"),
                          1);

      rc = service.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (rc == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to start the Telecom Log Service.\n"),
                          1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Failed to start the Telecom Log Service.\n");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
