/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    TS_Clerk_Handler.h
//
// = AUTHOR
//    Prashant Jain 
// 
// ============================================================================

#if !defined (ACE_TS_CLERK_HANDLER_H)
#define ACE_TS_CLERK_HANDLER_H

#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Malloc.h"

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
  ACE_TS_Clerk_Handler (ACE_TS_Clerk_Processor *processor = 0,
			ACE_INET_Addr &addr = (ACE_INET_Addr &) ACE_Addr::sap_any);
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
			    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
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
  // Handle SIGPIPE.

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
  ACE_TS_Clerk_Processor (void);
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
  
private:
  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

  void alloc (void);
  // Allocate entry in shared memory for system time

  int update_time ();
  // Update delta_time using times obtained from all servers

  typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> MALLOC;
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

  LPCTSTR poolname_;
  // Pool name for backing store

  int blocking_semantics_;
  // Do a blocking/non-blocking connect 

  ACE_UINT32 cur_sequence_num_;
  // Sequence number of next expected update from servers
};

ACE_SVC_FACTORY_DECLARE (ACE_TS_Clerk_Processor)

#endif /* ACE_TS_CLERK_HANDLER_H */
