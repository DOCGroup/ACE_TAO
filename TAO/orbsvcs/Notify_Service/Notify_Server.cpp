// $Id$

#include "Notify_Service.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "orbsvcs/Notify/Notify_Default_CO_Factory.h"
#include "orbsvcs/Notify/Notify_Default_POA_Factory.h"
#include "orbsvcs/Notify/Notify_Default_Collection_Factory.h"
#include "orbsvcs/Notify/Notify_Default_EMO_Factory.h"

// Driver function for the TAO Notify Service.

int
main (int argc, char *argv[])
{
  // Init factories.
  TAO_Notify_Default_CO_Factory::init_svc ();
  TAO_Notify_Default_POA_Factory::init_svc ();
  TAO_Notify_Default_Collection_Factory::init_svc ();
  TAO_Notify_Default_EMO_Factory::init_svc ();

  TAO_Notify_Service notify_service;

  ACE_DECLARE_NEW_CORBA_ENV;

  if (notify_service.init (argc, argv TAO_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failed to start the Notification Service.\n"),
                      1);

  ACE_TRY
    {
      notify_service.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      notify_service.shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      notify_service.shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Failed to start the Notification Service\n");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}
