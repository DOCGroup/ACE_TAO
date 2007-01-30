#include "Basic_Logging_Service.h"
#include "ace/OS_main.h"

#include "orbsvcs/Shutdown_Utilities.h"
#include "tao/debug.h"

ACE_RCSID (Basic_Logging_Service,
           Basic_Logging_Server,
           "$Id$")


class Logging_Svc_Shutdown
  : public Shutdown_Functor
{
public:
  Logging_Svc_Shutdown (Basic_Logging_Service& svc);

  void operator() (int which_signal);

private:
  Basic_Logging_Service&        svc_;
};

Logging_Svc_Shutdown::Logging_Svc_Shutdown (Basic_Logging_Service& svc)
  : svc_ (svc)
{
}

void
Logging_Svc_Shutdown::operator() (int which_signal)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Basic_Logging_Service: shutting down on signal %d\n",
                which_signal));

  (void) this->svc_.shutdown ();
}

// Driver function for the Basic_Logging_Service

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Basic_Logging_Service service;

  Logging_Svc_Shutdown killer (service);
  Service_Shutdown kill_contractor (killer);

  try
    {
      int rc;

      rc = service.init (argc, argv);
      if (rc == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to initialize the Telecom Log Service.\n"),
                          1);

      rc = service.run ();
      if (rc == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to start the Telecom Log Service.\n"),
                          1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Failed to start the Telecom Log Service.\n");
      return 1;
    }

  return 0;
}
