// TS_Clerk_Handler.cpp
// @(#)TS_Clerk_Handler.cpp	1.1	10/18/96

#define ACE_BUILD_SVC_DLL
#include "ace/Service_Config.h"
#include "ace/Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Time_Value.h"
#include "ace/Time_Request_Reply.h"
#include "ace/OS.h"
#include "ace/Malloc.h"
#include "TS_Clerk_Handler.h"

// A simple struct containing delta time and a sequence number
struct ACE_Time_Info 
{
  long delta_time_;
  ACE_UINT32 sequence_num_;
};

class ACE_TS_Clerk_Processor;  // forward declaration

class ACE_Svc_Export ACE_TS_Clerk_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //    The Clerk Handler provides the interface that is used by the
  //    Clerk Processor to send time update requests to all the
  //    servers. It obtains these updates from the servers and passes
  //    the updates to the Clerk Processor
  //
  // = DESCRIPTION
  //   	The Clerk Processor uses send_request() to send a request for
  //    time update to a server. The Clerk Handler internally computes
  //    the round trip delay for the reply to come back. Once it gets
  //    the reply back from the server (handle_input), it adjusts the
  //    system time using the round trip delay estimate and then
  //    passes the delta time by reference back to the Clerk Processor.
{
public:
  ACE_TS_Clerk_Handler (ACE_TS_Clerk_Processor *processor, 
			ACE_INET_Addr &addr);
  // Default constructor.

  // = Set/get the current state
  enum State
  {
    IDLE = 1,      // Prior to initialization.
    CONNECTING,    // During connection establishment.
    ESTABLISHED,   // Connection is established and active.
    DISCONNECTING, // In the process of disconnecting.
    FAILED	   // Connection has failed.
  };

  // = Set/get the current state.
  State state (void);
  void state (State);

  // = Set/get the current retry timeout delay.
  int timeout (void);
  void timeout (int);

  // = Set/get the maximum retry timeout delay.
  int max_timeout (void);
  void max_timeout (int);

  virtual int open (void * = 0);
  // Activate this instance of the <ACE_TS_Clerk_Handler>
  // (called by the <ACE_TS_Clerk_Processor>).

  virtual ACE_HANDLE get_handle (void) const;
  // Return the handle of the message_fifo_;

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::RWE_MASK);
  // Called when object is removed from the ACE_Reactor
  
  virtual int handle_input (ACE_HANDLE);
  // Receive time update from a server.

  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);
  // Restart connection asynchronously when timeout occurs.

  void remote_addr (ACE_INET_Addr &addr);
  ACE_INET_Addr &remote_addr (void);
  // Get/Set remote addr

  int send_request (ACE_UINT32 sequence_num, ACE_Time_Info &time_info);
  // Send request for time update to the server as well as return the
  // current time info by reference.

protected:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle SIGINT.

  static void stderr_output (int = 0);
  
  enum
    {
      MAX_RETRY_TIMEOUT = 300 // 5 minutes is the maximum timeout.
    };
private:
  int recv_reply (ACE_Time_Request &reply);
  // Receive a reply from a server containing time update

  int reinitiate_connection (void);
  // Reinitiate connection with the server

  State state_;
  // The current state of the connection

  int timeout_;
  // Amount of time to wait between reconnection attempts

  int max_timeout_;
  // Maximum amount of time to wait between reconnection attempts

  ACE_INET_Addr remote_addr_;
  // Remote Addr used for connecting to the server

  ACE_TS_Clerk_Processor *processor_;
  // Instance of Clerk Processor used for re-establishing connections

  ACE_UINT32 start_time_;
  // Time at which request was sent (used to compute round trip delay)

  ACE_UINT32 cur_sequence_num_;
  // Next sequence number of time request (waiting for this update from
  // the server). 

  ACE_Time_Info time_info_;
  // Record of current delta time and current sequence number
};

class ACE_TS_Clerk_Processor : public ACE_Connector <ACE_TS_Clerk_Handler, ACE_SOCK_CONNECTOR>
  // = TITLE
  //    This class manages all the connections to the servers along
  //    with querying them periodically for time updates.
  // = DESCRIPTION
  //   	The Clerk Processor creates connections to all the servers and
  //    creates an ACE_TS_Clerk_Handler for each connection to handle
  //    the requests and replies. It periodically sends a request for
  //    time update through each of the handlers and uses the replies for
  //    computing a synchronized system time.
{
public:
  ACE_TS_Clerk_Processor ();
  // Default constructor

  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);
  // Query servers for time periodically (timeout value)

  int initiate_connection (ACE_TS_Clerk_Handler *, ACE_Synch_Options &);
  // Set up connections to all servers

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

  void alloc (void);
  // Allocate entry in shared memory for system time

  int update_time ();
  // Update delta_time using times obtained from all servers

  typedef ACE_Malloc <ACE_MMAP_Memory_Pool, ACE_Null_Mutex> MALLOC;
  typedef ACE_Allocator_Adapter<MALLOC> ALLOCATOR;
  ALLOCATOR *shmem_;
  // Allocator (used for reading/writing system time from/to shared memory)

  typedef ACE_Unbounded_Set <ACE_TS_Clerk_Handler *> HANDLER_SET;
  typedef ACE_Unbounded_Set_Iterator <ACE_TS_Clerk_Handler *> HANDLER_SET_ITERATOR;  
  HANDLER_SET handler_set_;  
  // Set of TS_Clerk_Handlers and iterator over the set.

  struct System_Time
    {
      long *delta_time_;       // Difference between system time and local time
      long *last_local_time_;  // Last local time
    };

  System_Time system_time_;
  // Clerk system time containing pointers to entries in shared memory

  int timer_id_;
  // Timer id returned by Reactor

  int timeout_;
  // Time period for updating system time

  const char *poolname_;
  // Pool name for backing store

  int blocking_semantics_;
  // Do a blocking/non-blocking connect 

  ACE_UINT32 cur_sequence_num_;
  // Sequence number of next expected update from servers
};


ACE_TS_Clerk_Handler::ACE_TS_Clerk_Handler (ACE_TS_Clerk_Processor *processor,
					    ACE_INET_Addr &addr)
: processor_ (processor),
  remote_addr_ (addr),
  state_ (ACE_TS_Clerk_Handler::IDLE),
  timeout_ (ACE_DEFAULT_TIMEOUT),
  max_timeout_ (ACE_TS_Clerk_Handler::MAX_RETRY_TIMEOUT)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::ACE_TS_Clerk_Handler");
  this->time_info_.delta_time_ = 0;
  this->time_info_.sequence_num_ = 0;
}

// This is called when a <send> to a server fails...
int
ACE_TS_Clerk_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::handle_signal");
  return 0;
}

// Set the connection state
void
ACE_TS_Clerk_Handler::state (ACE_TS_Clerk_Handler::State state)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::state");
  this->state_ = state;
}

// Get the connection state
ACE_TS_Clerk_Handler::State
ACE_TS_Clerk_Handler::state (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::state");
  return this->state_;
}

// Sets the timeout delay.
void
ACE_TS_Clerk_Handler::timeout (int to)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::timeout");
  if (to > this->max_timeout_)
    to = this->max_timeout_;

  this->timeout_ = to;
}

// Recalculate the current retry timeout delay using exponential
// backoff.  Returns the original timeout (i.e., before the
// recalculation).
int
ACE_TS_Clerk_Handler::timeout (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::timeout");
  int old_timeout = this->timeout_;
  this->timeout_ *= 2;
  
  if (this->timeout_ > this->max_timeout_)
    this->timeout_ = this->max_timeout_;

  return old_timeout;
}

// Set the max timeout delay.
void
ACE_TS_Clerk_Handler::max_timeout (int mto)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::max_timeout");
  this->max_timeout_ = mto;
}

// Gets the max timeout delay.
int
ACE_TS_Clerk_Handler::max_timeout (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::max_timeout");
  return this->max_timeout_;
}

int  
ACE_TS_Clerk_Handler::open (void *)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::open");
  ACE_INET_Addr server_addr;

  // Set connection state as established
  this->state (ACE_TS_Clerk_Handler::ESTABLISHED);

  // Register ourselves to receive SIGPIPE so we can attempt
  // reconnections.
#if !defined (ACE_WIN32)
  if (ACE_Service_Config::reactor ()->register_handler (SIGPIPE, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n", 
		       "register_handler (SIGPIPE)"), -1);
#endif

  // Register ourselves with the reactor to receive input
  if (ACE_Service_Config::reactor ()->register_handler (this->get_handle (), 
							this,
							ACE_Event_Handler::READ_MASK | 
							ACE_Event_Handler::EXCEPT_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%n: %p\n", "register_handler (this)"));

  // Figure out what remote port we're really bound to.
  else if (this->peer ().get_remote_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_remote_addr"), -1);
  
  ACE_DEBUG ((LM_DEBUG, 
	      "TS Clerk Daemon connected to port %d on handle %d\n",
	      server_addr.get_port_number (),
	      this->peer ().get_handle ()));

  return 0;
}

ACE_HANDLE 
ACE_TS_Clerk_Handler::get_handle (void) const
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::get_handle");
  return this->peer().get_handle ();
}

int 
ACE_TS_Clerk_Handler::handle_close (ACE_HANDLE,
				    ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::handle_close");
  ACE_DEBUG ((LM_DEBUG, "(%t) shutting down on handle %d\n", this->get_handle ()));
  return this->reinitiate_connection ();
}

int
ACE_TS_Clerk_Handler::reinitiate_connection (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::reinitiate_connection");
  // Skip over deactivated descriptors.

  // Set state to connecting so that we don't try to send anything
  // using this handler 
  this->state (ACE_TS_Clerk_Handler::CONNECTING);
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) Scheduling reinitiation of connection\n"));

      // Reschedule ourselves to try and connect again.
      if (ACE_Service_Config::reactor ()->schedule_timer (this, 0, 
							  this->timeout ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "schedule_timer"), -1);
    }
  return 0;
}

// Receive a time update from a server
int
ACE_TS_Clerk_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::handle_input");
  // We're getting a time update message from a server
  ACE_Time_Request reply;
  if (this->recv_reply (reply) != 0)
    return -1;
  else
    {
      // Get current local time
      ACE_UINT32 local_time = ACE_OS::time (0);

      // Compure delta time (difference between current local time and
      // system time obtained from the server)
      long t = reply.time () - local_time;

      // Compute round trip delay and adjust time accordingly
      ACE_UINT32 one_way_time = (local_time - this->start_time_)/2;
      t += one_way_time;

      // Now update time info (to be retrieved by Clerk_Processor)
      this->time_info_.delta_time_ = t;
      this->time_info_.sequence_num_ = this->cur_sequence_num_;
    }
  return 0;
}

// Restart connection asynchronously when timeout occurs.
int
ACE_TS_Clerk_Handler::handle_timeout (const ACE_Time_Value &tv,
				      const void *arg)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::handle_timeout");
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) attempting to reconnect to server with timeout = %d\n", 
	      this->timeout_));

  // Close down peer to reclaim descriptor if need be. Note this is
  // necessary to reconnect. 
  this->peer ().close ();

  return this->processor_->initiate_connection (this, ACE_Synch_Options::asynch);
}

void
ACE_TS_Clerk_Handler::remote_addr (ACE_INET_Addr &addr)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::remote_addr");
  this->remote_addr_ = addr;
}

ACE_INET_Addr &
ACE_TS_Clerk_Handler::remote_addr (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::remote_addr");
  return this->remote_addr_;
}

int
ACE_TS_Clerk_Handler::recv_reply (ACE_Time_Request &reply)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::recv_reply");
  const int bytes_expected = reply.size ();

  // Since Time_Request messages are fixed size, read the entire
  // message in one go.
  ssize_t n = this->peer ().recv ((void *) &reply, bytes_expected);

  if (n != bytes_expected)
    {
      switch (n)
	{
	case -1:
	  // FALLTHROUGH 
	  ACE_DEBUG ((LM_DEBUG, "****************** recv_reply returned -1\n"));
	default:
	  ACE_ERROR ((LM_ERROR, "%p got %d bytes, expected %d bytes\n",
		      "recv failed", n, bytes_expected));
	  // FALLTHROUGH 
	case 0:
	  // We've shutdown unexpectedly
	  return -1;
	  // NOTREACHED
	}
    }
  else if (reply.decode () == -1)       // Decode the request into host byte order.
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "decode failed"), -1);
  return 0;  
}


int
ACE_TS_Clerk_Handler::send_request (ACE_UINT32 sequence_num, ACE_Time_Info &time_info)
{
  ACE_TRACE ("ACE_TS_Clerk_Handler::send_request");
  void    *buffer;
  ssize_t length;

  // Update current sequence number
  this->cur_sequence_num_ = sequence_num;
  
  // First update the current time info.
  time_info.delta_time_ = this->time_info_.delta_time_;
  time_info.sequence_num_ = this->time_info_.sequence_num_;

  // Now prepare a new time update request
  ACE_Time_Request request (ACE_Time_Request::TIME_UPDATE, 0, 0);
 
  if ((length = request.encode (buffer)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "encode failed"), -1);
 
  // Compute start time of sending request (needed to compute
  // roundtrip delay)
  this->start_time_ = ACE_OS::time (0);

  // Send the request
  if (this->peer ().send_n (buffer, length) != length)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send_n failed"), -1);

  return 0;      
}

ACE_TS_Clerk_Processor::ACE_TS_Clerk_Processor ()
: timeout_ (ACE_DEFAULT_TIMEOUT),
  poolname_ (ACE_DEFAULT_BACKING_STORE),
  blocking_semantics_ (0),
  cur_sequence_num_ (0)
{
}

void
ACE_TS_Clerk_Processor::alloc (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::alloc");
  ACE_NEW (this->shmem_, ALLOCATOR (this->poolname_));

  void *temp = 0;
  // Only create the state if it doesn't already exist.
  if (this->shmem_->find (ACE_DEFAULT_TIME_SERVER_STR, temp) ==  -1)
    {
      // Allocate the space out of shared memory for the system time entry
      temp = this->shmem_->malloc (sizeof (this->system_time_));
      
      // Give it a name binding
      this->shmem_->bind (ACE_DEFAULT_TIME_SERVER_STR, temp);

      // Set up pointers. Note that we add one to get to the second
      // field in the structure
      this->system_time_.delta_time_ = (long *) temp;
      this->system_time_.last_local_time_ = ((long *) temp) + 1;

      // Initialize
      *(this->system_time_.delta_time_) = 0;
      *(this->system_time_.last_local_time_) = ACE_OS::time (0);
    }
}

// Query the servers for the latest time
int
ACE_TS_Clerk_Processor::handle_timeout (const ACE_Time_Value &tv,
					const void *arg)
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::handle_timeout");
  return this->update_time ();
}

int 
ACE_TS_Clerk_Processor::update_time ()
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::update_time");
  ACE_UINT32 expected_sequence_num = this->cur_sequence_num_;

  // Increment sequence number
  this->cur_sequence_num_++;

  int count = 0;
  long total_delta = 0;
  ACE_Time_Info time_info;

  //  Call send_request() on all handlers
  ACE_TS_Clerk_Handler **handler = 0;
  
  for (HANDLER_SET_ITERATOR set_iterator (this->handler_set_);
       set_iterator.next (handler) != 0;
       set_iterator.advance ())
    {      
      if ((*handler)->state () == ACE_TS_Clerk_Handler::ESTABLISHED)
	{
	  if ((*handler)->send_request (this->cur_sequence_num_, time_info) == -1)
	    return -1;
	  // Check if sequence numbers match; otherwise discard
	  else if (expected_sequence_num != 0 &&
		   time_info.sequence_num_ == expected_sequence_num)
	    {
	      count++;
	      ACE_DEBUG ((LM_DEBUG, "[%d] Delta time: %d\n", count, time_info.delta_time_));

	      // #### Can check here if delta value falls within a threshold ####
	      total_delta += time_info.delta_time_;
	    }
	}
    }
  // Update system_time_ using average of times obtained from all the servers.
  // Note that we are keeping two things in shared memory: the delta
  // time (difference between our system clock and the local clock),
  // and the last local time
  if (count > 0) 
    { 
      // At least one server is out there
      *(this->system_time_.delta_time_) = total_delta/count;
    }
  else
    {
      // No servers are out there (or this is the first time around
      // computing the time) so set delta time to zero. This
      // would mean that clients would use the actual local system time.
    *(this->system_time_.delta_time_) = 0;
  }
  // Update the last local time
  *(this->system_time_.last_local_time_) = ACE_OS::time (0);

  ACE_DEBUG ((LM_DEBUG, "Average delta time: %d\n", *(this->system_time_.delta_time_)));
  return 0;
}


int 
ACE_TS_Clerk_Processor::fini (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::fini");

  // Cancel the timer
  if (this->timer_id_ != -1)
    ACE_Service_Config::reactor ()->cancel_timer (this->timer_id_);

  // Destroy all the handlers
  ACE_TS_Clerk_Handler **handler = 0;

  for (HANDLER_SET_ITERATOR set_iterator (this->handler_set_);
       set_iterator.next (handler) != 0;
       set_iterator.advance ())
    {      
      if ((*handler)->state () != ACE_TS_Clerk_Handler::IDLE)
	// Mark state as DISCONNECTING so we don't try to reconnect...
	(*handler)->state (ACE_TS_Clerk_Handler::DISCONNECTING);

      // Deallocate resources.
      (*handler)->destroy (); // Will trigger a delete
    }

  // Remove the backing store
  this->shmem_->remove ();
  return 0;
}

int 
ACE_TS_Clerk_Processor::info (char **strp, size_t length) const
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::info");
  return 0;
}

int  
ACE_TS_Clerk_Processor::init (int argc, char *argv[])
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::init");
  // Use the options hook to parse the command line arguments and set
  // options.
  this->parse_args (argc, argv);

  this->alloc ();

#if !defined (ACE_WIN32)
  // Ignore SIPPIPE so each Output_Channel can handle it.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);

//  ACE_Sig_Set sig_set;
//  sig_set.sig_add (SIGINT);

  // Register ourselves to receive SIGINT and SIGPIPE
  // so we can shut down gracefully via signals.
  if (ACE_Service_Config::reactor ()->register_handler (SIGINT,
							this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n", 
		       "register_handler"), -1);
#endif
  ACE_Synch_Options &synch_options = this->blocking_semantics_ == 0 
    ? ACE_Synch_Options::asynch : ACE_Synch_Options::synch;

  // Now set up connections to all servers
  ACE_TS_Clerk_Handler **handler = 0;
  
  for (HANDLER_SET_ITERATOR set_iterator (this->handler_set_);
       set_iterator.next (handler) != 0;
       set_iterator.advance ())
    {      
      this->initiate_connection (*handler, synch_options);
    }
  // Now set up timer to receive updates from server
  // set the timer to go off after timeout value
  this->timer_id_ = ACE_Service_Config::reactor ()->schedule_timer (this, 
								    NULL, 
								    ACE_Time_Value (this->timeout_),
								    ACE_Time_Value (this->timeout_));
  return 0;
}

int
ACE_TS_Clerk_Processor::initiate_connection (ACE_TS_Clerk_Handler *handler,
					     ACE_Synch_Options &synch_options)
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::initiate_connection");
  char buf[MAXHOSTNAMELEN];

  // Mark ourselves as idle so that the various iterators 
  // will ignore us until we are connected/reconnected.
  handler->state (ACE_TS_Clerk_Handler::IDLE);

  if (handler->remote_addr ().addr_to_string (buf, sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      "can't obtain peer's address"), -1);

  // Establish connection with the server.
  if (this->connect (handler,
		     handler->remote_addr (),
		     synch_options) == -1)
    {
      if (errno != EWOULDBLOCK)
	{
	  handler->state (ACE_TS_Clerk_Handler::FAILED);
	  ACE_DEBUG ((LM_DEBUG, "(%t) %p on address %s\n", "connect", buf));

	  // Reschedule ourselves to try and connect again.
	  if (synch_options[ACE_Synch_Options::USE_REACTOR])
	    {
	      if (ACE_Service_Config::reactor ()->schedule_timer (handler, 
								  0, 
								  handler->timeout ()) == 0)
		ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "schedule_timer"), -1);
	    }
	  else 
	    // Failures on synchronous connects are reported as errors
	    // so that the caller can decide how to proceed.
	    return -1;
	}
      else
	{
	  handler->state (ACE_TS_Clerk_Handler::CONNECTING);
	  ACE_DEBUG ((LM_DEBUG, 
		     "(%t) in the process of connecting %s to %s\n",
		      synch_options[ACE_Synch_Options::USE_REACTOR] 
		      ? "asynchronously" : "synchronously", buf));
	}
    }
  else 
    {
      handler->state (ACE_TS_Clerk_Handler::ESTABLISHED);
      ACE_DEBUG ((LM_DEBUG, "(%t) connected to %s on %d\n", 
		 buf, handler->get_handle ()));
    }
  return 0;
}

int
ACE_TS_Clerk_Processor::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::parse_args");
  ACE_INET_Addr server_addr;
  ACE_TS_Clerk_Handler *handler;
  char server_host[BUFSIZ];

  // Create a default entry
  ACE_OS::sprintf (server_host, "%s:%d",
		   ACE_DEFAULT_SERVER_HOST, 
		   ACE_DEFAULT_LOGGING_SERVER_PORT);

  ACE_Get_Opt get_opt (argc, argv, "h:t:p:b", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'h':
	  // Get the hostname:port and create an ADDR
	  server_addr.set (get_opt.optarg);

	  // Create a new handler
	  ACE_NEW_RETURN (handler,
			  ACE_TS_Clerk_Handler (this, server_addr),
			  -1);
	  
	  // Cache the handler
	  this->handler_set_.insert (handler);
	  break;
	case 't':
	  // Get the timeout value
	  this->timeout_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'p':
	  // Get the poolname
	  this->poolname_ = get_opt.optarg;
	  break;
	case 'b':
	  // Blocking semantics
	  this->blocking_semantics_ = 1;
	  break;
	default:
	  ACE_ERROR_RETURN ((LM_ERROR, 
			     "%n:\n[-p hostname:port] [-t timeout] [-p poolname]\n%a", 1),
			    -1);
	  break;
	}
    }
  return 0;
}

int  
ACE_TS_Clerk_Processor::suspend (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::suspend");
  return 0;
}

int  
ACE_TS_Clerk_Processor::resume (void)
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::resume");
  return 0;
}

// Signal the server to shutdown gracefully.

int
ACE_TS_Clerk_Processor::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_TS_Clerk_Processor::handle_signal");
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the TS_Clerk.

ACE_SVC_FACTORY_DEFINE (ACE_TS_Clerk_Processor)

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Connector<ACE_TS_Clerk_Handler, ACE_SOCK_Connector, ACE_INET_Addr>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
