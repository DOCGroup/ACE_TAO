// $Id$

#include "ace/Service_Config.h"
#include "ace/Logging_Strategy.h"
#include "TS_Clerk_Handler.h"
#include "TS_Server_Handler.h"
#include "Client_Logging_Handler.h"
#include "Name_Handler.h"
#include "Token_Handler.h"
#include "Server_Logging_Handler.h"

ACE_RCSID(servers, main, "$Id$")

int
main (int argc, char *argv[])
{
  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive signals so we can shut down
  // gracefully.
  if (ACE_Reactor::instance ()->register_handler (sig_set,
                                                  &sa) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n"),
                      -1);

  // Try to link in the svc.conf entries dynamically.
  if (ACE_Service_Config::open (argc, argv) == -1)
    {
      if (errno != ENOENT)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "open"),
                          1);
      else // Use static linking.
        {
          if (ACE::debug () == 0)
            ACE_LOG_MSG->priority_mask (~LM_DEBUG,
                                        ACE_Log_Msg::PROCESS);

          // Calling ACE_SVC_INVOKE to create a new Service_Object.
          // Stash the newly created Service_Object into an
          // ACE_Service_Object_Ptr which is an <auto_ptr> specialized
          // for ACE_Service_Object.

          char *l_argv[3];
          char name_port[] = "-p " ACE_DEFAULT_NAME_SERVER_PORT_STR;

          l_argv[0] = name_port;
          l_argv[1] = 0;
          ACE_Service_Object_Ptr sp_1 = ACE_SVC_INVOKE (ACE_Name_Acceptor);

          if (sp_1->init (1, l_argv) == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "Name Service",
                        1));

          char time_port[] = "-p " ACE_DEFAULT_TIME_SERVER_PORT_STR;
          l_argv[0] = time_port;
          l_argv[1] = 0;
          ACE_Service_Object_Ptr sp_2 = ACE_SVC_INVOKE (ACE_TS_Server_Acceptor);

          if (sp_2->init (1, l_argv) == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "TS Server Acceptor", 1));

          char clerk_port[] = "-p 10011";
          l_argv[0] = argv[0];
          l_argv[1] = clerk_port;
          l_argv[2] = 0;
          ACE_Service_Object_Ptr sp_3 = ACE_SVC_INVOKE (ACE_TS_Clerk_Processor);

          if (sp_3->init (2, l_argv) == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "TS Clerk Processor",
                        1));

#if defined (ACE_HAS_TOKENS_LIBRARY)
          char token_port[] = "-p " ACE_DEFAULT_TOKEN_SERVER_PORT_STR;
          l_argv[0] = token_port;
          l_argv[1] = 0;
          ACE_Service_Object_Ptr sp_4 = ACE_SVC_INVOKE (ACE_Token_Acceptor);

          if (sp_4->init (1, l_argv) == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "Token Service",
                        1));
#endif /* ACE_HAS_TOKENS_LIBRARY */

          char thr_logging_port[] = "-p " ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR;
          l_argv[0] = thr_logging_port;
          l_argv[1] = 0;
          ACE_Service_Object_Ptr sp_5 =
            ACE_SVC_INVOKE (ACE_Thr_Server_Logging_Acceptor);

          if (sp_5->init (1, l_argv) == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "Threaded Logging Server",
                        1));

          char logging_port[] = "-p " ACE_DEFAULT_LOGGING_SERVER_PORT_STR;
          l_argv[0] = logging_port;
          l_argv[1] = 0;
          ACE_Service_Object_Ptr sp_6 =
            ACE_SVC_INVOKE (ACE_Server_Logging_Acceptor);

          if (sp_6->init (1, l_argv) == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "Logging Server", 1));

          l_argv[0] = logging_port;
          l_argv[1] = 0;
          ACE_Service_Object_Ptr sp_7 =
            ACE_SVC_INVOKE (ACE_Client_Logging_Acceptor);

          if (sp_7->init (1, l_argv) == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "Logging Client",
                        1));

          // Run forever, performing the configured services until we
          // are shut down by a SIGINT/SIGQUIT signal.

          ACE_Reactor::run_event_loop ();

          // Destructors of ACE_Service_Object_Ptr's automagically
          // call fini().
        }
    }
  else // Use dynamic linking.
    {
      if (ACE::debug () == 0)
        ACE_LOG_MSG->priority_mask (~LM_DEBUG,
                                    ACE_Log_Msg::PROCESS);

      // Run forever, performing the configured services until we are
      // shut down by a SIGINT/SIGQUIT signal.

      ACE_Reactor::run_event_loop ();
    }

  return 0;
}
