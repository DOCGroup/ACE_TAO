// $Id$

/* We try to keep the server much simpler than before and
   remove any thing related to the acceptor or the handler 
   from the main (). This lets keep the server running irrespective of
   the state of the acceptor service, in our case and any service in
   the general case. */

/* We would need the ACE_Service_Config class as it provides an
   application developers interface to the entire Service
   Configuration Framework */
#include "ace/Service_Config.h"

/* Since we are seperating the acceptor service class from the
   server, we need to declare our Acceptor_Service */
#include "Acceptor_Service.h"

extern "C" void handler (int)
{
  ACE_Service_Config::reconfig_occurred (1);
}

int
main (int argc, char *argv[])
{
  /* Perform daemon services update ... this opens the svc.conf
     file and reads the entries present in the svc.conf
     file. We will later learn the syntax of making an entry
     into a svc.conf file. But for now, remember that this is a
     file which has the entries to load or unload a service
     dynamically or statically. */
  if (ACE_Service_Config::open (argc,
                                argv,
                                ACE_DEFAULT_LOGGER_KEY,
                                0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Service_Config::open"),
                      -1);
  
  /* Install our signal handler.  As we already know, you can actually
     register signal handlers with the reactor.  You might do that
     when the signal handler is responsible for performing "real"
     work.  Our simple flag-setter doesn't justify deriving from
     ACE_Event_Handler and providing a callback function though.  */
   ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGHUP);

   /* Start running the event loop so that it
      listens for events and acts accordingly. This event loop will run
      until either the event loop is ended explicitly or an error
      occurs. */
   ACE_Reactor::run_event_loop ();

  /* NOT REACHED */

}
