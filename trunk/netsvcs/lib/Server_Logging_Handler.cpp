// $Id$
// Server_Logging_Handler.cpp

#define ACE_BUILD_SVC_DLL
#include "ace/Synch.h"
#include "ace/TLI_Acceptor.h"
#include "ace/SOCK_Acceptor.h"

#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "Server_Logging_Handler.h"

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_1>
class ACE_Server_Logging_Handler : public ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_SYNCH_2>
{
  // = TITLE
  //    Product object created by an <ACE_Server_Logging_Acceptor>.  An
  //    <ACE_Server_Logging_Handler> receives, frames, and processes logging
  //    records.
  // 
  // = DESCRIPTION
  //     Defines the classes that perform server logging daemon
  //     functionality.
public:
  ACE_Server_Logging_Handler (ACE_Thread_Manager * = 0);

  virtual int open (void * = 0);
  // Hook called by <Server_Logging_Acceptor> when connection is
  // established.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Process remote logging records. 

protected:
  int handle_logging_record (void);
  // Receive the logging record from a client.
  
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static COUNTER request_count_;
  // Count the number of logging records that arrive.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  char host_name_[MAXHOSTNAMELEN + 1];
  // Name of the host we are connected to.
};

#if !defined (ACE_HAS_TLI)
#define LOGGING_PEER_ACCEPTOR ACE_SOCK_ACCEPTOR
#define LOGGING_PEER_STREAM ACE_SOCK_STREAM
#else /* use sockets */
#define LOGGING_PEER_ACCEPTOR ACE_TLI_ACCEPTOR
#define LOGGING_PEER_STREAM ACE_TLI_STREAM
#endif /* ACE_HAS_TLI */

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Track number of requests.
template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_1>
COUNTER ACE_Server_Logging_Handler<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_2>::request_count_ = (COUNTER) 0;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

typedef ACE_Server_Logging_Handler<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH>
	SERVER_LOGGING_HANDLER;

class ACE_Server_Logging_Acceptor : public ACE_Strategy_Acceptor<SERVER_LOGGING_HANDLER, LOGGING_PEER_ACCEPTOR>
  // = TITLE
  //     This class implements the ACE single-threaded logging service.
  //
  // = DESCRIPTION
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Strategy_Acceptor>.
{
public:
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

private:
  ACE_Schedule_All_Reactive_Strategy<SERVER_LOGGING_HANDLER> scheduling_strategy_;
  // The scheduling strategy is designed for Reactive services.
};

int
ACE_Server_Logging_Acceptor::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Server_Logging_Acceptor::parse_args");

  this->service_port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_LOG_MSG->open ("Logging Service");

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
ACE_Server_Logging_Acceptor::init (int argc, 
				   char *argv[])
{
  ACE_TRACE ("ACE_Server_Logging_Acceptor::init");

  // Use the options hook to parse the command line arguments and set
  // options.
  this->parse_args (argc, argv);

  // Set the acceptor endpoint into listen mode (use the Singleton
  // global Reactor...).
  if (this->open (this->service_addr_, ACE_Service_Config::reactor (),
		  0, 0, 0, 
		  &this->scheduling_strategy_,
		  "Logging Server", "ACE single-threaded logging service") == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p on port %d\n", 
		       "acceptor::open failed", 
		       this->service_addr_.get_port_number ()), -1);

  // Ignore SIGPIPE so that each <SVC_HANDLER> can handle this on its
  // own.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);
  ACE_UNUSED_ARG (sig);

  ACE_INET_Addr server_addr;

  // Figure out what port we're really bound to.
  if (this->acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_local_addr"), -1);
    
  ACE_DEBUG ((LM_DEBUG, 
	      "starting up Logging Server at port %d on handle %d\n", 
	     server_addr.get_port_number (),
	     this->acceptor ().get_handle ()));
  return 0;
}

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_1>
ACE_Server_Logging_Handler<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_2>::ACE_Server_Logging_Handler (ACE_Thread_Manager *)
{
  this->host_name_[0] = '\0'; // Initialize to a known state.
}

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_1> int
ACE_Server_Logging_Handler<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_2>::handle_logging_record (void)
{
  ssize_t len;
  // Lock used to serialize access to std output 
  // (this should be in the class, but the SunC++ compiler is broken...)
  static ACE_SYNCH_MUTEX lock;

  // Perform two recv's to emulate record-oriented semantiCLS.
  // Note that this code is not entirely portable since it
  // relies on the fact that sizeof (ssize_t) is the same
  // on both the sender and receiver side.  To correctly
  // handle this is painful, and we leave it as an exercise
  // for the reader ;-).

  ssize_t n = this->peer ().recv (&len, sizeof len);

  switch (n)
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "%p at host %s\n",
			"server logger", this->host_name_), -1);      
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR, "closing log daemon at host %s\n",
			this->host_name_), -1);	
      /* NOTREACHED */
    case sizeof (ssize_t):
      {
	ACE_Log_Record lp;
	
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
	u_long count = ++this->request_count_;
	ACE_DEBUG ((LM_DEBUG, "request count = %d\n", count));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

        len = ntohl (len);
	n = this->peer ().recv_n ((void *) &lp, len);
	if (n != len)
	  ACE_ERROR_RETURN ((LM_ERROR, "len = %d, %p at host %s\n",
			    n, "server logger", this->host_name_), -1);
	/* NOTREACHED */
	  
	lp.decode ();

	if (lp.length () == n)
	  {
	    // Serialize output, if necessary (i.e., if we are running
	    // in separate threads).
	    // ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, lock, -1);

	    lp.print (this->host_name_, 0, stderr);
	  }
	else
	  ACE_ERROR ((LM_ERROR, "error, lp.length = %d, n = %d\n",
		     lp.length (), n));
	break;
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "%p at host %s\n",
			"server logger", this->host_name_), -1);
      /* NOTREACHED */
    }

  return n;
}

// Hook called by Server_Logging_Acceptor when connection is
// established.

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_1> int
ACE_Server_Logging_Handler<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_2>::open (void *)
{
  // Register ourselves with the Reactor to enable subsequent
  // dispatching.
  if (ACE_Service_Config::reactor ()->register_handler 
      (this, ACE_Event_Handler::READ_MASK) == -1)
    return -1;

  ACE_PEER_STREAM_ADDR client_addr;

  // Determine the address of the client and display it.
  if (this->peer ().get_remote_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_remote_addr"), -1);

  ACE_OS::strncpy (this->host_name_, client_addr.get_host_name (), MAXHOSTNAMELEN + 1);

  ACE_DEBUG ((LM_DEBUG, "(%t) accepted connection from host %s on fd %d\n",
	      client_addr.get_host_name (), this->peer ().get_handle ()));

  // Shut off non-blocking IO if it was enabled...
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "disable"), -1);

  return 0;
}

// Callback routine for handling the reception of remote logging
// transmissions.

template <ACE_PEER_STREAM_1, class COUNTER, ACE_SYNCH_1> int
ACE_Server_Logging_Handler<ACE_PEER_STREAM_2, COUNTER, ACE_SYNCH_2>::handle_input (ACE_HANDLE)
{
  return this->handle_logging_record () > 0 ? 0 : -1;
}

#if !defined (ACE_HAS_THREADS) 
typedef u_long COUNTER;
#define ACE_LOGGER_SYNCH ACE_NULL_SYNCH 
#else
typedef ACE_Atomic_Op <ACE_Thread_Mutex, u_long> COUNTER;
#define ACE_LOGGER_SYNCH ACE_MT_SYNCH 
#endif /* ACE_HAS_THREADS */

class ACE_Svc_Export ACE_Thr_Server_Logging_Handler : public ACE_Server_Logging_Handler<LOGGING_PEER_STREAM, COUNTER, ACE_LOGGER_SYNCH> 
  // = TITLE
  //    Product object created by a <ACE_Thr_Server_Logging_Acceptor>.  An
  //    <ACE_Thr_Server_Logging_Handler> receives, frames, and processes
  //    logging records.
  //
  // = DESCRIPTION
  //     Each client is handled in its own separate thread.
{
public:
  ACE_Thr_Server_Logging_Handler (ACE_Thread_Manager * = 0);

  virtual int open (void * = 0);
  // Override activation definition in the ACE_Svc_Handler class (will
  // spawn a new thread if we've got threads).

  virtual int svc (void);
  // Process remote logging records. 
};

class ACE_Thr_Server_Logging_Acceptor : public ACE_Strategy_Acceptor<ACE_Thr_Server_Logging_Handler, LOGGING_PEER_ACCEPTOR>
  // = TITLE
  //     This class implements the ACE multi-threaded logging service.
  //
  // = DESCRIPTION
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Strategy_Acceptor>.
{
public:
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

private:
  ACE_Schedule_All_Threaded_Strategy<ACE_Thr_Server_Logging_Handler> scheduling_strategy_;
  // The scheduling strategy is designed for multi-threaded services.
};

int
ACE_Thr_Server_Logging_Acceptor::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Thr_Server_Logging_Acceptor::parse_args");

  this->service_port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_LOG_MSG->open ("Logging Service");

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
ACE_Thr_Server_Logging_Acceptor::init (int argc, 
				       char *argv[])
{
  ACE_TRACE ("ACE_Thr_Server_Logging_Acceptor::init");

  // Use the options hook to parse the command line arguments and set
  // options.
  this->parse_args (argc, argv);

  // Set the acceptor endpoint into listen mode (use the Singleton
  // global Reactor...).
  if (this->open (this->service_addr_, ACE_Service_Config::reactor (),
		  0, 0, 0, 
		  &this->scheduling_strategy_,
		  "Thr Logging Server", "ACE multi-threaded logging service") == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p on port %d\n", 
		       "acceptor::open failed", 
		       this->service_addr_.get_port_number ()), -1);

  // Ignore SIGPIPE so that each <SVC_HANDLER> can handle this on its
  // own.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);
  ACE_UNUSED_ARG (sig);

  ACE_INET_Addr server_addr;

  // Figure out what port we're really bound to.
  if (this->acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_local_addr"), -1);
    
  ACE_DEBUG ((LM_DEBUG, 
	      "starting up Threaded Logging Server at port %d on handle %d\n", 
	     server_addr.get_port_number (),
	     this->acceptor ().get_handle ()));
  return 0;
}

// The following are "Factories" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the
// single-threaded and multi-threaded logging server.

ACE_SVC_FACTORY_DEFINE (ACE_Server_Logging_Acceptor)
ACE_SVC_FACTORY_DEFINE (ACE_Thr_Server_Logging_Acceptor)

// No-op...

ACE_Thr_Server_Logging_Handler::ACE_Thr_Server_Logging_Handler (ACE_Thread_Manager *)
{
}

// Override definition in the ACE_Svc_Handler class (spawn a new
// thread if we're configured with ACE_HAS_THREADS!).

int
ACE_Thr_Server_Logging_Handler::open (void *)
{
  // Shut off non-blocking IO since now we can block in our own
  // thread!
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "disable"), -1);

  ACE_INET_Addr client_addr;

  // Determine the address of the client and display it.
  if (this->peer ().get_remote_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_remote_addr"), -1);

  ACE_OS::strncpy (this->host_name_, client_addr.get_host_name (), MAXHOSTNAMELEN + 1);

  ACE_DEBUG ((LM_DEBUG, "(%t) accepted connection from host %s on fd %d\n",
	      client_addr.get_host_name (), this->peer ().get_handle ()));

  // Spawn a new thread of control to handle logging records with the
  // client.  Note that this implicitly uses the
  // ACE_Service_Config::thr_mgr () to control all the threads.
  if (this->activate (THR_BOUND | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);    
  return 0;
}

// Process remote logging records. 

int
ACE_Thr_Server_Logging_Handler::svc (void)
{
  int result = 0;

  // Loop until the client terminates the connection or an error occurs.

  while ((result = this->handle_input ()) > 0)
    continue;

  return result;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Acceptor<ACE_Server_Logging_Handler<ACE_TLI_STREAM, unsigned long, ACE_NULL_SYNCH>, ACE_TLI_ACCEPTOR>;
template class ACE_Acceptor<ACE_Thr_Server_Logging_Handler, ACE_TLI_ACCEPTOR>;
template class ACE_Accept_Strategy<ACE_Server_Logging_Handler<ACE_TLI_STREAM, unsigned long, ACE_NULL_SYNCH>, ACE_TLI_ACCEPTOR>;
template class ACE_Accept_Strategy<ACE_Thr_Server_Logging_Handler, ACE_TLI_ACCEPTOR>;
template class ACE_Concurrency_Strategy<ACE_Server_Logging_Handler<ACE_TLI_STREAM, unsigned long, ACE_NULL_SYNCH> >;
template class ACE_Concurrency_Strategy<ACE_Thr_Server_Logging_Handler>;
template class ACE_Creation_Strategy<ACE_Server_Logging_Handler<ACE_TLI_STREAM, unsigned long, ACE_NULL_SYNCH> >;
template class ACE_Creation_Strategy<ACE_Thr_Server_Logging_Handler>;
template class ACE_Schedule_All_Reactive_Strategy<ACE_Server_Logging_Handler<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH> >;
template class ACE_Schedule_All_Threaded_Strategy<ACE_Thr_Server_Logging_Handler>;
template class ACE_Scheduling_Strategy<ACE_Server_Logging_Handler<ACE_TLI_STREAM, unsigned long, ACE_NULL_SYNCH> >;
template class ACE_Scheduling_Strategy<ACE_Thr_Server_Logging_Handler>;
template class ACE_Server_Logging_Handler<ACE_TLI_STREAM, ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long>, ACE_MT_SYNCH>;
template class ACE_Server_Logging_Handler<ACE_TLI_STREAM, unsigned long, ACE_NULL_SYNCH>;
template class ACE_Strategy_Acceptor<ACE_Server_Logging_Handler<LOGGING_PEER_STREAM, u_long, ACE_NULL_SYNCH>, LOGGING_PEER_ACCEPTOR>;
template class ACE_Strategy_Acceptor<ACE_Thr_Server_Logging_Handler, LOGGING_PEER_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_TLI_STREAM, ACE_SYNCH>;
#if defined (ACE_HAS_THREADS)
template class ACE_Svc_Handler<ACE_TLI_STREAM, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_THREADS */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
