// Token_Handler.cpp
// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Token_Request_Reply.h"
#include "ace/Token_Collection.h"
#include "ace/Local_Tokens.h"
#include "Token_Handler.h"

class ACE_Svc_Export ACE_Token_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //    Product object created by an <ACE_Token_Acceptor>.  A
  //    <Token_Handler> exchanges messages with a <Token_Proxy> object
  //    on the client-side.
  //
  // = DESCRIPTION
  //   This class is the main workhorse of the ACE Token service.  It
  //   receives token operation requests from remote clients and turns
  //   them into calls on local tokens (acquire, release, renew, and
  //   remove).  In OMG CORBA terms, it is an object adapter. It also
  //   schedules and handles timeouts that are used to support "timed
  //   waits."  Clients used timed waits to bound the amount of time
  //   they block trying to get a token.

{
public:
  // = Initialization and termination.

  ACE_Token_Handler (ACE_Thread_Manager * = 0);
  // Default constructor.

  // = Accessor and mutator methods.

  // = Remote operations "exported" to a client.
  virtual int acquire (ACE_Token_Proxy *proxy);
  // Try to acquire the token.
  // Precondition: client *may* hold the token already (i.e.,
  // supports recursive acquisitions).

  virtual int try_acquire (ACE_Token_Proxy *proxy);
  // Try to acquire the token.

  virtual int release (ACE_Token_Proxy *proxy);
  // Release the token and allow the next client that is waiting to
  // proceed.  Preconditions: client must hold the token.

  virtual int renew (ACE_Token_Proxy *proxy);
  // Yield the token if any clients are waiting, otherwise keep the
  // token.  Preconditions: client must hold the token.

  virtual int remove (ACE_Token_Proxy *proxy);
  // Remove the specified token from the Token_Map.  Preconditions:
  // ACE_Token must exist.  @@ Any other preconditions, e.g., must
  // client hold token, must there be no waiters, etc.?

  void sleep_hook (void);
  // Called by TS_[Mutex,RLock,WLock] when we hold the mutex and
  // someone wants it.

  void token_acquired (ACE_TPQ_Entry *);
  // Called by TS_[Mutex,RLock,WLock] when we are waiting and acquire
  // the mutex.

protected:
  // = Low level routines for framing requests, dispatching
  // operations, and returning replies.

  virtual int abandon (int send_error);
  // Our connection has been closed.

  virtual int recv_request (void);
  // Receive, frame, and decode the client's request.
  
  virtual int dispatch (void);
  // Dispatch the appropriate operation to handle the client's
  // request.
  
  virtual int send_reply (ACE_UINT32 errnum);
  // Create and send a reply to the client.

  // = Demultiplexing hooks.
  virtual int handle_input (ACE_HANDLE);
  // Callback method invoked by the <ACE_Reactor> when client events
  // arrive.

  // = Timer hook.
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
  // Enable clients to limit the amount of time they wait for a token.

  ACE_Token_Proxy *get_proxy (void);
  // return a proxy for the calling client_id and token name.

private:

  virtual ACE_Token_Proxy *create_proxy (void);
  // Switches on the type of token_request_ and creates a new
  // Token_Proxy. 

  ACE_Synch_Options request_options_;
  // Keeps track of the synchronization options (i.e., the timeout
  // interval).

  ACE_Token_Collection collection_;
  // collection of the client's token proxies.

  int timeout_id_;
  // ID returned by the Reactor that is used to kill registered timers
  // when a token operation times out.

  ACE_Token_Request token_request_;
  // Cache request from the client.

  ACE_Token_Reply token_reply_;
  // Cache reply to the client.
};

// = DESCRIPTION of ACE_TS_* classes:
//     When Tokens are released, waiting token proxies are notified
//     when the releasing thread calls token_acquired on the waiting
//     proxy.  The Token Server specializes ACE_Token_Proxy to
//     redefine the implementation of token_acquired.  When
//     token_acquired is called, the Token_Handler can then send the
//     response back over the socket connection to unblock the
//     client side.
//     Since only the Token_Handler uses ACE_TS_Mutex, we've moved
//     the definition to the .cpp file.

class ACE_TS_Mutex : public ACE_Local_Mutex
  // = TITLE
  //     ACE_TS_Mutex  -- ACE_*T*oken_*S*erver_Mutex
{
public:
  ACE_TS_Mutex (const char *name,
		ACE_Token_Handler *th);
  // Creation.

protected:
  virtual void sleep_hook (void);
  // Somebody wants our token!

  virtual void token_acquired (ACE_TPQ_Entry *);
  // We've been taken off the waiters list and given the token!  Call
  // the Token_Handler associated at construction, so it can tell the
  // remote client.

  ACE_TS_Mutex (const ACE_TS_Mutex &);
  // Duplication.

  virtual ACE_Token_Proxy *clone (void) const;
  // Return a deep copy.

private:
  ACE_Token_Handler* th_;
  // The Token Handler associated with this proxy.  Set at
  // construction and notified when blocking acquires succeed.
};

class ACE_TS_RLock : public ACE_Local_RLock
  // = TITLE
  //     ACE_TS_RLock  -- ACE_*T*oken_*S*erver_RLock
{
public:
  ACE_TS_RLock (const char *name,
		ACE_Token_Handler *th);
  // Creation.

protected:
  virtual void sleep_hook (void);
  // Somebody wants our token!

  virtual void token_acquired (ACE_TPQ_Entry *);
  // We've been taken off the waiters list and given the token!  Call
  // the Token_Handler associated at construction, so it can tell the
  // remote client.

  ACE_TS_RLock (const ACE_TS_RLock&);
  // Duplication.

  virtual ACE_Token_Proxy *clone (void) const;
  // Return a deep copy.

private:
  ACE_Token_Handler* th_;
  // the Token Handler associated with this proxy.  Set at
  // construction and notified when blocking acquires succeed.
};

class ACE_TS_WLock : public ACE_Local_WLock
  // = TITLE
  //     ACE_TS_WLock  -- ACE_*T*oken_*S*erver_WLock
{
public:
  ACE_TS_WLock (const char *name,
		ACE_Token_Handler *th);
  // Creation.

protected:
  virtual void sleep_hook (void);
  // Somebody wants our token!

  virtual void token_acquired (ACE_TPQ_Entry *);
  // We've been taken off the waiters list and given the token!  Call
  // the Token_Handler associated at construction, so it can tell the
  // remote client.

  ACE_TS_WLock (const ACE_TS_WLock&);
  // Duplication.

  virtual ACE_Token_Proxy *clone (void) const;
  // Return a deep copy.

private:
  ACE_Token_Handler* th_;
  // the Token Handler associated with this proxy.  Set at
  // construction and notified when blocking acquires succeed.
};

// ************************************************************

class ACE_Token_Acceptor : public ACE_Strategy_Acceptor<ACE_Token_Handler, ACE_SOCK_ACCEPTOR>
  // = TITLE
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Strategy_Acceptor>.
{
public:
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

private:
  ACE_Schedule_All_Reactive_Strategy<ACE_Token_Handler> scheduling_strategy_;
  // The scheduling strategy is designed for Reactive services.
};

int
ACE_Token_Acceptor::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Token_Acceptor::parse_args");

  this->service_port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_LOG_MSG->open ("Token Service");

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
	  break;
	}
    }

  this->service_addr_.set (this->service_port_);
  return 0;
}

int
ACE_Token_Acceptor::init (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Token_Acceptor::init");

  // Use the options hook to parse the command line arguments and set
  // options.
  this->parse_args (argc, argv);

  // Set the acceptor endpoint into listen mode (use the Singleton
  // global Reactor...).
  if (this->open (this->service_addr_, ACE_Service_Config::reactor (),
		  0, 0, 0, 
		  &this->scheduling_strategy_,
		  "Token Server", "ACE token service") == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p on port %d\n", 
		       "acceptor::open failed", 
		       this->service_addr_.get_port_number ()), -1);

  // Register ourselves to receive SIGINT so we can shutdown
  // gracefully.
  if (this->reactor ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n", 
		      "register_handler (SIGINT)"), -1);

  // Ignore SIGPIPE so that each <SVC_HANDLER> can handle this on its
  // own.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);

  ACE_INET_Addr server_addr;

  if (this->acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_remote_addr"), -1);
    
  ACE_DEBUG ((LM_DEBUG, 
	      "starting up Token Server at port %d on handle %d\n", 
	     server_addr.get_port_number (),
	     this->acceptor ().get_handle ()));
  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Naming
// Server.

ACE_SVC_FACTORY_DEFINE (ACE_Token_Acceptor)

// Default constructor.

ACE_Token_Handler::ACE_Token_Handler (ACE_Thread_Manager *tm)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> (tm),
    collection_ (1),
    timeout_id_ (0)
{
  ACE_TRACE ("ACE_Token_Handler::ACE_Token_Handler");
}

// Create and send a reply to the client.

/* VIRTUAL */ int 
ACE_Token_Handler::send_reply (ACE_UINT32 err)
{
  ACE_TRACE ("ACE_Token_Handler::send_reply");
  void *buf;
  size_t len;
  ssize_t n;

  this->token_reply_.errnum (err);

  len = this->token_reply_.encode (buf);

  n = this->peer ().send (buf, len);

  if (n != (ssize_t) len)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "%p, expected len = %d, actual len = %d\n",
		      "send failed", len, n), -1);
  else
    return 0;
}

// Acquire the token.

/* VIRTUAL */ int 
ACE_Token_Handler::acquire (ACE_Token_Proxy *proxy)
{
  ACE_TRACE ("ACE_Token_Handler::acquire");
  ACE_DEBUG ((LM_DEBUG, "in acquire for client id = %s\n", 
	     proxy->client_id ()));

  // @@ add notify in token request reply
  if (proxy->acquire (0, 0, ACE_Synch_Options::asynch) == -1)
    {
      if (errno != EWOULDBLOCK)
	// bad bad bad
	return this->send_reply (errno);

      // acquire would block
      if (request_options_[ACE_Synch_Options::USE_TIMEOUT] == 1)
	{
	  // check for polling
	  if ((request_options_.timeout ().sec () == 0) &&
	      (request_options_.timeout ().usec () == 0))
	    return this->send_reply (EWOULDBLOCK);

	  // schedule a timer
	  this->timeout_id_ = this->reactor ()->schedule_timer 
	    (this, (void *) proxy, request_options_.timeout ());
	  if (timeout_id_ == -1)
	    {
	      ACE_ERROR ((LM_ERROR, "%p\n", "schedule_timer"));
	      return this->send_reply (errno);
	    }
	}
      // send no reply.  wait until we acquire it or until the timer
      // goes off.
      return 0;
    }
  else // success
    return this->send_reply (ACE_Token_Reply::SUCCESS);
}

// Try to acquire the token.  Never block.

/* VIRTUAL */ int 
ACE_Token_Handler::try_acquire (ACE_Token_Proxy *proxy)
{
  ACE_TRACE ("ACE_Token_Handler::try_acquire");

  ACE_DEBUG ((LM_DEBUG, "in try_acquire for client id = %s\n",
	     proxy->client_id ()));

  // @@ add notify in token request reply
  if (proxy->tryacquire () == -1)
    return this->send_reply (errno);
  else
    return this->send_reply (ACE_Token_Reply::SUCCESS);
}

// Release the token and allow the next client that is waiting to
// proceed.

/* VIRTUAL */ int 
ACE_Token_Handler::release (ACE_Token_Proxy *proxy)
{
  ACE_TRACE ("ACE_Token_Handler::release");
  ACE_DEBUG ((LM_DEBUG, 
	      "in release for client id = %s\n", 
	      proxy->client_id ()));

  if (proxy->release (ACE_Synch_Options::asynch) == -1)
    // oops, it failed
    return this->send_reply (ACE_LOG_MSG->errnum ());

  // success
  if (this->timeout_id_ != 0)
    {
      this->reactor ()->cancel_timer (timeout_id_);
      this->timeout_id_ = 0;
    }
  
  return this->send_reply (ACE_Token_Reply::SUCCESS);
}

// Yield the token if any clients are waiting, otherwise keep the
// token.

/* VIRTUAL */ int 
ACE_Token_Handler::renew (ACE_Token_Proxy *proxy)
{
  ACE_TRACE ("ACE_Token_Handler::renew");

  ACE_DEBUG ((LM_DEBUG, "in renew for client id = %s\n", 
	     proxy->client_id ()));

  if (proxy->renew (token_request_.requeue_position (), 
		    ACE_Synch_Options::asynch) == -1)
    {
      int result = ACE_LOG_MSG->errnum ();
      if (result != EWOULDBLOCK)
	// bad bad bad
	return this->send_reply (result);

      // acquire would block
      if (request_options_[ACE_Synch_Options::USE_TIMEOUT] == 1)
	{
	  this->timeout_id_ = this->reactor ()->schedule_timer 
	    (this, 0, request_options_.timeout ());
	  if (timeout_id_ == -1)
	    {
	      ACE_ERROR ((LM_ERROR, "%p\n", "schedule_timer"));
	      return this->send_reply (ACE_LOG_MSG->errnum ());
	    }
	}
      // Send no reply.  wait until we acquire it or until the timer
      // goes off.
      return 0;
    }
  else
    // Success, we still hold the token.
    return this->send_reply (ACE_Token_Reply::SUCCESS);
}

/* VIRTUAL */ int 
ACE_Token_Handler::remove (ACE_Token_Proxy *proxy)
{
  ACE_TRACE ("ACE_Token_Handler::remove");
  ACE_DEBUG ((LM_DEBUG, "in remove for client id = %s\n", 
	     proxy->client_id ()));
  ACE_ERROR ((LM_ERROR, "sorry: ACE_Token_Handler::remove() is not implemented"));

  return this->send_reply (ENOTSUP);
}

// Enable clients to limit the amount of time they'll wait for a
// token.

/* VIRTUAL */ int 
ACE_Token_Handler::handle_timeout (const ACE_Time_Value &, 
				   const void *tp)
{
  ACE_TRACE ("ACE_Token_Handler::handle_timeout");

  this->timeout_id_ = 0;

  // @@ add a try acquire here!
  // Try to acquire the token, but if we can't get it immediately
  // then abandon the wait.
  //  if (this->try_acquire (&token_entry) == -1)
  //    return this->abandon (token_entry);

  ACE_Token_Proxy *proxy = (ACE_Token_Proxy *) tp;

  ACE_DEBUG ((LM_DEBUG, "in handle_timeout for client id = %s\n", 
	     proxy->client_id ()));

  // Remove ourselves from the waiter list.
  proxy->release ();

  this->send_reply (ETIME);
  return 0;
}

// Dispatch the appropriate operation to handle the client request.

ACE_Token_Proxy *
ACE_Token_Handler::get_proxy (void)
{
  ACE_TRACE ("ACE_Token_Handler::get_proxy");

  // See if the proxy already exists in the collection.
  ACE_Token_Proxy *proxy = collection_.is_member (token_request_.token_name ());

  // If not, create one.
  if (proxy == 0)
    {
      proxy = this->create_proxy ();

      // Put the new_proxy in this client_id's collection.
      if (collection_.insert (*proxy) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "insert failed\n"), 0);

      // Delete our copy (one was created in the collection).
      delete proxy;
      proxy = collection_.is_member (token_request_.token_name ());

      if (proxy == 0)
	ACE_ERROR_RETURN ((LM_ERROR, "is_member failed\n"), 0);

      // Set the client_id (it was set to 1 since we're
      // single-threaded.
      proxy->client_id (token_request_.client_id ());
    }

  return proxy;
}

ACE_Token_Proxy *
ACE_Token_Handler::create_proxy (void)
{
  ACE_TRACE ("ACE_Token_Handler::new_proxy");

  ACE_Token_Proxy *proxy;
  
  switch (token_request_.token_type ())
    {
    case ACE_Tokens::RWLOCK:
      if (token_request_.proxy_type () == ACE_RW_Token::READER)
	ACE_NEW_RETURN (proxy, 
			ACE_TS_RLock (token_request_.token_name (), this),
			0);
      else
	ACE_NEW_RETURN (proxy,
			ACE_TS_WLock (token_request_.token_name (), this),
			0);
      break;
    case ACE_Tokens::MUTEX:
      ACE_NEW_RETURN (proxy,
		      ACE_TS_Mutex (token_request_.token_name (), this),
		      0);
      break;
    default:
      // Nonexistent token type.
      errno = EINVAL;
      return 0;
    }

  // Check for failed new.
  if (proxy == 0)
    errno = ENOMEM;

  return proxy;
}

int
ACE_Token_Handler::dispatch (void)
{
  ACE_TRACE ("ACE_Token_Handler::dispatch");
  ACE_Token_Proxy *proxy = this->get_proxy ();

  if (proxy == 0)
    return -1;

  // Dispatch the appropriate request.
  switch (this->token_request_.operation_type ())
    {
    case ACE_Token_Request::ACQUIRE:
      return this->acquire (proxy);
    case ACE_Token_Request::TRY_ACQUIRE:
      return this->try_acquire (proxy);
    case ACE_Token_Request::RELEASE:
      return this->release (proxy);
    case ACE_Token_Request::RENEW:
      return this->renew (proxy);
    case ACE_Token_Request::REMOVE:
      return this->remove (proxy);
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "invalid type = %d\n",
			this->token_request_.operation_type ()), -1);
      /* NOTREACHED */
    }
}
  
// Receive, frame, and decode the client's request.
// Note, this method should use non-blocking I/O.

/* VIRTUAL */ int 
ACE_Token_Handler::recv_request (void)
{
  ACE_TRACE ("ACE_Token_Handler::recv_request");
  ssize_t n;

  // Read the first 4 bytes to get the length of the message
  // This implementation assumes that the first 4 bytes are
  // the length of the message.
  n = this->peer ().recv ((void *) &this->token_request_, 
			  sizeof (ACE_UINT32));

  switch (n)
    {
    case -1:
      /* FALLTHROUGH */
    default:
      ACE_ERROR ((LM_ERROR, "%p got %d bytes, expected %d bytes\n", 
		 "recv failed", n, sizeof (ACE_UINT32)));
      /* FALLTHROUGH */
    case 0:
      // We've shutdown unexpectedly, let's abandon the connection.
      this->abandon (0);
      return -1;
      /* NOTREACHED */
    case sizeof (ACE_UINT32):
      {
	// Transform the length into host byte order.
	ssize_t length = this->token_request_.length ();

	// Do a sanity check on the length of the message.
	if (length > (ssize_t) sizeof this->token_request_)
	  {
	    ACE_ERROR ((LM_ERROR, "length %d too long\n", length));
	    return this->abandon (1);
	  }

	// Receive the rest of the request message.
	// @@ beware of blocking read!!!.
	n = this->peer ().recv ((void *) (((char *) &this->token_request_)
					  + sizeof (ACE_UINT32)), 
				length - sizeof (ACE_UINT32));
	
	// Subtract off the size of the part we skipped over...
	if (n != (length - sizeof (ACE_UINT32)))
	  {
	    ACE_ERROR ((LM_ERROR, "%p expected %d, got %d\n", 
		       "invalid length", length, n));
	    return this->abandon (1);
	  }

	// Decode the request into host byte order.
	if (this->token_request_.decode () == -1)
	  {
	    ACE_ERROR ((LM_ERROR, "%p\n", "decode failed"));
	    return this->abandon (1);
	  }

	// if (OS::debug)
	this->token_request_.dump ();
      }
    }
  return 0;
}

// Callback method invoked by the ACE_Reactor when 
// events arrive from the client.

/* VIRTUAL */ int 
ACE_Token_Handler::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Token_Handler::handle_input");

  ACE_DEBUG ((LM_DEBUG, "****************** in handle_input\n"));

  if (this->recv_request () == -1)
    return -1;
  else
    return this->dispatch ();
}

void
ACE_Token_Handler::sleep_hook (void)
{
  ACE_TRACE ("ACE_Token_Handler::sleep_hook");
  // @@ what should we do?
  return;
}

void
ACE_Token_Handler::token_acquired (ACE_TPQ_Entry *)
{
  ACE_TRACE ("ACE_Token_Handler::token_acquired");

  if (this->timeout_id_ != 0)
    {
      this->reactor ()->cancel_timer (this->timeout_id_);
      this->timeout_id_ = 0;
    }
  
  this->send_reply (ACE_Token_Reply::SUCCESS);
}

int
ACE_Token_Handler::abandon (int send_error)
{
  ACE_TRACE ("ACE_Token_Handler::abandon");

  // Release ownership or remove us from the waiter list.
  if (this->timeout_id_ != 0)
    {
      this->reactor ()->cancel_timer (timeout_id_);
      this->timeout_id_ = 0;
    }

  // @@ release all tokens
  collection_.release ();
  
  if (send_error)
    return this->send_reply (EIO);
  else
    return -1;
}

// ************************************************************
// ************************************************************
// ************************************************************

ACE_TS_Mutex::ACE_TS_Mutex (const char *name,
			    ACE_Token_Handler *th)
: th_ (th), 
  ACE_Local_Mutex (name, 0, 1) // The 1 is debug.
{
  ACE_TRACE ("ACE_TS_Mutex::ACE_TS_Mutex");
}

ACE_TS_Mutex::ACE_TS_Mutex (const ACE_TS_Mutex &m)
: th_ (m.th_), 
  ACE_Local_Mutex (m)
{
  ACE_TRACE ("ACE_TS_Mutex::ACE_TS_Mutex");
  this->open (m.name (), m.ignore_deadlock_, m.debug_);
}

void 
ACE_TS_Mutex::sleep_hook (void)
{
  ACE_TRACE ("ACE_TS_Mutex::sleep_hook");
  th_->sleep_hook ();
  return;
}

void
ACE_TS_Mutex::token_acquired (ACE_TPQ_Entry *e)
{
  ACE_TRACE ("ACE_TS_Mutex::token_acquired");
  // Notify the token handler.
  th_->token_acquired (e);
  return;
}

ACE_Token_Proxy *
ACE_TS_Mutex::clone (void) const
{
  ACE_TRACE ("ACE_TS_Mutex::clone");
  ACE_Token_Proxy *temp;
  ACE_NEW_RETURN (temp, ACE_TS_Mutex (*this), 0);
  return temp;
}

// ************************************************************

ACE_TS_RLock::ACE_TS_RLock (const char *name,
			    ACE_Token_Handler *th)
: th_ (th), 
  ACE_Local_RLock (name, 0, 1) // The 1 is debug.
{
  ACE_TRACE ("ACE_TS_RLock::ACE_TS_RLock");
}

ACE_TS_RLock::ACE_TS_RLock (const ACE_TS_RLock &r)
: th_ (r.th_), 
  ACE_Local_RLock (r)
{
  ACE_TRACE ("ACE_TS_RLock::ACE_TS_RLock");
  this->open (r.name (), r.ignore_deadlock_, r.debug_);
}

void 
ACE_TS_RLock::sleep_hook (void)
{
  ACE_TRACE ("ACE_TS_RLock::sleep_hook");
  th_->sleep_hook ();
  return;
}

void
ACE_TS_RLock::token_acquired (ACE_TPQ_Entry *e)
{
  ACE_TRACE ("ACE_TS_RLock::token_acquired");
  // Notify the token handler.
  th_->token_acquired (e);
  return;
}

ACE_Token_Proxy *
ACE_TS_RLock::clone (void) const
{
  ACE_TRACE ("ACE_TS_RLock::clone");
  ACE_Token_Proxy *temp;
  
  ACE_NEW_RETURN (temp, ACE_TS_RLock (*this), 0);
  return temp;
}

// ************************************************************

ACE_TS_WLock::ACE_TS_WLock (const char *name,
			    ACE_Token_Handler *th)
: th_ (th), 
  ACE_Local_WLock (name, 0, 1) // The 1 is debug.
{
  ACE_TRACE ("ACE_TS_WLock::ACE_TS_WLock");
}

ACE_TS_WLock::ACE_TS_WLock (const ACE_TS_WLock &w)
: th_ (w.th_),
  ACE_Local_WLock (w)
{
  ACE_TRACE ("ACE_TS_WLock::ACE_TS_WLock");
  this->open (w.name (), w.ignore_deadlock_, w.debug_);
}

void 
ACE_TS_WLock::sleep_hook (void)
{
  ACE_TRACE ("ACE_TS_WLock::sleep_hook");
  th_->sleep_hook ();
  return;
}

void
ACE_TS_WLock::token_acquired (ACE_TPQ_Entry *e)
{
  ACE_TRACE ("ACE_TS_WLock::token_acquired");
  // Notify the token handler.
  th_->token_acquired (e);
  return;
}

ACE_Token_Proxy *
ACE_TS_WLock::clone (void) const
{
  ACE_TRACE ("ACE_TS_WLock::clone");
  ACE_Token_Proxy *temp;
  
  ACE_NEW_RETURN (temp, ACE_TS_WLock (*this), 0);
  return temp;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Strategy_Acceptor<ACE_Token_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Schedule_All_Reactive_Strategy<ACE_Token_Handler>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
