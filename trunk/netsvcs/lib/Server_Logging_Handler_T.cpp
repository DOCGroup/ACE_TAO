// $Id$

#ifndef ACE_SERVER_LOGGING_HANDLERT_C
#define ACE_SERVER_LOGGING_HANDLERT_C

#define ACE_BUILD_SVC_DLL

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Get_Opt.h"
#include "Server_Logging_Handler_T.h"

ACE_RCSID(lib, Server_Logging_Handler_T, "$Id$")

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Track number of requests.
template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_DECL, class LMR>
COUNTER ACE_Server_Logging_Handler_T<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_USE, LMR>::request_count_ = (COUNTER) 0;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_DECL, class LMR>
ACE_Server_Logging_Handler_T<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_USE, LMR>::ACE_Server_Logging_Handler_T
  (ACE_Thread_Manager *,
   LMR const &receiver)
   // Initialize the CString to something that is not the empty string
   // to avoid problems when calling fast_rep()
#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES) && !defined (__GNUG__)
  : receiver_ (receiver, ACE_CString (" ", 1))
#else
  : receiver_ (receiver),
    host_name_ (ACE_CString (" ", 1))
#endif /* ! ACE_HAS_BROKEN_HPUX_TEMPLATES && ! __GNUG__ */
{
}

// Callback routine for handling the reception of remote logging
// transmissions.

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_DECL, class LMR> int
ACE_Server_Logging_Handler_T<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_USE, LMR>::handle_input (ACE_HANDLE)
{
  int result = this->handle_logging_record ();
  return result > 0 ? 0 : -1;
}

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_DECL, class LMR> const char *
ACE_Server_Logging_Handler_T<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_USE, LMR>::host_name (void)
{
#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES) && !defined (__GNUG__)
  return this->receiver_.m_.fast_rep ();
#else
  return this->host_name_.fast_rep ();
#endif /* ! ACE_HAS_BROKEN_HPUX_TEMPLATES && ! __GNUG__ */
}

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_DECL, class LMR> int
ACE_Server_Logging_Handler_T<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_USE, LMR>::handle_logging_record (void)
{
  ACE_INT32 length;

  // We need to use the ol' two-read trick here since TCP sockets
  // don't support framing natively.  Note that the first call is just
  // a "peek" -- we don't actually remove the data until the second
  // call.  Note that this code is portable as long as ACE_UNIT32 is
  // always 32 bits on both the sender and receiver side.

  switch (this->peer ().recv ((void *) &length,
                              sizeof length,
                              MSG_PEEK))
    {
    default:
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p at host %s\n",
                         "server logger",
                         this->host_name ()),
                        -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "closing log daemon at host %s\n",
                         this->host_name ()),
                        -1);
      /* NOTREACHED */
    case sizeof length:
      {
        ACE_Log_Record lp;

        // Use ACE_NTOHL to get around bug in egcs 2.91.6x.
        //
        // Using the ACE_NTOHL macro is functionally equivalent to
        // the standard ntohl() system call but it may be more
        // inefficient on some platforms.
        length = ACE_NTOHL (length);

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
        u_long count = ++this->request_count_;
        ACE_DEBUG ((LM_DEBUG,
                    "request count = %d, length = %d\n",
                    count,
                    length));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

        // Perform the actual <recv> this time.
        ssize_t n = this->peer ().recv_n ((void *) &lp,
                                          length);
        if (n != length)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%d != %d, %p at host %s\n",
                             n,
                             length,
                             "server logger",
                             this->host_name ()),
                            -1);

        lp.decode ();

        if (lp.length () == n)
          {
            // Send the log record to the log message receiver for
            // processing.
            ostream *orig_ostream = ACE_Log_Msg::instance ()->msg_ostream ();
            receiver ().log_record (this->host_name (),
                                    lp);
            receiver ().log_output (this->host_name (),
                                    lp,
                                    orig_ostream);
          }
        else
          ACE_ERROR ((LM_ERROR,
                      "error, lp.length = %d, n = %d\n",
                      lp.length (),
                      n));
        return n;
      }
    }

  ACE_NOTREACHED (return -1;)
}

// Hook called by Server_Logging_Acceptor when connection is
// established.

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_DECL, class LMR> int
ACE_Server_Logging_Handler_T<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_USE, LMR>::open_common (void)
{
  // Shut off non-blocking IO if it was enabled...
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "disable"),
                      -1);
  ACE_PEER_STREAM_ADDR client_addr;

  // Determine the address of the client and display it.
  if (this->peer ().get_remote_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_remote_addr"),
                      -1);

#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES) && !defined (__GNUG__)
  this->receiver_.m_ = ACE_CString (client_addr.get_host_name (),
                                    MAXHOSTNAMELEN + 1);
#else
  this->host_name_ = ACE_CString (client_addr.get_host_name (),
                                  MAXHOSTNAMELEN + 1);
#endif /* ! ACE_HAS_BROKEN_HPUX_TEMPLATES && ! __GNUG__ */


  ACE_DEBUG ((LM_DEBUG,
              "(%t) accepted connection from host %s on fd %d\n",
              client_addr.get_host_name (),
              this->peer ().get_handle ()));

  return 0;
}

template<class SLH, class LMR, class SST>
ACE_Server_Logging_Acceptor_T<SLH, LMR, SST>::ACE_Server_Logging_Acceptor_T (void)
{
}

template<class SLH, class LMR, class SST> LMR &
ACE_Server_Logging_Acceptor_T<SLH, LMR, SST>::receiver (void)
{
  return receiver_;
}

template<class SLH, class LMR, class SST> SST &
ACE_Server_Logging_Acceptor_T<SLH, LMR, SST>::scheduling_strategy (void)
{
#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES) && !defined (__GNUG__)
  return receiver_.m_;
#else
  return schedule_strategy_;
#endif /* ! ACE_HAS_BROKEN_HPUX_TEMPLATES && ! __GNUG__ */
}

template<class SLH, class LMR, class SST> int
ACE_Server_Logging_Acceptor_T<SLH, LMR, SST>::init (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Server_Logging_Acceptor<SLH, LMR, SST>::init");

  // Use the options hook to parse the command line arguments and set
  // options.
  this->parse_args (argc, argv);

  // Set the acceptor endpoint into listen mode (use the Singleton
  // global Reactor...).
  if (this->open (this->service_addr_,
                  ACE_Reactor::instance (),
                  0, 0, 0,
                  &this->scheduling_strategy(),
                  "Logging Server",
                  "ACE logging service") == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%n: %p on port %d\n",
                       "acceptor::open failed",
                       this->service_addr_.get_port_number ()),
                      -1);
  // Ignore SIGPIPE so that each <SVC_HANDLER> can handle this on its
  // own.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);
  ACE_UNUSED_ARG (sig);

  ACE_INET_Addr server_addr;

  // Figure out what port we're really bound to.
  if (this->acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_local_addr"),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              "starting up Logging Server at port %d on handle %d\n",
             server_addr.get_port_number (),
             this->acceptor ().get_handle ()));
  return 0;
}

template<class SLH, class LMR, class SST> int
ACE_Server_Logging_Acceptor_T<SLH, LMR, SST>::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Server_Logging_Acceptor<SLH, LMR, SST>::parse_args");

  this->service_port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_LOG_MSG->open ("Logging Service");

  ACE_Get_Opt get_opt (argc, argv, "p:", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
        {
        case 'p':
          this->service_port_ = ACE_OS::atoi (get_opt.optarg);
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "%n:\n[-p server-port]\n%a", 1),
                           -1);
        }
    }

  this->service_addr_.set (this->service_port_);
  return 0;
}

template<class SLH, class LMR, class SST> int
ACE_Server_Logging_Acceptor_T<SLH, LMR, SST>::make_svc_handler (SLH *&handler)
{
  ACE_NEW_RETURN (handler,
                  SLH (ACE_Thread_Manager::instance (),
                       this->receiver()),
                  -1);
  return 0;
}

template<class LMR>
ACE_Server_Logging_Handler<LMR>::ACE_Server_Logging_Handler (ACE_Thread_Manager * tm,
                                                             LMR const& receiver)
  : ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH, LMR>(tm,
                                                                                   receiver)
{
}

template<class LMR>
ACE_Server_Logging_Handler<LMR>::ACE_Server_Logging_Handler(ACE_Thread_Manager * tm)
  : ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH, LMR>(tm, LMR())
{
}

template<class LMR>  int
ACE_Server_Logging_Handler<LMR>::open (void *)
{
  // call base class open_common
  if (this->open_common () != 0)
    return -1;

  // Register ourselves with the Reactor to enable subsequent
  // dispatching.
  if (ACE_Reactor::instance ()->register_handler
      (this, ACE_Event_Handler::READ_MASK) == -1)
    return -1;
  return 0;
}

template<class LMR>
ACE_Thr_Server_Logging_Handler<LMR>::ACE_Thr_Server_Logging_Handler (ACE_Thread_Manager *tm, LMR const &receiver)
  : ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, ACE_LOGGER_COUNTER, ACE_LOGGER_SYNCH, LMR>(tm, receiver)
{
}

template<class LMR>
ACE_Thr_Server_Logging_Handler<LMR>::ACE_Thr_Server_Logging_Handler (ACE_Thread_Manager *tm)
  : ACE_Server_Logging_Handler_T<LOGGING_PEER_STREAM, ACE_LOGGER_COUNTER, ACE_LOGGER_SYNCH, LMR>(tm, LMR ())
{
}

template<class LMR> int
ACE_Thr_Server_Logging_Handler<LMR>::open (void *)
{
  // call base class open_common
  if (this->open_common () != 0)
    return -1;

  // Spawn a new thread of control to handle logging records with the
  // client.  Note that this implicitly uses the
  // ACE_Thread_Manager::instance () to control all the threads.
  if (this->activate (THR_BOUND | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "spawn"),
                      -1);
  return 0;
}

// Process remote logging records.

template<class LMR> int
ACE_Thr_Server_Logging_Handler<LMR>::svc (void)
{
  int result = 0;

  // Loop until the client terminates the connection or an error occurs.

  while ((result = this->handle_input ()) > 0)
    continue;

  return result;
}
#endif /* ACE_SERVER_LOGGING_HANDLER_TT_C */
