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
  // Init Factories
  TAO_Notify_Default_CO_Factory::init_svc ();
  TAO_Notify_Default_POA_Factory::init_svc ();
  TAO_Notify_Default_Collection_Factory::init_svc ();
  TAO_Notify_Default_EMO_Factory::init_svc ();

  TAO_Notify_Service service;

  ACE_TRY_NEW_ENV
    {
      if (service.init (argc,
                        argv,
                        ACE_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to start the Notification Service.\n"),
                          1);
      ACE_TRY_CHECK;

      if (service.run () == -1)
        {
          service.shutdown ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Failed to run the Notification Service.\n"),
                            1);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Failed to start the Notification Service\n");
      return 1;
    }
  ACE_ENDTRY;

  service.shutdown ();

  return 0;
}
