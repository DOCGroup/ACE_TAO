// $Id$

#include "ace/Service_Config.h"
#include "TS_Clerk_Handler.h"
#include "TS_Server_Handler.h"
#include "Client_Logging_Handler.h"
#include "Name_Handler.h"
#include "Token_Handler.h"
#include "Server_Logging_Handler.h"
#include "Logging_Strategy.h"

class Service_Ptr
{
public:
  Service_Ptr (ACE_Service_Object *so)
    : service_object_ (so) {}

  ~Service_Ptr (void) { this->service_object_->fini (); }

  ACE_Service_Object *operator-> () { return this->service_object_; }

private:
  ACE_Service_Object *service_object_;
  // Holds the service object until we're done.
};

int 
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Service_Config::end_reactor_event_loop);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT so we can shut
  // down gracefully via signals.
  ACE_Service_Config::reactor ()->register_handler (sig_set, &sa);

  // Try to link in the svc.conf entries dynamically.
  if (daemon.open (argc, argv) == -1)
    {
      if (errno != ENOENT)
	ACE_ERROR ((LM_ERROR, "%p\n%a", "open", 1));
      else // Use static linking.
	{
	  char *l_argv[3];

	  l_argv[0] = "-p " ACE_DEFAULT_NAME_SERVER_PORT_STR;
	  l_argv[1] = 0;
	  Service_Ptr sp_1 = ACE_SVC_INVOKE (ACE_Name_Acceptor);

	  if (sp_1->init (1, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "Name_Service", 1));

	  l_argv[0] = "-p " ACE_DEFAULT_TIME_SERVER_PORT_STR;
	  l_argv[1] = 0;
	  Service_Ptr sp_2 = ACE_SVC_INVOKE (ACE_TS_Server_Acceptor);

	  if (so->init (1, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "ACE_TS_Server_Acceptor", 1));

	  l_argv[0] = argv[0];
	  l_argv[1] = "-p 10011";
	  l_argv[2] = 0;
	  Service_Ptr sp_2 = ACE_SVC_INVOKE (ACE_TS_Clerk_Processor);

	  if (sp_2->init (2, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "ACE_TS_Clerk_Processor", 1));

	  l_argv[0] = "-p " ACE_DEFAULT_TOKEN_SERVER_PORT_STR;
	  l_argv[1] = 0;
	  Service_Ptr sp_3 = ACE_SVC_INVOKE (ACE_Token_Acceptor);

	  if (sp_3->init (1, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "Token_Service", 1));

	  l_argv[0] = "-p " ACE_DEFAULT_LOGGING_SERVER_PORT_STR;
	  l_argv[1] = 0;
	  Service_Ptr sp_4 = ACE_SVC_INVOKE (ACE_Server_Logging_Acceptor);

	  if (sp_4->init (1, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "Logging_Service", 1));

	  l_argv[0] = "-p " ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR;
	  l_argv[1] = 0;
	  Service_Ptr sp_5 = ACE_SVC_INVOKE (ACE_Thr_Server_Logging_Acceptor);

	  if (sp_5->init (1, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "Thr_Logging_Service", 1));
	}

      // Run forever, performing the configured services until we are shut
      // down by a SIGINT/SIGQUIT signal.

      daemon.run_reactor_event_loop ();

      // Destructors of Service_Ptr's automagically call fini().
    }
  else // Use dynamic linking.

    // Run forever, performing the configured services until we are shut
    // down by a SIGINT/SIGQUIT signal.

    daemon.run_reactor_event_loop ();

  return 0;
}
