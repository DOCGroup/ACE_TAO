// Test program for the event transceiver.  This program can play the
// @(#)transceiver.cpp	1.1	10/18/96

// role of either Consumer or Supplier.  You can terminate this
// program by typing ^C....

#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"
#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_THREADS)

class Event_Transceiver : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //     Generate and receives messages from the event server.
  // 
  // = DESCRIPTION
  //     This class is both a consumer and supplier of events, i.e.,
  //     it is a ``transceiver.''
{
public:
  Event_Transceiver (void);

  // = Svc_Handler hook called by the <ACE_Connector>.
  virtual int open (void *);
  // Initialize the transceiver when we are connected.

  // = Demultplexing hooks from the <ACE_Reactor>.
  virtual int handle_input (ACE_HANDLE);
  // Receive data from STDIN or socket.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Close down via SIGINT.

private:
  int receiver (void);
  // Reads data from socket and writes to ACE_STDOUT.

  int forwarder (void);
  // Writes data from ACE_STDIN to socket.
};

// Close down via SIGINT.

int 
Event_Transceiver::handle_signal (int signum, 
				       siginfo_t *, 
				       ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));
  
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

Event_Transceiver::Event_Transceiver (void)
{
  ACE_Sig_Set sig_set;
  
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  if (ACE_Service_Config::reactor ()->register_handler 
      (sig_set, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
}

int
Event_Transceiver::open (void *)
{
  if (ACE_Service_Config::reactor ()->register_handler
      (this->peer ().get_handle (), 
       this,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
  else if (ACE::register_stdin_handler (this,
					ACE_Service_Config::reactor (),
					ACE_Service_Config::thr_mgr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_stdin_handler"), -1);
  return 0;
}

int
Event_Transceiver::handle_input (ACE_HANDLE handle)
{
  if (handle == ACE_STDIN)
    return this->forwarder ();
  else
    return this->receiver ();
}


int
Event_Transceiver::forwarder (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) entering transceiver forwarder\n"));

  char buf[BUFSIZ];
  ssize_t n = ACE_OS::read (ACE_STDIN, buf, sizeof buf);
  int result = 0;

  if (n <= 0 || this->peer ().send_n (buf, n) != n)
    result = -1;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) leaving transceiver forwarder\n"));
  return result;
}

int
Event_Transceiver::receiver (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) entering transceiver receiver\n"));

  char buf[BUFSIZ];

  ssize_t n = this->peer ().recv (buf, sizeof buf);
  int result = 0;

  if (n <= 0 || ACE_OS::write (ACE_STDOUT, buf, n) != n)
    result = -1;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) leaving transceiver receiver\n"));
  return result;
}

// Port number of event server.
static u_short port_number;

// Name of event server.
static char *host_name;

// Handle the command-line arguments.

static void 
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "h:p:");

  port_number = ACE_DEFAULT_SERVER_PORT;
  host_name = ACE_DEFAULT_SERVER_HOST;

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'h':
	host_name = get_opt.optarg;
	break;
      case 'p':
	port_number = ACE_OS::atoi (get_opt.optarg);
	break;
      default:
	ACE_ERROR ((LM_ERROR, 
		    "usage: %n [-p portnum] [-h host_name]\n%a", 1));
	/* NOTREACHED */
	break;
      }
}

int 
main (int argc, char *argv[]) 
{ 
  ACE_Service_Config daemon (argv[0]);

  parse_args (argc, argv);

  // Establish the connection.
  ACE_Connector<Event_Transceiver, ACE_SOCK_CONNECTOR> connector;
  Event_Transceiver transceiver;

  if (connector.connect (&transceiver, ACE_INET_Addr (port_number, host_name)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", host_name), 1);

  // Run event loop until either the event server shuts down or we get
  // a SIGINT.
  ACE_Service_Config::run_reactor_event_loop ();
  return 0;
}
#else
int 
main (void)
{
  ACE_ERROR ((LM_ERROR, "test not defined for this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
