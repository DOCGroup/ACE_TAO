// $Id$

// This is a simple example of using the ACE_IOStream and
// ACE_Streambuf_T templates to create an object based on ACE_*_Stream
// classes, which mimic a C++ iostream.

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Service_Config.h"
#include "ace/Signal.h"
#include "ace/Sig_Adapter.h"

#include "iostream_server.h"
#include "ace/OS_NS_unistd.h"

#if !defined (ACE_LACKS_ACE_IOSTREAM)

int
Handler::open (void *)
{
  if (this->reactor ()->register_handler (this,
                                          ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "registering connection handler with ACE_Reactor\n"),
                      -1);
  return 0;
}

Handler::Handler (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P) starting handler %x\n", this));
}

Handler::~Handler (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P) shutting down handler %x\n", this));
  ACE_Reactor::end_event_loop ();
}

int
Handler::handle_input (ACE_HANDLE)
{
  int i;
  float f;

  // Check to see if the socket is closed down.
  if (this->peer ().eof ())
    ACE_ERROR_RETURN ((LM_ERROR, "(%P) connection closed\n"), -1);

#if defined (ACE_HAS_STRING_CLASS)
  ACE_IOStream_String s;

  if (!(this->peer () >> i >> f >> s))
    ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "error getting data"), -1);

  cerr << "(" << ACE_OS::getpid () << ") Client sent:\n\t";
  cerr << "(" << i << ") (" << f << ") (" << s << ")" << endl ;

  if (!(this->peer () << "Received: " << i << " " << f << " " << s << endl))
    ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "error sending data"), -1);

#else
  if (!(this->peer () >> i >> f))
    ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "error getting data"), -1);

  cerr << "(" << ACE_OS::getpid () << ") Client sent:\n\t";
  cerr << "(" << i << ") (" << f << ")" << endl;

  if (!(this->peer () << i << " " << f << endl))
    ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "error sending data"), -1);
#endif /* ACE_HAS_STRING_CLASS */

  // In order to flush the output to the peer, we have to use the sync
  // () function.  Some iostreams implementations let us use a 'flush'
  // function much like the 'endl' function.

  // this->peer ().sync ();
  return 0;
}

// Create a factory object that will accept new connection requests
// and create handler objects for us.

typedef ACE_Acceptor<Handler, ACE_SOCK_ACCEPTOR> IOStream_Acceptor;
#endif /* !ACE_LACKS_ACE_IOSTREAM */

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
#if !defined (ACE_LACKS_ACE_IOSTREAM)
  ACE_Service_Config daemon;

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT so we can shut
  // down gracefully via signals.
  if (ACE_Reactor::instance ()->register_handler (sig_set,
                                                  &sa) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n"), -1);

  IOStream_Acceptor peer_acceptor;

  ACE_INET_Addr addr (argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT);

  if (peer_acceptor.open (addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P) starting up daemon\n"));

  ACE_Reactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P) shutting down server daemon\n"));

#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, "ACE_IOSTREAM not supported on this platform\n"));
#endif /* !ACE_LACKS_ACE_IOSTREAM */
  return 0;
}

