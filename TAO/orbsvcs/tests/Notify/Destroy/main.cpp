//$Id$

#include "../lib/Notify_Test_Client.h"
#include "orbsvcs/orbsvcs/NotifyExtC.h"

int
ACE_TMAIN (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize the base class.
      Notify_Test_Client client;
      client.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      
      CosNotifyChannelAdmin::EventChannelFactory_ptr ecf = client.notify_factory ();
      
      NotifyExt::EventChannelFactory_var ecf_ext = NotifyExt::EventChannelFactory::_narrow (ecf);
      
      ecf_ext->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_LIB_TEXT("Failed to run the program\n"));
      return 1;
    }
  ACE_ENDTRY;
  
  return 0;
}
