// $Id$
// server_loggerd.cpp,v 4.29 2003/12/30 23:18:59 shuston Exp

// This server daemon collects, formats, and displays logging
// information forwarded from client daemons running on other hosts in
// the network.  In addition, this example illustrates how to use the
// ACE_Reactor, ACE_Acceptor, ACE_Singleton, and the ACE_Test_and_Set
// components.

#include "ace/OS_NS_string.h"
#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/Null_Mutex.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Singleton.h"
#include "ace/Test_and_Set.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Log_Record.h"
#include "ace/Test_and_Set.h"

#include "server_loggerd.h"

ACE_RCSID(Acceptor_server, server_loggerd, "$Id$")

// ----------------------------------------

// Return the port number.

u_short
Options::port (void)
{
  return this->port_;
}

// Parse the command-line options.

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  this->port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'p':
        this->port_ = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        break;
      }
}

// ----------------------------------------

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
REACTOR;

// Our Options Singleton.
typedef ACE_Singleton<Options, ACE_Null_Mutex>
OPTIONS;

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
QUIT_HANDLER;

// ----------------------------------------

// Specialize a Logging Acceptor.
typedef ACE_Acceptor <Logging_Handler, ACE_SOCK_ACCEPTOR>
Logging_Acceptor;

// Default constructor.

Logging_Handler::Logging_Handler (void)
{
}

int
Logging_Handler::handle_timeout (const ACE_Time_Value &,
                                 const void *arg)
{
#if defined (ACE_NDEBUG)
  ACE_UNUSED_ARG (arg);
#endif /* ACE_NDEBUG */

  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) handling timeout from this = %@\n"), this));
  return 0;
}

// Perform the logging record receive.

int
Logging_Handler::handle_input (ACE_HANDLE)
{
  // Perform two recv's to emulate record-oriented semantics.  Note
  // that this code is not entirely portable since it relies on the
  // fact that sizeof (ssize_t) is the same on both the sender and
  // receiver side.  To correctly handle this is painful, and we leave
  // it as an exercise for the reader ;-).

  ssize_t len;
  ssize_t n = this->peer ().recv ((void *) &len, sizeof len);

  switch (n)
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t) %p at host %C\n"),
                         ACE_TEXT ("client logger"), this->peer_name_), -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) closing log daemon at host %C (fd = %d)\n"),
                         this->peer_name_, this->get_handle ()), -1);
      /* NOTREACHED */
    case sizeof (size_t):
      {
        ACE_Log_Record lp;

        len = ntohl (len);
        n = this->peer ().recv_n ((void *) &lp, len);

        if (n != len)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t) %p at host %C\n"),
                             ACE_TEXT ("client logger"), this->peer_name_),-1);
        /* NOTREACHED */

        lp.decode ();

        if (lp.length () == n)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ")));
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
            lp.print (ACE_TEXT_CHAR_TO_TCHAR (this->peer_name_), 1, cerr);
#else
            lp.print (ACE_TEXT_CHAR_TO_TCHAR (this->peer_name_), 1, stderr);
#endif
          }
        else
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) error, lp.length = %d, n = %d\n"),
                      lp.length (), n));
        break;
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t) %p at host %C\n"),
                         ACE_TEXT ("client logger"), this->peer_name_), -1);
      /* NOTREACHED */
    }

  return 0;
}

int
Logging_Handler::open (void *)
{
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;
  else
    {
      ACE_OS::strncpy (this->peer_name_,
                       addr.get_host_name (),
                       MAXHOSTNAMELEN + 1);

      if (REACTOR::instance ()->register_handler (this, READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) can't register with reactor\n")),
                          -1);
      else if (REACTOR::instance ()->schedule_timer
               (this,
                (const void *) this,
                ACE_Time_Value (2),
                ACE_Time_Value (2)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) can't register with reactor\n")),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) connected with %C\n"),
                    this->peer_name_));
      return 0;
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Acceptor factory.
  Logging_Acceptor peer_acceptor;

  OPTIONS::instance ()->parse_args (argc, argv);

  // We need to pass in REACTOR::instance () here so that we don't use
  // the default ACE_Reactor::instance ().

  if (peer_acceptor.open
      (ACE_INET_Addr (OPTIONS::instance ()->port ()),
       REACTOR::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);

  // Register QUIT_HANDLER to receive SIGINT commands.  When received,
  // QUIT_HANDLER becomes "set" and thus, the event loop below will
  // exit.
  else if (REACTOR::instance ()->register_handler
           (SIGINT, QUIT_HANDLER::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("registering service with ACE_Reactor\n")),
                      -1);

  // Run forever, performing logging service.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting up server logging daemon\n")));

  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance ()->is_set () == 0)
    REACTOR::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) shutting down server logging daemon\n")));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> *
  ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>::singleton_;
template ACE_Singleton<Options, ACE_Null_Mutex> *
  ACE_Singleton<Options, ACE_Null_Mutex>::singleton_;
template ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex> *
  ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
