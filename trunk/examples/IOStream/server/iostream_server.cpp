// This is a simple example of using the ACE_IOStream_T and
// ACE_Streambuf_T templates to create an object based on ACE_*_Stream
// classes which mimic a C++ iostream.

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Service_Config.h"
#include "ace/IOStream.h"

#if !defined (ACE_LACKS_ACE_IOSTREAM)

// Declare a new type which will case an ACE_SOCK_Stream to behave
// like an iostream.  The new type (ACE_SOCK_IOStream) can be used
// anywhere an ACE_SOCK_Stream is used.

typedef ACE_IOStream_T<ACE_SOCK_Stream> ACE_SOCK_IOStream ;

// Create a service handler object based on our new
// iostream/SOCK_Stream hybrid.

typedef ACE_Svc_Handler<ACE_SOCK_IOStream, ACE_INET_Addr, ACE_NULL_SYNCH>
	Service_Handler;

class Handler : public Service_Handler
  // = TITLE
  //     Extend the <Service_Handler> object to do our bidding.  All of
  //     this is fairly standard until we get to the <handle_input>
  //     where we begin using the iostream characteristics of the
  //     peer.
{
public:
  Handler (void) {}

  virtual int open (void *)
  {
    if (this->reactor () ->register_handler
	(this, ACE_Event_Handler::READ_MASK) == - 1)
      ACE_ERROR_RETURN ((LM_ERROR,
			 "registering connection handler with ACE_Reactor\n"),
			- 1);

    return 0;
  }

  virtual void destroy (void)
  {
    this->peer ().close ();
    delete this ;
  }

  virtual int close (u_long)
  {
    this->destroy ();
    return 0 ;
  }

  virtual int handle_input (ACE_HANDLE)
  {
    int i;
    float f;

#if defined (ACE_HAS_STRING_CLASS)
    String s;

    if (!(this -> peer () >> i >> f >> s))
      {
	cerr << "Error getting data" << endl ;
	return - 1 ;
      }

    cerr << "Received (" << i << ") (" << f << ") (" << s << ")" << endl ;

    if (!(this -> peer () << "Received: " << i << " " << f << " " << s << endl))
      {
	cerr << __LINE__ << "Error sending data" << endl ;
	return - 1 ;
      }
#else
    if (!(this -> peer () >> i >> f))
      {
	cerr << "Error getting data" << endl ;
	return - 1 ;
      }

    cerr << "Received (" << i << ") (" << f << ")" << endl;

    if (!(this -> peer () << i << " " << f << endl))
      {
	cerr << __LINE__ << "Error sending data" << endl ;
	return - 1 ;
      }
#endif /* ACE_HAS_STRING_CLASS */

    // In order to flush the output to the peer, we have to use the
    // sync () function.  Some iostreams implementations let us use a
    // 'flush' function much like the 'endl' function.

    // this->peer ().sync ();
    return 0;
  }
};

// Create an object which will accept new connection requests and
// create handler objects for us.

typedef ACE_Acceptor<Handler, ACE_SOCK_ACCEPTOR> Logging_Acceptor;
#endif /* !ACE_LACKS_ACE_IOSTREAM */

int
main (int argc, char *argv [])
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
  if (ACE_Reactor::instance ()->register_handler (sig_set, &sa) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n"), -1);

  Logging_Acceptor peer_acceptor ;

  if (peer_acceptor.open (ACE_INET_Addr (argc > 1 ? atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT)) == - 1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), - 1);

  else if (ACE_Reactor::instance ()->register_handler
	   (&peer_acceptor, ACE_Event_Handler::READ_MASK) == - 1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), - 1);

  ACE_DEBUG ((LM_DEBUG, " (%P|%t) starting up daemon\n"));

  ACE_Reactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG, " (%P|%t) shutting down server daemon\n"));

#else
  ACE_ERROR ((LM_ERROR, "ACE_IOSTREAM not supported on this platform\n"));
#endif /* !ACE_LACKS_ACE_IOSTREAM */
  return 0;
}


#if !defined (ACE_LACKS_ACE_IOSTREAM)
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor <Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_IOStream_T <ACE_SOCK_Stream>;
template class ACE_Streambuf_T <ACE_SOCK_Stream>;
template class ACE_Svc_Handler <ACE_SOCK_IOStream, ACE_INET_Addr, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor <Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_IOStream_T <ACE_SOCK_Stream>
#pragma instantiate ACE_Streambuf_T <ACE_SOCK_Stream>
#pragma instantiate ACE_Svc_Handler <ACE_SOCK_IOStream, ACE_INET_Addr, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* !ACE_LACKS_ACE_IOSTREAM */
