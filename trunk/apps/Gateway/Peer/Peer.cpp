// $Id$

// These classes process Supplier/Consumer events sent from the gateway
// (gatewayd) to its various peers (peerd).  These classes works as
// follows:
//
// 1. Gateway_Acceptor creates a listener endpoint and waits passively
//    for gatewayd to connect with it.
//
// 2. When gatewayd connects, Peer_Acceptor creates an
//    Peer_Handler object that sends/receives events from
//    gatewayd.
//
// 3. The Peer_Handler waits for gatewayd to inform it of its supplier
//    ID, which is prepended to all outgoing events sent from peerd.
//
// 4. Once the supplier ID is set, peerd periodically sends events to
//    gatewayd.  Peerd also receives and "processes" events
//    forwarded to it from gatewayd.  In this program, peerd
//    "processes" events by writing them to stdout.

#define ACE_BUILD_SVC_DLL

#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "Event.h"
#include "Peer.h"

static int verbose = 0;

// Handle Peer events arriving as events.

class ACE_Svc_Export Peer_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  // = Initialization and termination methods.
  Peer_Handler (void);
  // Initialize the peer.

  ~Peer_Handler (void);
  // Shutdown the Peer.

  virtual int open (void * = 0);
  // Initialize the handler (called by ACE_Acceptor::handle_input())

  virtual int handle_input (ACE_HANDLE);
  // Receive and process peer events.

  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);
  // Send a event to a gateway (may be queued if necessary).

  virtual int handle_output (ACE_HANDLE);
  // Finish sending a event when flow control conditions abate.

  virtual int handle_timeout (const ACE_Time_Value &,
			      const void *arg);
  // Periodically send events via ACE_Reactor timer mechanism.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Perform object termination.

protected:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;

  // We'll allow up to 16 megabytes to be queued per-output
  // channel!!!!  This is clearly a policy in search of refinement...
  enum { MAX_QUEUE_SIZE = 1024 * 1024 * 16 };

  ACE_INT32 proxy_id_;
  // Proxy ID of the peer (obtained from gatewayd).  For simplicity,
  // in this implementation we also use the Proxy ID as the Supplier
  // ID.  This might change in future releases.

  virtual int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put().

  virtual int recv (ACE_Message_Block *&);
  // Receive an Peer event from gatewayd.

  virtual int send (ACE_Message_Block *);
  // Send an Peer event to gatewayd.

  int xmit_stdin (void);
  // Receive a event from stdin and send it to the gateway.

  int (Peer_Handler::*do_action_) (void);
  // Pointer-to-member-function for the current action to run in this state.

  int await_supplier_id (void);
  // Action that receives the route id.

  int await_events (void);
  // Action that receives events.

  ACE_Message_Block *msg_frag_;
  // Keep track of event fragment to handle non-blocking recv's from gateway.

  size_t total_bytes_;
  // The total number of bytes sent/received to the gateway.
};

Peer_Handler::Peer_Handler (void)
  : proxy_id_ (0),
    msg_frag_ (0),
    total_bytes_ (0)
{
  this->msg_queue ()->high_water_mark (Peer_Handler::MAX_QUEUE_SIZE);
}

// Upcall from the ACE_Acceptor::handle_input() that turns control
// over to our application-specific Gateway handler.

int
Peer_Handler::open (void *a)
{
  ACE_DEBUG ((LM_DEBUG, "Gateway handler's handle = %d\n",
	     this->peer ().get_handle ()));

  // Call down to the base class to activate and register this
  // handler.
  if (this->inherited::open (a) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enable"), -1);

  char *to = ACE_OS::getenv ("TIMEOUT");
  int timeout = to == 0 ? 100000 : ACE_OS::atoi (to);

  // Schedule the time between disconnects.  This should really be a
  // "tunable" parameter.
  if (ACE_Reactor::instance ()->schedule_timer (this, 0, timeout) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "schedule_timer"));

  // If there are events left in the queue, make sure we enable the
  // ACE_Reactor appropriately to get them sent out.
  if (this->msg_queue ()->is_empty () == 0
      && ACE_Reactor::instance ()->schedule_wakeup
          (this, ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_wakeup"), -1);

  // First action is to wait to be notified of our supplier id.
  this->do_action_ = &Peer_Handler::await_supplier_id;
  return 0;
}

// Read events from stdin and send them to the gatewayd.

int
Peer_Handler::xmit_stdin (void)
{
  if (this->proxy_id_ != -1)
    {
      ACE_Message_Block *mb;

      ACE_NEW_RETURN (mb,
		      ACE_Message_Block (sizeof (Event)),
		      -1);

      Event *event = (Event *) mb->rd_ptr ();

      ssize_t n = ACE_OS::read (ACE_STDIN, event->data_, sizeof event->data_);

      switch (n)
	{
	case 0:
	  ACE_DEBUG ((LM_DEBUG, "stdin closing down\n"));

	  // Take stdin out of the ACE_Reactor so we stop trying to
	  // send events.
	  ACE_Reactor::instance ()->remove_handler
	    (ACE_STDIN, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::READ_MASK);
	  mb->release ();
	  break;
	case -1:
	  mb->release ();
	  ACE_ERROR ((LM_ERROR, "%p\n", "read"));
	  break;
	default:
	  // For simplicity, we'll use our proxy id as the supplier id
	  // (which we must store in network byte order).
	  event->header_.supplier_id_ = this->proxy_id_;
	  event->header_.len_ = n;
	  event->header_.priority_ = 0;
	  event->header_.type_ = 0;

	  // Convert all the fields into network byte order.
	  event->header_.encode ();

	  mb->wr_ptr (sizeof (Event_Header) + n);

	  if (this->put (mb) == -1)
	    {
	      if (errno == EWOULDBLOCK) // The queue has filled up!
		ACE_ERROR ((LM_ERROR, "%p\n",
			   "gateway is flow controlled, so we're dropping events"));
	      else
		ACE_ERROR ((LM_ERROR, "%p\n", "transmission failure in xmit_stdin"));

	      // Caller is responsible for freeing a ACE_Message_Block
	      // if failures occur.
	      mb->release ();
	    }
	}
    }
  return 0;
}

// Perform a non-blocking put() of event MB.  If we are unable to send
// the entire event the remainder is re-Taskd at the *front* of the
// Message_Queue.

int
Peer_Handler::nonblk_put (ACE_Message_Block *mb)
{
  // Try to send the event.  If we don't send it all (e.g., due to
  // flow control), then re-ACE_Task the remainder at the head of the
  // Message_Queue and ask the ACE_Reactor to inform us (via
  // handle_output()) when it is possible to try again.

  ssize_t n;

  if ((n = this->send (mb)) == -1)
    return -1;
  else if (errno == EWOULDBLOCK) // Didn't manage to send everything.
    {
      ACE_DEBUG ((LM_DEBUG,
		  "queueing activated on handle %d to supplier id %d\n",
		 this->get_handle (), this->proxy_id_));

      // ACE_Queue in *front* of the list to preserve order.
      if (this->msg_queue ()->enqueue_head
	  (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enqueue_head"), -1);

      // Tell ACE_Reactor to call us back when we can send again.
      if (ACE_Reactor::instance ()->schedule_wakeup
	  (this, ACE_Event_Handler::WRITE_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_wakeup"), -1);
      return 0;
    }
  else
    return n;
}

// Finish sending a event when flow control conditions abate.  This
// method is automatically called by the ACE_Reactor.

int
Peer_Handler::handle_output (ACE_HANDLE)
{
  ACE_Message_Block *mb = 0;

  ACE_DEBUG ((LM_DEBUG, "in handle_output\n"));
  // The list had better not be empty, otherwise there's a bug!

  if (this->msg_queue ()->dequeue_head
      (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) != -1)
    {
      switch (this->nonblk_put (mb))
	{
	case 0: 	  // Partial send.
	  ACE_ASSERT (errno == EWOULDBLOCK);
	  // Didn't write everything this time, come back later...
	  break;

	case -1:
	  // Caller is responsible for freeing a ACE_Message_Block if
	  // failures occur.
	  mb->release ();
	  ACE_ERROR ((LM_ERROR, "%p\n",
		      "transmission failure in handle_output"));

	  /* FALLTHROUGH */
	default: // Sent the whole thing.

	  // If we succeed in writing the entire event (or we did not
	  // fail due to EWOULDBLOCK) then check if there are more
	  // events on the Message_Queue.  If there aren't, tell the
	  // ACE_Reactor not to notify us anymore (at least until
	  // there are new events queued up).

	  if (this->msg_queue ()->is_empty ())
	    {
	      ACE_DEBUG ((LM_DEBUG,
			  "queue now empty on handle %d to supplier id %d\n",
			  this->get_handle (),
			  this->proxy_id_));

	      if (ACE_Reactor::instance ()->cancel_wakeup
		  (this, ACE_Event_Handler::WRITE_MASK) == -1)
		ACE_ERROR ((LM_ERROR, "%p\n", "cancel_wakeup"));
	    }
	}
    }
  else
    ACE_ERROR ((LM_ERROR, "%p\n", "dequeue_head"));
  return 0;
}

// Send an event to a peer (may block if necessary).

int
Peer_Handler::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  if (this->msg_queue ()->is_empty ())
    // Try to send the event *without* blocking!
    return this->nonblk_put (mb);
  else
    // If we have queued up events due to flow control then just
    // enqueue and return.
    return this->msg_queue ()->enqueue_tail
      (mb, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

// Send an Peer event to gatewayd.

int
Peer_Handler::send (ACE_Message_Block *mb)
{
  ssize_t n;
  size_t len = mb->length ();

  if ((n = this->peer ().send (mb->rd_ptr (), len)) <= 0)
    return errno == EWOULDBLOCK ? 0 : n;
  else if (n < (ssize_t) len)
    {
      // Re-adjust pointer to skip over the part we did send.
      mb->rd_ptr (n);
      this->total_bytes_ += n;
    }
  else // if (n == length).
    {
      // The whole event is sent, we can now safely deallocate the
      // buffer.  Note that this should decrement a reference count...
      this->total_bytes_ += n;
      mb->release ();
      errno = 0;
    }
  ACE_DEBUG ((LM_DEBUG, "sent %d bytes, total bytes sent = %d\n",
	     n, this->total_bytes_));
  return n;
}

// Receive an Event from gatewayd.  Handles fragmentation.

int
Peer_Handler::recv (ACE_Message_Block *&mb)
{
  if (this->msg_frag_ == 0)
    // No existing fragment...
    ACE_NEW_RETURN (this->msg_frag_,
		    ACE_Message_Block (sizeof (Event)),
		    -1);

  Event *event = (Event *) this->msg_frag_->rd_ptr ();
  ssize_t header_received = 0;

  const ssize_t HEADER_SIZE = sizeof (Event_Header);
  ssize_t header_bytes_left_to_read =
    HEADER_SIZE - this->msg_frag_->length ();

  if (header_bytes_left_to_read > 0)
    {
      header_received = this->peer ().recv
	(this->msg_frag_->wr_ptr (), header_bytes_left_to_read);

      if (header_received == -1 /* error */
	  || header_received == 0  /* EOF */)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n",
		      "Recv error during header read"));
	  ACE_DEBUG ((LM_DEBUG,
		      "attempted to read %d bytes\n",
		      header_bytes_left_to_read));
	  this->msg_frag_ = this->msg_frag_->release ();
	  return header_received;
	}

      // Bump the write pointer by the amount read.
      this->msg_frag_->wr_ptr (header_received);

      // At this point we may or may not have the ENTIRE header.
      if (this->msg_frag_->length () < HEADER_SIZE)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Partial header received: only %d bytes\n",
		     this->msg_frag_->length ()));
	  // Notify the caller that we didn't get an entire event.
	  errno = EWOULDBLOCK;
	  return -1;
	}

      // Convert the header into host byte order so that we can access
      // it directly without having to repeatedly muck with it...
      event->header_.decode ();

      if (event->header_.len_ > sizeof event->data_)
	{
	  // This data_ payload is too big!
	  errno = EINVAL;
	  ACE_DEBUG ((LM_DEBUG,
		      "Data payload is too big (%d bytes)\n",
		      event->header_.len_));
	  return -1;
	}
    }

  // At this point there is a complete, valid header in Event.  Now we
  // need to get the event payload.  Due to incomplete reads this may
  // not be the first time we've read in a fragment for this message.
  // We account for this here.  Note that the first time in here
  // msg_frag_->wr_ptr() will point to event->data_.  Every time we do
  // a successful fragment read, we advance wr_ptr().  Therefore, by
  // subtracting how much we've already read from the
  // event->header_.len_ we complete the data_bytes_left_to_read...

  ssize_t data_bytes_left_to_read =
    ssize_t (event->header_.len_ - (msg_frag_->wr_ptr () - event->data_));

  ssize_t data_received =
    this->peer ().recv (this->msg_frag_->wr_ptr (), data_bytes_left_to_read);

  // Try to receive the remainder of the event.

  switch (data_received)
    {
    case -1:
      if (errno == EWOULDBLOCK)
	// This might happen if only the header came through.
	return -1;
      else
        /* FALLTHROUGH */;

    case 0: // Premature EOF.
      this->msg_frag_ = this->msg_frag_->release ();
      return 0;

    default:
      // Set the write pointer at 1 past the end of the event.
      this->msg_frag_->wr_ptr (data_received);

      if (data_received != data_bytes_left_to_read)
        {
          errno = EWOULDBLOCK;
	  // Inform caller that we didn't get the whole event.
          return -1;
        }
      else
        {
          // Set the read pointer to the beginning of the event.
          this->msg_frag_->rd_ptr (this->msg_frag_->base ());

	  mb = this->msg_frag_;

          // Reset the pointer to indicate we've got an entire event.
          this->msg_frag_ = 0;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) supplier id = %d, cur len = %d, total bytes read = %d\n",
		 event->header_.supplier_id_, event->header_.len_, data_received + header_received));
      if (verbose)
	ACE_DEBUG ((LM_DEBUG, "data_ = %*s\n", event->header_.len_ - 2, event->data_));
      return data_received + header_received;
    }
}

// Receive various types of input (e.g., Peer event from the
// gatewayd, as well as stdio).

int
Peer_Handler::handle_input (ACE_HANDLE sd)
{
  ACE_DEBUG ((LM_DEBUG, "in handle_input, sd = %d\n", sd));
  if (sd == ACE_STDIN) // Handle event from stdin.
    return this->xmit_stdin ();
  else
    // Perform the appropriate action depending on the state we are
    // in.
    return (this->*do_action_) ();
}

// Action that receives our supplier id from the Gateway.

int
Peer_Handler::await_supplier_id (void)
{
  ssize_t n = this->peer ().recv (&this->proxy_id_,
				  sizeof this->proxy_id_);

  if (n != sizeof this->proxy_id_)
    {
      if (n == 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "gatewayd has closed down unexpectedly\n"), -1);
      else
	ACE_ERROR_RETURN ((LM_ERROR,
			   "%p, bytes received on handle %d = %d\n",
			  "recv", this->get_handle (), n), -1);
    }
  else
    {
      this->proxy_id_ = ntohl (this->proxy_id_);
      ACE_DEBUG ((LM_DEBUG, "assigned proxy id %d\n",
		  this->proxy_id_));
    }

  // Transition to the action that waits for Peer events.
  this->do_action_ = &Peer_Handler::await_events;

  // Reset standard input.
  ACE_OS::rewind (stdin);

  // Register this handler to receive test events on stdin.

  if (ACE::register_stdin_handler (this,
				   ACE_Reactor::instance (),
				   ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_stdin_handler"), -1);
  return 0;
}

// Action that receives events.

int
Peer_Handler::await_events (void)
{
  ACE_Message_Block *mb = 0;
  ssize_t n = this->recv (mb);

  switch (n)
    {
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR, "gatewayd has closed down\n"), -1);
      /* NOTREACHED */
    case -1:
      if (errno == EWOULDBLOCK)
	// A short-read, we'll come back and finish it up later on!
	return 0;
      else
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), -1);
      /* NOTREACHED */
    default:
      {
	// We got a valid event, so let's process it now!  At the
	// moment, we just print out the event contents...

	Event *event = (Event *) mb->rd_ptr ();
	this->total_bytes_ += mb->length ();

	ACE_DEBUG ((LM_DEBUG,
		    "route id = %d, cur len = %d, total len = %d\n",
		    event->header_.supplier_id_,
		    event->header_.len_,
		    this->total_bytes_));
      if (verbose)
	ACE_DEBUG ((LM_DEBUG, "data_ = %s\n", event->data_));

	mb->release ();
	return 0;
      }
    }
}

// Periodically send events via ACE_Reactor timer mechanism.

int
Peer_Handler::handle_timeout (const ACE_Time_Value &, const void *)
{
  // Shut down the handler.
  return this->handle_close ();
}

Peer_Handler::~Peer_Handler (void)
{
  // Shut down the handler.
  this->handle_close ();
}

// Handle shutdown of the Peer object.

int
Peer_Handler::handle_close (ACE_HANDLE,
			    ACE_Reactor_Mask)
{
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      ACE_DEBUG ((LM_DEBUG, "shutting down Peer on handle %d\n",
		 this->get_handle ()));

      // Explicitly remove ourselves for ACE_STDIN (the ACE_Reactor
      // removes this->handle ().  Note that
      // ACE_Event_Handler::DONT_CALL instructs the ACE_Reactor *not*
      // to call this->handle_close(), which would otherwise lead to
      // recursion!).
      ACE_Reactor::instance ()->remove_handler
	(ACE_STDIN, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::READ_MASK);

      // Deregister this handler with the ACE_Reactor.
      if (ACE_Reactor::instance ()->remove_handler
	  (this, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::ALL_EVENTS_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "handle = %d: %p\n",
			  this->get_handle (), "remove_handler"), -1);

      // Close down the peer.
      this->peer ().close ();
    }
  return 0;
}

// A factory class that accept connections from gatewayd and
// dynamically creates a new Peer object to do the dirty work.

class ACE_Svc_Export Peer_Acceptor : public ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  // = Initialization and termination methods.
  Peer_Acceptor (void);
  // Create the Peer.

  virtual int init (int argc, char *argv[]);
  // Initialize the acceptor.

  virtual int info (char **, size_t) const;
  // Return info about this service.

  virtual int fini (void);
  // Perform termination.

  virtual int make_svc_handler (Peer_Handler *&);
  // Factory method that creates the Peer_Handler once.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle various signals (e.g., SIGPIPE, SIGINT, and SIGQUIT)

  void parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

private:
  Peer_Handler *peer_handler_;
  // Pointer to memory allocated exactly once.

  ACE_INET_Addr addr_;
  // Our addr.

  typedef ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR> inherited;
};

Peer_Acceptor::Peer_Acceptor (void)
  : addr_ (ACE_DEFAULT_PEER_SERVER_PORT)
{
  ACE_NEW (peer_handler_, Peer_Handler);
}

// Note how this method just passes back the pre-allocated Peer_Handler
// instead of having the ACE_Acceptor allocate a new one each time!

int
Peer_Acceptor::make_svc_handler (Peer_Handler *&sh)
{
  sh = this->peer_handler_;
  return 0;
}

int
Peer_Acceptor::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "signal %S occurred\n", signum));

  if (signum != SIGPIPE)
    // Shut down the main event loop.
    ACE_Reactor::end_event_loop();

  return 0;
}

// Returns information on the currently active service.

int
Peer_Acceptor::info (char **strp, size_t length) const
{
  char buf[BUFSIZ];
  char addr_str[BUFSIZ];

  ACE_INET_Addr addr;

  if (this->acceptor ().get_local_addr (addr) == -1)
    return -1;
  else if (addr.addr_to_string (addr_str, sizeof addr) == -1)
    return -1;

  ACE_OS::sprintf (buf, "%s\t %s/%s %s",
	     "Gateway peer daemon", addr_str, "tcp",
	     "# Gateway traffic generator and data sink\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

// Hook called by the explicit dynamic linking facility to terminate
// the peer.

int
Peer_Acceptor::fini (void)
{
  this->peer_handler_->destroy (); // Will trigger a delete.
  return inherited::fini ();
}

void
Peer_Acceptor::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "dp:v", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'p':
	  this->addr_.set (ACE_OS::atoi (get_opt.optarg));
	  break;
	case 'd':
	  break;
	case 'v': // Verbose mode.
	  verbose = 1;
	  break;
	default:
	  break;
	}
    }
}

// Hook called by the explicit dynamic linking facility to initialize
// the peer.

int
Peer_Acceptor::init (int argc, char *argv[])
{
  this->parse_args (argc, argv);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGPIPE);

  // Register ourselves to receive SIGINT and SIGQUIT so we can shut
  // down gracefully via signals.

#if defined (ACE_WIN32)
  if (ACE_Reactor::instance ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
#else
  if (ACE_Reactor::instance ()->register_handler (sig_set, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
#endif

  // Call down to the Acceptor's open() method.
  if (this->inherited::open (this->addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (this->acceptor ().get_local_addr (this->addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_local_addr"), -1);
  else
    ACE_DEBUG ((LM_DEBUG, "listening at port %d\n",
		this->addr_.get_port_number ()));
  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the Peer_Acceptor.

ACE_SVC_FACTORY_DEFINE (Peer_Acceptor)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

