#if !defined (CPP_ACCEPTOR_C)
// $Id$

#define CPP_ACCEPTOR_C

#include "ace/Service_Config.h"
#include "CPP-acceptor.h"

#define PR_ST_1 ACE_PEER_STREAM_1
#define PR_ST_2 ACE_PEER_STREAM_2
#define PR_AC_1 ACE_PEER_ACCEPTOR_1
#define PR_AC_2 ACE_PEER_ACCEPTOR_2
#define PR_AD ACE_PEER_STREAM_ADDR
#define SH SVC_HANDLER

template <PR_ST_1>
Svc_Handler<PR_ST_2>::Svc_Handler (ACE_Reactor *r)
  : SVC_HANDLER (0, 0, r)
{
}

template <PR_ST_1> int 
Svc_Handler<PR_ST_2>::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "calling Svc_Handler close\n"));

  // Free up the handle.
  this->peer ().close ();
  return 0;
}

template <PR_ST_1> int 
Svc_Handler<PR_ST_2>::open (void *)
{ 
  PR_AD client_addr;
  char buf[BUFSIZ];

  if (this->peer ().get_remote_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_remote_addr"), -1);
  else if (client_addr.addr_to_string (buf, sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "addr_to_string"), -1);
  else
    ACE_DEBUG ((LM_DEBUG, "client addr %s on handle %d\n",
		buf, this->peer ().get_handle ()));

  // Process the connection immediately since we are an interative
  // server.
  return this->handle_input ();
}

// Receive and process the data from the client.

template <PR_ST_1> int 
Svc_Handler<PR_ST_2>::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  // Read data from client (terminate on error).

  ACE_DEBUG ((LM_DEBUG, "(%t) in handle_input\n"));

  for (ssize_t r_bytes; (r_bytes = this->peer ().recv (buf, sizeof buf)) > 0; )
    if (ACE_OS::write (ACE_STDOUT, buf, r_bytes) != r_bytes)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE::send_n"), -1);

    // Send back ack.
  if (this->peer ().send_n ("", 1) != 1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send_n"), -1);
  return 0;
}

template <PR_ST_1> int 
Svc_Handler<PR_ST_2>::handle_timeout (const ACE_Time_Value &, 
				       const void *)
{
  ACE_DEBUG ((LM_DEBUG, "%p\n", "handle_timeout"));
  return 0;
}

template <class SH, PR_AC_1> int
IPC_Server<SH, PR_AC_2>::init (int argc, char *argv[])
{
  const char *local_addr = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_PORT_STR;
  ACE_Time_Value timeout (argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_TIMEOUT);
  int use_reactor = argc > 3 ? ACE_Synch_Options::USE_REACTOR : 0;

  this->options_.set (ACE_Synch_Options::USE_TIMEOUT | use_reactor, timeout);

  if (this->server_addr_.set (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "set"), -1);
  // Call down to the ACCEPTOR's open() method to do the initialization.
  if (this->inherited::open (this->server_addr_, 
			     use_reactor ? ACE_Reactor::instance () : 0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  // Handle the SIGINT signal through the ACE_Reactor.
  else if (ACE_Reactor::instance ()->register_handler
	   (SIGINT, &this->done_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
#if !defined (ACE_WIN32)
  // Handle the SIGPIPE signal through the ACE_Reactor.
  else if (ACE_Reactor::instance ()->register_handler 
	   (SIGPIPE, &this->done_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
#endif /* ACE_WIN32 */
  else
    return 0;
}

template <class SH, PR_AC_1>
IPC_Server<SH, PR_AC_2>::IPC_Server (void)
  : done_handler_ (ACE_Sig_Handler_Ex (ACE_Reactor::end_event_loop))
{
}

template <class SH, PR_AC_1> int
IPC_Server<SH, PR_AC_2>::fini (void)
{
  return 0;
}

template <class SH, PR_AC_1> 
IPC_Server<SH, PR_AC_2>::~IPC_Server (void)
{
}

// Run the interative service.

template <class SH, PR_AC_1> int
IPC_Server<SH, PR_AC_2>::svc (void)
{
  char buf[BUFSIZ];                                          

  if (this->server_addr_.addr_to_string (buf, sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "addr_to_string"), -1);
  else
    ACE_DEBUG ((LM_DEBUG, "starting server addr %s on handle %d\n",
		buf, this->get_handle ()));

  // Performs the iterative server activities.

  while (ACE_Reactor::event_loop_done() == 0)
    {
      SH sh (this->reactor ());
                                                                     
      // Create a new SH endpoint, which performs all processing in
      // its open() method (note no automatic restart if errno ==
      // EINTR).

      if (this->accept (&sh, 0, this->options_, 0) == -1)
	{ 
	  if (errno == EWOULDBLOCK && this->reactor ())
	    this->reactor ()->handle_events ();
	  else
	    ACE_ERROR ((LM_ERROR, "%p on handle %d\n", 
			"accept", this->acceptor ().get_handle ()));
	}

      // SH's destructor closes the stream implicitly but the
      // listening endpoint stays open.
    }

  /* NOTREACHED */
  return 0;
}

#undef PR_ST_1
#undef PR_ST_2
#undef PR_AC_1
#undef PR_AC_2
#undef PR_AD
#undef SH
#endif /* CPP_ACCEPTOR_C */
