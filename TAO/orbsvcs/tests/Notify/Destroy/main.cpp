//$Id$

#include "../lib/Notify_Test_Client.h"
#include "orbsvcs/NotifyExtC.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize the base class.
      Notify_Test_Client client;
      client.init (argc, argv ACE_ENV_ARG_PARAMETER);

      CosNotifyChannelAdmin::EventChannelFactory_ptr ecf = client.notify_factory ();

      NotifyExt::EventChannelFactory_var ecf_ext = NotifyExt::EventChannelFactory::_narrow (ecf);

      ACE_DEBUG((LM_DEBUG, "Destroying ECF...\n"));

      ecf_ext->destroy ();

      ACE_DEBUG((LM_DEBUG, "ECF destroyed.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, ACE_TEXT("Error: \n"));
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
