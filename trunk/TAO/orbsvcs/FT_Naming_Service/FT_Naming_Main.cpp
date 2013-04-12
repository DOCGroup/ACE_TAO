// $Id$

#include "orbsvcs/Log_Macros.h"
#include "FT_Naming_Service.h"
#include "ace/OS_main.h"

#include "orbsvcs/Shutdown_Utilities.h"
#include "tao/debug.h"

#include "tao/ImR_Client/ImR_Client.h"

class Naming_Svc_Shutdown : public Shutdown_Functor
{
public:
  Naming_Svc_Shutdown (TAO_FT_Naming_Service& ns);

  void operator() (int which_signal);
private:
  TAO_FT_Naming_Service& ns_;
};

Naming_Svc_Shutdown::Naming_Svc_Shutdown (TAO_FT_Naming_Service& ns)
  : ns_(ns)
{
}

void
Naming_Svc_Shutdown::operator() (int which_signal)
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                ACE_TEXT ("FT Name Service: shutting down on signal %d\n"),
                which_signal));
  (void) this->ns_.shutdown ();
}

// Driver function for the TAO FT Naming Service.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_FT_Naming_Service naming_service;

  // Stuff to insure that we're gracefully shut down...
  Naming_Svc_Shutdown killer (naming_service);
  Service_Shutdown kill_contractor(killer);

  if (naming_service.init (argc, argv) == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("Failed to start the Naming Service.\n")),
                      1);
  try
    {
      naming_service.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("NamingService"));
      return 1;
    }

  naming_service.fini ();

  return 0;
}
