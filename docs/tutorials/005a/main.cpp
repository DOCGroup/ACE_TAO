// $Id$
#include "ace/Reactor.h"
ACE_Reactor * g_reactor;
	
static sig_atomic_t finished = 0;
class Logging_Handler;
	
extern "C" void handler (int) { finished = 1; }
static void *thread_run(void *);
	
static const u_short PORT = ACE_DEFAULT_SERVER_PORT;
	
int main (int argc, char *argv[])
{
  g_reactor = new ACE_Reactor;
	
  Logging_Acceptor peer_acceptor;
	
  if (peer_acceptor.open (ACE_INET_Addr (PORT)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
	
  else if (g_reactor->register_handler (&peer_acceptor, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);
	
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
	
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting up server logging daemon\n"));
	
  while ( !finished )
    g_reactor->handle_events ();
	
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutting down server logging daemon\n"));
  return 0;
}
