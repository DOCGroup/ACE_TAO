// $Id$

#if !defined (CPP_ACCEPTOR_C)

#define CPP_ACCEPTOR_C

#include "ace/Service_Config.h"
#include "CPP-acceptor.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Signal.h"



#define PR_ST_1 ACE_PEER_STREAM_1
#define PR_ST_2 ACE_PEER_STREAM_2
#define PR_AC_1 ACE_PEER_ACCEPTOR_1
#define PR_AC_2 ACE_PEER_ACCEPTOR_2
#define PR_AD ACE_PEER_STREAM_ADDR
#define SVH SVC_HANDLER

template <PR_ST_1>
Svc_Handler<PR_ST_2>::Svc_Handler (ACE_Reactor *r)
  : SVC_HANDLER (0, 0, r)
{
}

template <PR_ST_1> int
Svc_Handler<PR_ST_2>::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("calling Svc_Handler close\n")));

  // Free up the handle.
  this->peer ().close ();
  return 0;
}

template <PR_ST_1> int
Svc_Handler<PR_ST_2>::open (void *)
{
  PR_AD client_addr;
  ACE_TCHAR buf[BUFSIZ];

  if (this->peer ().get_remote_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("get_remote_addr")),
                      -1);
  else if (client_addr.addr_to_string (buf,
                                       sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("addr_to_string")),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("client addr %s on handle %d\n"),
                buf,
                this->peer ().get_handle ()));

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

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) in handle_input\n")));

  for (ssize_t r_bytes;
       (r_bytes = this->peer ().recv (buf,
                                      sizeof buf)) > 0;
       )
    if (ACE_OS::write (ACE_STDOUT,
                       buf,
                       r_bytes) != r_bytes)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("ACE::send_n")),
                        -1);
  // Send back ack.
  if (this->peer ().send_n ("",
                            1) != 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("send_n")),
                      -1);
  return 0;
}

template <PR_ST_1> int
Svc_Handler<PR_ST_2>::handle_timeout (const ACE_Time_Value &,
                                      const void *)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%p\n"), ACE_TEXT ("handle_timeout")));
  return 0;
}

template <class SVH, PR_AC_1> int
IPC_Server<SVH, PR_AC_2>::init (int argc, ACE_TCHAR *argv[])
{
  const ACE_TCHAR *local_addr = argc > 1
    ? argv[1]
    : ACE_DEFAULT_SERVER_PORT_STR;
  ACE_Time_Value timeout (argc > 2
                          ? ACE_OS::atoi (argv[2])
                          : ACE_DEFAULT_TIMEOUT);
  int use_reactor = argc > 3
    ? ACE_Synch_Options::USE_REACTOR
    : 0;

  this->options_.set (ACE_Synch_Options::USE_TIMEOUT | use_reactor,
                      timeout);

  if (this->server_addr_.set (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("set")),
                      -1);
  // Call down to the ACCEPTOR's <open> method to do the
  // initialization.
  if (this->inherited::open (this->server_addr_,
                             use_reactor
                             ? ACE_Reactor::instance ()
                             : 0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      -1);
  // Handle the SIGINT signal through the <ACE_Reactor>.
  else if (ACE_Reactor::instance ()->register_handler
           (SIGINT,
            &this->done_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("register_handler")),
                      -1);
#if !defined (ACE_WIN32)
  // Handle the SIGPIPE signal through the <ACE_Reactor>.
  else if (ACE_Reactor::instance ()->register_handler
           (SIGPIPE,
            &this->done_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("register_handler")),
                      -1);
#endif /* ACE_WIN32 */
  else
    return 0;
}

template <class SVH, PR_AC_1>
IPC_Server<SVH, PR_AC_2>::IPC_Server (void)
  : done_handler_ (ACE_Sig_Handler_Ex (ACE_Reactor::end_event_loop))
{
}

template <class SVH, PR_AC_1> int
IPC_Server<SVH, PR_AC_2>::fini (void)
{
  return 0;
}

template <class SVH, PR_AC_1>
IPC_Server<SVH, PR_AC_2>::~IPC_Server (void)
{
}

template <class SVH, PR_AC_1> int
IPC_Server<SVH, PR_AC_2>::handle_close (ACE_HANDLE handle,
                                       ACE_Reactor_Mask mask)
{
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (mask);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("calling IPC_Server handle_close, but accept handle stays open!\n")));
  return 0;
}

// Run the interative service.

template <class SVH, PR_AC_1> int
IPC_Server<SVH, PR_AC_2>::svc (void)
{
  ACE_TCHAR buf[BUFSIZ];

  if (this->server_addr_.addr_to_string (buf,
                                         sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("addr_to_string")),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("starting server addr %s on handle %d\n"),
                buf,
                this->get_handle ()));

  // Performs the iterative server activities.

  while (ACE_Reactor::event_loop_done () == 0)
    {
      SVH sh (this->reactor ());

      // Create a new <SVH> endpoint, which performs all processing in
      // its <open> method (note no automatic restart if errno ==
      // EINTR).

      if (this->accept (&sh,
                        0,
                        this->options_,
                        0) == -1)
        {
          if (errno == EWOULDBLOCK
              && this->reactor ())
            // Handle the accept asynchronously if necessary.
            this->reactor ()->handle_events ();
          else
            // We've probably timed out...
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p on handle %d\n"),
                        ACE_TEXT ("accept"),
                        this->acceptor ().get_handle ()));
        }

      // <SVH>'s destructor closes the stream implicitly but the
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
#undef SVH
#endif /* CPP_ACCEPTOR_C */
