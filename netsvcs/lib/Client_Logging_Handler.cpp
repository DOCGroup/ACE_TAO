// $Id$

// Client_Logging_Handler.cpp

#define ACE_BUILD_SVC_DLL
#include "ace/Get_Opt.h"
#include "Client_Logging_Handler.h"

#if defined (ACE_LACKS_FIFO)
#define DEFAULT_RENDEZVOUS "10012"
#else
#define DEFAULT_RENDEZVOUS ACE_DEFAULT_RENDEZVOUS
#endif /* ACE_LACKS_FIFO */

ACE_Client_Logging_Handler::ACE_Client_Logging_Handler (const char rendezvous[])
  : logging_output_ (ACE_STDOUT)
{
#if defined (ACE_LACKS_FIFO)
  if (this->acceptor_.open (ACE_INET_Addr (DEFAULT_RENDEZVOUS)) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));
  // Register passive-mode socket to receive connections from clients.
  else if (ACE_Reactor::instance ()->register_handler
	   (this->acceptor_.get_handle (),
	    this,
	    ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%n: %p\n", "register_handler"));
  ACE_DEBUG ((LM_DEBUG,
	      "opened acceptor socket at %s on handle %d\n",
	      rendezvous,
	      this->acceptor_.get_handle ()));
#else
  if (ACE_OS::unlink (rendezvous) == -1 && errno == EACCES)
    ACE_ERROR ((LM_ERROR, "%p\n", "unlink"));

  else if (this->msg_queue_.open (rendezvous) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));

  // Register message FIFO to receive input from clients.  Note that
  // we need to put the EXCEPT_MASK here to deal with SVR4 MSG_BAND
  // data correctly...
  else if (ACE_Reactor::instance ()->register_handler
	   (this->msg_queue_.get_handle (),
	    this,
	    ACE_Event_Handler::READ_MASK | ACE_Event_Handler::EXCEPT_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%n: %p\n",
		"register_handler"));
  ACE_DEBUG ((LM_DEBUG,
	      "opened fifo at %s on handle %d\n",
	     rendezvous,
	     this->msg_queue_.get_handle ()));
#endif /* ACE_LACKS_FIFO */
}

// This is called when a <send> to the logging server fails...

int
ACE_Client_Logging_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Client_Logging_Connector::handle_signal");
  return -1;
}

int
ACE_Client_Logging_Handler::open (void *)
{
  ACE_INET_Addr server_addr;

  this->logging_output_ = this->peer ().get_handle ();

  // Register ourselves to receive SIGPIPE so we can attempt
  // reconnections.
#if !defined (ACE_LACKS_FIFO)
  if (ACE_Reactor::instance ()->register_handler (SIGPIPE, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n",
		       "register_handler (SIGPIPE)"), -1);
#endif /* !ACE_LACKS_FIFO */

  // Figure out what remote port we're really bound to.
  if (this->peer ().get_remote_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_remote_addr"), -1);

  ACE_DEBUG ((LM_DEBUG,
	      "starting up Client Logging Daemon, "
	      "connected to port %d on handle %d\n",
	     server_addr.get_port_number (),
	     this->peer ().get_handle ()));
  return 0;
}

/* VIRTUAL */ ACE_HANDLE
ACE_Client_Logging_Handler::get_handle (void) const
{
  ACE_TRACE ("ACE_Client_Logging_Handler::get_handle");
#if defined (ACE_LACKS_FIFO)
  ACE_ERROR_RETURN ((LM_ERROR,
		     "get_handle() shouldn't be called\n"),
		     ACE_INVALID_HANDLE);
#else
  return this->msg_queue_.get_handle ();
#endif /* ACE_LACKS_FIFO */
}

// Receive a logging record from an application.

int
ACE_Client_Logging_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_DEBUG ((LM_DEBUG, "in handle_input, handle = %u\n", handle));

  if (handle == this->peer ().get_handle ())
    // We're getting a message from the logging server!
    ACE_ERROR_RETURN ((LM_ERROR, "received data from server!\n"), -1);
#if defined (ACE_LACKS_FIFO)
  else if (handle == this->acceptor_.get_handle ())
    {
      ACE_SOCK_Stream client_stream;

      if (this->acceptor_.accept (client_stream) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "accept"), -1);

      // Register message socket to receive input from clients.
      else if (ACE_Reactor::instance ()->register_handler
	       (client_stream.get_handle (),
		this,
		ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n",
			   "register_handler"), -1);
      return 0;
    }
#endif /* ACE_LACKS_FIFO */  

  // We're getting a logging message from a local application.

  ACE_Log_Record log_record;
  ACE_Str_Buf msg ((void *) &log_record,
		   0,
		   sizeof log_record);

#if defined (ACE_LACKS_FIFO)
  long length;

  // We need to use the ol' two-read trick here since TCP sockets
  // don't support framing natively.

  switch (ACE_OS::recv (handle,
			(char *) &length,
			sizeof length,
			MSG_PEEK))
    {
      // Handle shutdown and error cases.
    default:
    case -1:
    case 0:
      if (ACE_Reactor::instance ()->remove_handler
	  (handle,
	   ACE_Event_Handler::READ_MASK
	   | ACE_Event_Handler::DONT_CALL) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n",
			   "remove_handler"), -1);
      ACE_OS::closesocket (handle);
      ACE_DEBUG ((LM_DEBUG, "client closing down\n"));
      return 0;
      /* NOTREACHED */

    case sizeof length:
      // Process normal data reception.
      if (ACE_OS::recv (handle,
			(char *) &log_record,
			(int) length) != length)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), -1);
    }
#else
  // We've got a framed IPC mechanism, so we can just to a recv().
  if (this->msg_queue_.recv (msg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "ACE_FIFO_Recv_Msg::recv"),
		      0);
#endif /* ACE_LACKS_FIFO */

  // Forward the logging record to the server.
  if (this->send (log_record) == -1)
    ACE_ERROR ((LM_ERROR,
		"%p\n",
		"send"));
  return 0;
}

// Receive a logging record from an application send via a non-0
// MSG_BAND...  This just calls handle_input().

int
ACE_Client_Logging_Handler::handle_exception (ACE_HANDLE handle)
{
  return this->handle_input (handle);
}

// Called when object is removed from the ACE_Reactor

int
ACE_Client_Logging_Handler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "shutting down!!!\n"));

#if defined (ACE_LACKS_FIFO)
  if (ACE_Reactor::instance ()->remove_handler
      (this->acceptor_.get_handle (),
       ACE_Event_Handler::ACCEPT_MASK
       | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR ((LM_ERROR, "%n: %p\n",
		"remove_handler"));
#else
  if (ACE_Reactor::instance ()->remove_handler
      (this->msg_queue_.get_handle (),
       ACE_Event_Handler::READ_MASK 
       | ACE_Event_Handler::EXCEPT_MASK 
       | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR ((LM_ERROR, "%n: %p\n",
		"remove_handler"));
#endif /* ACE_LACKS_FIFO */

  this->msg_queue_.close ();
  this->destroy ();
  return 0;
}

int
ACE_Client_Logging_Handler::handle_output (ACE_HANDLE)
{
  return 0;
}

// Encodes the contents of log_record object using network byte-order
// and sends it to the logging server.

int
ACE_Client_Logging_Handler::send (ACE_Log_Record &log_record)
{
  if (this->logging_output_ == ACE_STDOUT)
	log_record.print ("<localhost>", 0, stderr);
  else
    {
      long len = log_record.length ();
      long encoded_len = htonl (len);

      log_record.encode ();

      if (this->peer ().send (4, &encoded_len, sizeof encoded_len,
			      (char *) &log_record, len) == -1)
	// Switch over to logging to stdout for now.  In the long run,
	// we'll need to queue up the message, try to reestablish a
	// connection, and then send the queued data once we've
	// reconnect to the logging server.
	this->logging_output_ = ACE_STDOUT;
    }

  return 0;
}

class ACE_Client_Logging_Connector : public ACE_Connector<ACE_Client_Logging_Handler, ACE_SOCK_CONNECTOR>
  // = TITLE
  //     This factory creates connections with the
  //     <Server_Logging_Acceptor>. 
  //
  // = DESCRIPTION
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Connector>.
{
protected:
  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  // Called when service is linked.

  virtual int fini (void);
  // Called when service is unlinked.

  virtual int info (char **strp, size_t length) const;
  // Called to determine info about the service.

  // = Scheduling hooks.
  virtual int suspend (void);
  virtual int resume (void);

private:
  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

  const char *server_host_;
  // Host where the logging server is located.

  u_short server_port_;
  // Port number where the logging server is listening for
  // connections.

  ACE_INET_Addr server_addr_;
  // Address of the logging server.

  const char *rendezvous_key_;
  // Communication endpoint where the client logging daemon will
  // listen for application logging records.

  ACE_Client_Logging_Handler *handler_;
  // Pointer to the handler that does the work.
};

int
ACE_Client_Logging_Connector::fini (void)
{
  if (this->handler_ != 0)
    this->handler_->close (0);
  return 0;
}

int
ACE_Client_Logging_Connector::info (char **strp, size_t length) const
{
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "%d/%s %s",
		   this->server_addr_.get_port_number (), "tcp",
		   "# client logging daemon\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

int
ACE_Client_Logging_Connector::init (int argc, char *argv[])
{
  ACE_LOG_MSG->open ("Client Logging Service");

  // Use the options hook to parse the command line arguments and set
  // options.
  this->parse_args (argc, argv);

  ACE_NEW_RETURN (this->handler_,
		  ACE_Client_Logging_Handler (this->rendezvous_key_),
		  -1);

  // Establish connection with the server.
  if (this->connect (this->handler_,
		     this->server_addr_,
		     ACE_Synch_Options::synch) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p, using stdout\n",
		  "can't connect to logging server"));
      this->handler_ = 0;
    }
  return 0;
}

int
ACE_Client_Logging_Connector::parse_args (int argc, char *argv[])
{
  this->rendezvous_key_ = DEFAULT_RENDEZVOUS;
  this->server_port_ = ACE_DEFAULT_LOGGING_SERVER_PORT;
  this->server_host_ = ACE_DEFAULT_SERVER_HOST;

  ACE_Get_Opt get_opt (argc, argv, "h:k:p:", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'h':
	  this->server_host_ = get_opt.optarg;
	  break;
	case 'k':
	  this->rendezvous_key_ = get_opt.optarg;
	  break;
	case 'p':
	  this->server_port_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	default:
	  ACE_ERROR_RETURN ((LM_ERROR,
			    "%n:\n[-p server-port]\n%a", 1),
			   -1);
	}
    }

  if (this->server_addr_.set (this->server_port_, 
			      this->server_host_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "set"), -1);

  return 0;
}

int
ACE_Client_Logging_Connector::suspend (void)
{
  // To be done...
  return 0;
}

int
ACE_Client_Logging_Connector::resume (void)
{
  // To be done...
  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the
// single-threaded logging server.

ACE_SVC_FACTORY_DEFINE (ACE_Client_Logging_Connector)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<ACE_Client_Logging_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<ACE_Client_Logging_Handler>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<ACE_Client_Logging_Handler> *>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<ACE_Client_Logging_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<ACE_Client_Logging_Handler> *, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<ACE_Client_Logging_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<ACE_Client_Logging_Handler>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<ACE_Client_Logging_Handler> *>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<ACE_Client_Logging_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<ACE_Client_Logging_Handler> *, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

