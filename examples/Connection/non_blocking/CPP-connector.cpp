#if !defined (CPP_CONNECTOR_C)
// $Id$

#define CPP_CONNECTOR_C


#include "CPP-connector.h"
                                                        
#define PR_ST_1 ACE_PEER_STREAM_1
#define PR_ST_2 ACE_PEER_STREAM_2
#define PR_CO_1 ACE_PEER_CONNECTOR_1
#define PR_CO_2 ACE_PEER_CONNECTOR_2
#define PR_AD ACE_PEER_CONNECTOR_ADDR
#define SH SVC_HANDLER

template <PR_ST_1>
Peer_Handler<PR_ST_2>::Peer_Handler (ACE_Reactor *r)
  : action_ (&Peer_Handler<PR_ST_2>::idle)
{
  this->reactor (r);
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "activating %d\n", this->get_handle ()));
  this->action_ = &Peer_Handler<PR_ST_2>::connected;

  if (this->reactor ())
    this->reactor ()->register_handler (this, ACE_Event_Handler::WRITE_MASK);
  else
    {
      while (this->connected () != -1)
	continue;

      this->handle_close (0, ACE_Event_Handler::READ_MASK);
    }
  return 0;
}

template <PR_ST_1> ACE_HANDLE
Peer_Handler<PR_ST_2>::get_handle (void) const
{
  return this->peer ().get_handle ();
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::disconnecting (void)
{
  char buf[BUFSIZ];
  int n;

  if ((n = this->peer ().recv (buf, sizeof buf)) > 0)
    ACE_OS::write (ACE_STDOUT, buf, n);
  this->action_ = &Peer_Handler<PR_ST_2>::idle;
  return -1;
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::idle (void)
{
  ACE_DEBUG ((LM_DEBUG, "idle!\n"));
  return 0;
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::connected (void)
{
  char buf[BUFSIZ];
  int  n;

  ACE_DEBUG ((LM_DEBUG, "please enter input..: "));
      
  if ((n = ACE_OS::read (ACE_STDIN, buf, sizeof buf)) > 0
      && this->peer ().send_n (buf, n) != n)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "write failed"), -1);
  else if (n == 0) /* Explicitly close the connection. */
    {
      if (this->peer ().close () == -1) 
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);
      this->action_ = &Peer_Handler<PR_ST_2>::disconnecting;
      return -1;
    }
  else
    return 0;
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::stdio (void)
{
  char buf[BUFSIZ];
  int  n;

  ACE_DEBUG ((LM_DEBUG, "stdio!\n"));

  ACE_DEBUG ((LM_DEBUG, "please enter input..: "));
      
  if ((n = ACE_OS::read (ACE_STDIN, buf, sizeof buf)) > 0)
    {
      ACE_OS::write (ACE_STDOUT, buf, n);
      return 0;
    }
  else 
    return -1;
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "in handle_output\n"));

  return (this->*action_) ();
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "in handle_input\n"));

  return (this->*action_) ();
}

template <PR_ST_1> int
Peer_Handler<PR_ST_2>::handle_close (ACE_HANDLE, 
				     ACE_Reactor_Mask mask)
{
  ACE_DEBUG ((LM_DEBUG, "closing down (%d)\n", mask));
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::WRITE_MASK))
    {
      this->action_ = &Peer_Handler<PR_ST_2>::stdio;
      this->peer ().close ();
      ACE_OS::rewind (stdin);
      return this->reactor () && this->reactor ()->register_handler 
	(ACE_STDIN, this, ACE_Event_Handler::READ_MASK);
    }
  else if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK))
    delete this;
  return 0;

}

template <class SH, PR_CO_1> int
IPC_Client<SH, PR_CO_2>::svc (void)
{
  if (this->reactor ())
    ACE_Service_Config::run_reactor_event_loop ();

  return 0;
}

template <class SH, PR_CO_1> int
IPC_Client<SH, PR_CO_2>::fini (void)
{
  return 0;
}

template <class SH, PR_CO_1>
IPC_Client<SH, PR_CO_2>::IPC_Client (void)
  : done_handler_ (ACE_Sig_Handler_Ex (ACE_Service_Config::end_reactor_event_loop))
{
}

template <class SH, PR_CO_1> int
IPC_Client<SH, PR_CO_2>::init (int argc, char *argv[])
{
  // Call down to the CONNECTOR's open() method to do the initialization.
  this->inherited::open (ACE_Service_Config::reactor ());

  char *r_addr = argc > 1 ? argv[1] : 
    ACE_SERVER_ADDRESS (ACE_DEFAULT_SERVER_HOST, ACE_DEFAULT_SERVER_PORT_STR);
  ACE_Time_Value timeout (argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_TIMEOUT);
  char *l_addr = argc > 3 ? argv[3] : ACE_DEFAULT_LOCAL_PORT_STR;

  // Handle signals through the ACE_Reactor.
  if (ACE_Service_Config::reactor ()->register_handler
      (SIGINT, &this->done_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);

  PR_AD remote_addr (r_addr);
  PR_AD local_addr (l_addr);

  this->options_.set (ACE_Synch_Options::USE_REACTOR, timeout);

  // Connect to the peer, reusing the local addr if necessary.
  if (this->connect (new SH (this->reactor ()), remote_addr,
		     this->options_, local_addr, 1) == -1
	   && errno != EWOULDBLOCK)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connect"), -1);
  return 0;
}

template <class SH, PR_CO_1> 
IPC_Client<SH, PR_CO_2>::~IPC_Client (void)
{
}

template <class SH, PR_CO_1> int
IPC_Client<SH, PR_CO_2>::handle_close (ACE_HANDLE h, 
				       ACE_Reactor_Mask)
{
  if (h >= 0)
    ACE_ERROR ((LM_ERROR, "%p on %d\n", "connection failed", h));
  else // We are closing down the connector.
    {
      ACE_DEBUG ((LM_DEBUG, "closing down IPC_Client\n"));
      this->inherited::handle_close ();
    }

  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

#undef PR_ST_1
#undef PR_ST_2
#undef PR_CO_1
#undef PR_CO_2
#undef PR_AD
#undef SH
#endif /* CPP_CONNECTOR_C */
