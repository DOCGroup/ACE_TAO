//$Id$

#include "../lib/Notify_Test_Client.h"
#include "orbsvcs/NotifyExtC.h"
#include "ace/OS_main.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the base class.
      Notify_Test_Client client;
      client.init (argc, argv);

      CosNotifyChannelAdmin::EventChannelFactory_ptr ecf = client.notify_factory ();

      NotifyExt::EventChannelFactory_var ecf_ext = NotifyExt::EventChannelFactory::_narrow (ecf);

      ACE_DEBUG((LM_DEBUG, "Destroying ECF...\n"));

      ecf_ext->destroy ();

      ACE_DEBUG((LM_DEBUG, "ECF destroyed.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT("Error:\n"));
      return 1;
    }

  return 0;
}
