// $Id$

// This server daemon collects, formats, and displays logging
// information forwarded from client daemons running on other hosts in
// the network.  In addition, this example illustrates how to use the
// ACE_Reactor, ACE_Acceptor, ACE_Singleton, and the ACE_Test_and_Set
// components.

#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"

// ----------------------------------------

class Options
{
  // = TITLE
  //     Keeps track of the options.
public:
  void parse_args (int argc, char *argv[]);
  u_short port (void);

private:
  u_short port_;
  // Port number;
};

// Return the port number.

u_short
Options::port (void)
{
  return this->port_;
}

// Parse the command-line options.

void
Options::parse_args (int argc, char *argv[])
{
  this->port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_Get_Opt get_opt (argc, argv, "p:");

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'p':
	 this->port_ = ACE_OS::atoi (get_opt.optarg);
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

class Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //     Receive client message from the remote clients.
  //
  // = DESCRIPTION
  //     This class demonstrates how to receive messages from remote
  //     clients using the notification mechanisms in the
  //     <ACE_Reactor>.  In addition, it also illustrates how to
  //     utilize the <ACE_Reactor> timer mechanisms, as well.
{
public:
  // = Initialization and termination methods.
  Logging_Handler (void);

  // = Hooks for opening and closing handlers.
  virtual int open (void *);

protected:
  // = Demultiplexing hooks.
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);

private:
  char peer_name_[MAXHOSTNAMELEN + 1];
  // Host we are connected to.
};

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
  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
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
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client logger", this->peer_name_), -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
			 "(%P|%t) closing log daemon at host %s (fd = %d)\n",
			this->peer_name_, this->get_handle ()), -1);
      /* NOTREACHED */
    case sizeof (size_t):
      {
	ACE_Log_Record lp;

        len = ntohl (len);
	n = this->peer ().recv_n ((void *) &lp, len);

	if (n != len)
	  ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			    "client logger", this->peer_name_), -1);
	/* NOTREACHED */

	lp.decode ();

	if (lp.length () == n)
	  {
	    ACE_DEBUG ((LM_DEBUG, "(%P|%t) "));
	    lp.print (this->peer_name_, 1, cerr);
	  }
	else
	  ACE_ERROR ((LM_ERROR, "(%P|%t) error, lp.length = %d, n = %d\n",
		      lp.length (), n));
	break;
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client logger", this->peer_name_), -1);
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

      if (REACTOR::instance ()->register_handler
	  (this, ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) can't register with reactor\n"), -1);
      else if (REACTOR::instance ()->schedule_timer
	       (this,
		(const void *) this,
		ACE_Time_Value (2),
		ACE_Time_Value (2)) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "can'(%P|%t) t register with reactor\n"), -1);
      else
      	ACE_DEBUG ((LM_DEBUG,
		    "(%P|%t) connected with %s\n", this->peer_name_));
      return 0;
    }
}

int
main (int argc, char *argv[])
{
  // Acceptor factory.
  Logging_Acceptor peer_acceptor;

  OPTIONS::instance ()->parse_args (argc, argv);

  // We need to pass in REACTOR::instance () here so that we don't use
  // the default ACE_Reactor::instance ().

  if (peer_acceptor.open
      (ACE_INET_Addr (OPTIONS::instance ()->port ()),
       REACTOR::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  else if (REACTOR::instance ()->register_handler
	   (&peer_acceptor, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"), -1);

  // Register QUIT_HANDLER to receive SIGINT commands.  When received,
  // QUIT_HANDLER becomes "set" and thus, the event loop below will
  // exit.
  else if (REACTOR::instance ()->register_handler
	   (SIGINT, QUIT_HANDLER::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"), -1);

  // Run forever, performing logging service.

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up server logging daemon\n"));

  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance ()->is_set () == 0)
    REACTOR::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down server logging daemon\n"));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Logging_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>;
template class ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>;
template class ACE_Singleton<Options, ACE_Null_Mutex>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Logging_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<Options, ACE_Null_Mutex>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

