#include "Proxy_Handler_Connector.h"
// $Id$


Proxy_Handler_Connector::Proxy_Handler_Connector (void)
{
}

// Override the connection-failure method to add timer support.
// Note that these timers perform "expoential backoff" to 
// avoid rapidly trying to reestablish connections when a link
// goes down.

int
Proxy_Handler_Connector::handle_close (ACE_HANDLE sd, ACE_Reactor_Mask)
{
  ACE_Connector<Proxy_Handler, ACE_SOCK_CONNECTOR>::AST *stp = 0;

  // Locate the ACE_Svc_Handler corresponding to the socket descriptor.
  if (this->handler_map_.find (sd, stp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) can't locate channel %d in map, %p\n",
		      sd, "find"), -1);

  Proxy_Handler *channel = stp->svc_handler ();

  // Schedule a reconnection request at some point in the future
  // (note that channel uses an exponential backoff scheme).
  if (ACE_Service_Config::reactor ()->schedule_timer (channel, 0, 
					       channel->timeout ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      "schedule_timer"), -1);
  return 0;
}

// Initiate (or reinitiate) a connection to the Proxy_Handler.

int
Proxy_Handler_Connector::initiate_connection (Proxy_Handler *channel,
					ACE_Synch_Options &synch_options)
{
  char buf[MAXHOSTNAMELEN];

  // Mark ourselves as idle so that the various iterators 
  // will ignore us until we are reconnected.
  channel->state (Proxy_Handler::IDLE);

  if (channel->remote_addr ().addr_to_string (buf, sizeof buf) == -1
      || channel->local_addr ().addr_to_string (buf, sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      "can't obtain peer's address"), -1);

  // Try to connect to the Peer.

  if (this->connect (channel, channel->remote_addr (), 
		     synch_options, channel->local_addr ()) == -1)
    {
      if (errno != EWOULDBLOCK)
	{
	  channel->state (Proxy_Handler::FAILED);
	  ACE_DEBUG ((LM_DEBUG, "(%t) %p on address %s\n", 
		     "connect", buf));

	  // Reschedule ourselves to try and connect again.
	  if (synch_options[ACE_Synch_Options::USE_REACTOR])
	    {
	      if (ACE_Service_Config::reactor ()->schedule_timer 
		  (channel, 0, channel->timeout ()) == 0)
		ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
				  "schedule_timer"), -1);
	    }
	  else 
	    // Failures on synchronous connects are reported as errors
	    // so that the caller can decide how to proceed.
	    return -1;
	}
      else
	{
	  channel->state (Proxy_Handler::CONNECTING);
	  ACE_DEBUG ((LM_DEBUG, 
		     "(%t) in the process of connecting %s to %s\n",
		     synch_options[ACE_Synch_Options::USE_REACTOR] 
		     ? "asynchronously" : "synchronously", buf));
	}
    }
  else 
    {
      channel->state (Proxy_Handler::ESTABLISHED);
      ACE_DEBUG ((LM_DEBUG, "(%t) connected to %s on %d\n", 
		 buf, channel->get_handle ()));
    }
  return 0;
}
