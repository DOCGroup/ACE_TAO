// Client_Logging_Handler.cpp
// $Id$

#define ACE_BUILD_SVC_DLL
#include "ace/Service_Config.h"
#include "ace/Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/FIFO_Recv_Msg.h"
#include "Client_Logging_Handler.h"

class ACE_Svc_Export ACE_Client_Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //    This client logging daemon is a mediator that receives logging
  //    records from local applications processes and forwards them to
  //    the server logging daemon running on another host. 
  //
  // = DESCRIPTION
  //
{
public:
  // = Initialization and termination.

  ACE_Client_Logging_Handler (const char rendezvous[]);
  // Default constructor.

  virtual int open (void * = 0);
  // Activate this instance of the <ACE_Client_Logging_Handler>
  // (called by the <ACE_Client_Logging_Connector>).

  virtual ACE_HANDLE get_handle (void) const;
  // Return the handle of the message_fifo_;

  virtual int close (u_long);
  // Called when object is removed from the ACE_Reactor

protected:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle SIGINT.

  virtual int handle_input (ACE_HANDLE);
  // Receive logging records from applications.

  virtual int handle_exception (ACE_HANDLE);
  // Receive logging records from applications.  This is necessary to handle
  // madness with UNIX select, which can't deal with MSG_BAND data easily due
  // to its overly simple interface...  This just calls <handle_input>.

  virtual int handle_output (ACE_HANDLE);
  // Called back when it's ok to send.
  
  int send (ACE_Log_Record &log_record);
  // Send the <log_record> to the logging server.

  ACE_FIFO_Recv_Msg message_fifo_;
  // Message queue we use to receive logging records from clients.
  
  ACE_HANDLE logging_output_;
  // This is either a SOCKET (if we're connected to a logging server)
  // or ACE_STDOUT.

  static void stderr_output (int = 0);
};

ACE_Client_Logging_Handler::ACE_Client_Logging_Handler (const char rendezvous[])
  : logging_output_ (ACE_STDOUT)
{
  if (ACE_OS::unlink (rendezvous) == -1 && errno == EACCES)
    ACE_ERROR ((LM_ERROR, "%p\n", "unlink"));
  else if (this->message_fifo_.open (rendezvous) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));
  // Register message FIFO to receive input from clients.  Note that we need to
  // put the EXCEPT_MASK here to deal with SVR4 MSG_BAND data correctly...
  else if (ACE_Service_Config::reactor ()->register_handler
	   (this->message_fifo_.get_handle (), this,
	    ACE_Event_Handler::READ_MASK | ACE_Event_Handler::EXCEPT_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%n: %p\n", 
		"register_handler (message_fifo)"));
  ACE_DEBUG ((LM_DEBUG, 
	      "opened fifo at %s on handle %d\n",
	     rendezvous,
	     this->message_fifo_.get_handle ()));
}

// This is called when a <send> to the logging server fails...

int
ACE_Client_Logging_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Client_Logging_Connector::handle_signal");
//  return 0;
  return -1;
}

int  
ACE_Client_Logging_Handler::open (void *)
{
  ACE_INET_Addr server_addr;

  this->logging_output_ = this->peer ().get_handle ();

  // Register ourselves to receive SIGPIPE so we can attempt
  // reconnections.
  if (ACE_Service_Config::reactor ()->register_handler (SIGPIPE, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n", 
		       "register_handler (SIGPIPE)"), -1);

  // Figure out what remote port we're really bound to.
  else if (this->peer ().get_remote_addr (server_addr) == -1)
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
  return this->message_fifo_.get_handle ();
}


// Receive a logging record from an application.

int
ACE_Client_Logging_Handler::handle_input (ACE_HANDLE handle)
{
  if (handle == this->message_fifo_.get_handle ())
    {
      // We're getting a logging message from a local application.

      ACE_Log_Record log_record;
      ACE_Str_Buf msg ((void *) &log_record,
		       0, sizeof log_record);

      ACE_DEBUG ((LM_DEBUG, "in handle_input\n"));
      if (this->message_fifo_.recv (msg) == -1)
 	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_FIFO_Recv_Msg::recv"), -1);
      else if (this->send (log_record) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 0);
      return 0;
    }
  else if (handle == this->peer ().get_handle ())
    {
      // We're getting a message from the logging server!
      ACE_ASSERT (!"this shouldn't happen yet...\n");
    }
  return 0;
}

// Receive a logging record from an application send via a non-0 MSG_BAND...  
// This just calls handle_input().

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
  this->message_fifo_.close ();
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
	// Switch over to logging to stdout for now.  In the long
	// run, we'll need to queue up the message, try to
	// reestablish a connection, and then send the queued data
	// once we've reconnect to the logging server.
	this->logging_output_ = ACE_STDOUT;
    }

  return 0;
}


class ACE_Client_Logging_Connector : public ACE_Connector<ACE_Client_Logging_Handler, ACE_SOCK_CONNECTOR>
  // = TITLE
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
  
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle SIGINT.

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
  // Filename where the FIFO will listen for application logging
  // records.

  ACE_Client_Logging_Handler *handler_;
  // Pointer to the handler that does the work.
};

int 
ACE_Client_Logging_Connector::fini (void)
{
  this->handler_->destroy ();
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

  // Register ourselves to receive SIGINT so we can shutdown
  // gracefully.
  if (ACE_Service_Config::reactor ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n", 
		       "register_handler (SIGINT)"), -1);

  ACE_NEW_RETURN (this->handler_, 
		  ACE_Client_Logging_Handler (this->rendezvous_key_),
		  -1);

  // Establish connection with the server.
  if (this->connect (this->handler_, 
		     this->server_addr_, 
		     ACE_Synch_Options::synch) == -1)
    ACE_ERROR ((LM_ERROR, "%p, using stdout\n", 
		"can't connect to logging server"));
  return 0;
}

int
ACE_Client_Logging_Connector::parse_args (int argc, char *argv[])
{
  this->rendezvous_key_ = ACE_DEFAULT_RENDEZVOUS;
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
	  break;
	}
    }

  this->server_addr_.set (this->server_port_, this->server_host_);
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

// Signal the server to shutdown gracefully.

int
ACE_Client_Logging_Connector::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Client_Logging_Connector::handle_signal");
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the
// single-threaded logging server.

ACE_SVC_FACTORY_DEFINE (ACE_Client_Logging_Connector)

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Connector<ACE_Client_Logging_Handler, ACE_SOCK_CONNECTOR>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
