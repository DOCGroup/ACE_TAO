// $Id$

// Test program for the event transceiver.  This program can play the
// role of either Consumer or Supplier.  You can terminate this
// program by typing ^C....

#include "ace/Service_Config.h"
#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Get_Opt.h"

class Event_Transceiver : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //     Generate and receives messages from the event server.
  //
  // = DESCRIPTION
  //     This class is both a consumer and supplier of events, i.e.,
  //     it's a ``transceiver.''
public:
  // = Initialization method.
  Event_Transceiver (int argc, char *argv[]);
  // Performs the actual initialization.

  Event_Transceiver (void);
  // No-op constructor (required by the <ACE_Connector>).

  // = Svc_Handler hook called by the <ACE_Connector>.
  virtual int open (void *);
  // Initialize the transceiver when we are connected.

  // = Demultplexing hooks from the <ACE_Reactor>.
  virtual int handle_input (ACE_HANDLE);
  // Receive data from STDIN or socket.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Close down via SIGINT.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Close down the event loop.

private:
  int receiver (void);
  // Reads data from socket and writes to ACE_STDOUT.

  int transmitter (void);
  // Writes data from ACE_STDIN to socket.

  int parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

  u_short port_number_;
  // Port number of event server.

  char *host_name_;
  // Name of event server.

  char *role_;
  // Are we playing the Consumer or Supplier role?
};

// Handle the command-line arguments.

int
Event_Transceiver::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "Ch:p:S");

  this->port_number_ = ACE_DEFAULT_SERVER_PORT;
  this->host_name_ = ACE_DEFAULT_SERVER_HOST;
  this->role_ = "Supplier";

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'C':
	this->role_ = "Consumer";
	break;
      case 'h':
	this->host_name_ = get_opt.optarg;
	break;
      case 'p':
	this->port_number_ = ACE_OS::atoi (get_opt.optarg);
	break;
      case 'S':
	this->role_ = "Supplier";
	break;
      default:
	ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %n [-CS] [-h host_name] [-p portnum] \n"),
                          -1);
	/* NOTREACHED */
	break;
      }

  // Increment by 1 if we're the supplier to mirror the default
  // behavior of the Event_Server (which sets the Consumer port to
  // ACE_DEFAULT_SERVER_PORT and the Supplier port to
  // ACE_DEFAULT_SERVER_PORT + 1).  Note that this is kind of a
  // hack...
  if (ACE_OS::strcmp (this->role_, "Supplier") == 0
      && this->port_number_ == ACE_DEFAULT_SERVER_PORT)
    this->port_number_++;
}

int
Event_Transceiver::handle_close (ACE_HANDLE,
				 ACE_Reactor_Mask)
{
  ACE_Reactor::end_event_loop ();
  return 0;
}

// Close down via SIGINT or SIGQUIT.

int
Event_Transceiver::handle_signal (int signum,
				  siginfo_t *,
				  ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) received signal %S\n",
              signum));

  ACE_Reactor::end_event_loop ();
  return 0;
}

Event_Transceiver::Event_Transceiver (void)
{
}

Event_Transceiver::Event_Transceiver (int argc, char *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "parse_args"));
  else
    {
      ACE_Sig_Set sig_set;

      sig_set.sig_add (SIGINT);
      sig_set.sig_add (SIGQUIT);

      // Register to handle the SIGINT and SIGQUIT signals.
      if (ACE_Reactor::instance ()->register_handler
          (sig_set,
           this) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "register_handler"));

      // We need to register <this> here before we're connected since
      // otherwise <get_handle> will return the connection socket
      // handle for the peer.
      else if (ACE::register_stdin_handler (this,
                                            ACE_Reactor::instance (),
                                            ACE_Thread_Manager::instance ()) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "register_stdin_handler"));

      // Address of the server.
      ACE_INET_Addr server_addr (this->port_number_,
                                 this->host_name_);

      ACE_Connector<Event_Transceiver, ACE_SOCK_CONNECTOR> connector;

      // We need a pointer here because connect takes a reference to a
      // pointer!
      Event_Transceiver *etp = this;

      // Establish the connection to the Event Server.
      if (connector.connect (etp,
                             server_addr) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    this->host_name_));
    }
}

int
Event_Transceiver::open (void *)
{
  // Register ourselves to be notified when there's data to read on
  // the socket.
  if (ACE_Reactor::instance ()->register_handler
      (this,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "register_handler"),
		       -1);
  return 0;
}

int
Event_Transceiver::handle_input (ACE_HANDLE handle)
{
  // Determine whether we play the role of a consumer or a supplier.
  if (handle == ACE_STDIN)
    return this->transmitter ();
  else
    return this->receiver ();
}

int
Event_Transceiver::transmitter (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) entering %s transmitter\n",
	      this->role_));

  char buf[BUFSIZ];
  ssize_t n = ACE_OS::read (ACE_STDIN, buf, sizeof buf);
  int result = 0;

  if (n <= 0 || this->peer ().send_n (buf, n) != n)
    result = -1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) leaving %s transmitter\n",
	      this->role_));
  return result;
}

int
Event_Transceiver::receiver (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) entering %s receiver\n",
	      this->role_));

  char buf[BUFSIZ];

  ssize_t n = this->peer ().recv (buf, sizeof buf);
  int result = 0;

  if (n <= 0 
      || ACE_OS::write (ACE_STDOUT, buf, n) != n)
    result = -1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) leaving %s receiver\n",
	      this->role_));
  return result;
}

int
main (int argc, char *argv[])
{
  if (ACE_Service_Config::open (argv[0]) == -1
      && errno != ENOENT)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n", 
                       "open"),
                       -1);

  // Create and initialize the transceiver.
  Event_Transceiver transceiver (argc, argv);

  // Demonstrate how we can check if a constructor failed...
  if (ACE_LOG_MSG->op_status () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "Event_Transceiver constructor failed"),
                       -1);


  // Run event loop until either the event server shuts down or we get
  // a SIGINT.
  ACE_Reactor::run_event_loop ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<Event_Transceiver, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Svc_Tuple<Event_Transceiver>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<Event_Transceiver, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Svc_Tuple<Event_Transceiver>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Event_Transceiver> *, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
