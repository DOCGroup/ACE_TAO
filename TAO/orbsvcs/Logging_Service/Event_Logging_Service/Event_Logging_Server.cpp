#include "orbsvcs/Log_Macros.h"
#include "Event_Logging_Service.h"
#include "ace/OS_main.h"

#include "orbsvcs/Shutdown_Utilities.h"
#include "tao/debug.h"

class Logging_Svc_Shutdown
  : public Shutdown_Functor
{
public:
  explicit Logging_Svc_Shutdown (Event_Logging_Service & svc);
  Logging_Svc_Shutdown (Logging_Svc_Shutdown const &) = delete;
  Logging_Svc_Shutdown (Logging_Svc_Shutdown &&) = delete;
  Logging_Svc_Shutdown &operator= (Logging_Svc_Shutdown const &) = delete;
  Logging_Svc_Shutdown &operator= (Logging_Svc_Shutdown &&) = delete;

  void operator() (int which_signal) override;

private:
  Event_Logging_Service & svc_;
};

Logging_Svc_Shutdown::Logging_Svc_Shutdown (Event_Logging_Service & svc)
  : svc_ (svc)
{
}

void
Logging_Svc_Shutdown::operator() (int which_signal)
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Event_Logging_Service: shutting down on signal %d\n"),
                which_signal));

  this->svc_.shutdown ();
}

// Driver function for the Event_Logging_Service

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  Event_Logging_Service service {};

  Logging_Svc_Shutdown killer (service);
  Service_Shutdown kill_contractor (killer);

  try
    {
      int const init_result = service.init (argc, argv);
      if (init_result == -1)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to initialize the Telecom Log Service.\n")),
                          1);

      int const run_result = service.run ();
      if (run_result == -1)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to start the Telecom Log Service.\n")),
                          1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Failed to start the Telecom Log Service.\n");
      return 1;
    }

  return 0;
}
