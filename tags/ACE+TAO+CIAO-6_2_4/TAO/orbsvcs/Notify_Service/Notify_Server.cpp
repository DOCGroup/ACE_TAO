// $Id$

#include "orbsvcs/Log_Macros.h"
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
    ORBSVCS_DEBUG ((LM_DEBUG,
                "shutting down on signal %d\n", which_signal));
  (void) this->svc_.fini ();
}

// Driver function for the TAO Notify Service.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_Notify_Service_Driver notify_service;

  Notify_Service_Shutdown_Functor killer (notify_service);
  Service_Shutdown kill_contractor (killer);

  try
    {
      if (notify_service.init (argc, argv) == -1)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("Failed to initialize the Notification Service.\n")),
                          1);

      notify_service.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Failed to run the Notification Service\n"));
      return 1;
    }

  return 0;
}
