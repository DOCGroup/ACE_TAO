/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    IO_Handler.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_IO_HANDLER)
#define _IO_HANDLER

#include "ace/Service_Config.h"
#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"
#include "Consumer_Map.h"
#include "Consumer_Entry.h"
#include "Event.h"

// Forward declaration.
class IO_Handler_Connector;

class IO_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, SYNCH_STRATEGY>
  // = TITLE
  //    IO_Handler contains info about connection state and addressing.
  //
  // = DESCRIPTION
  //     The IO_Handler classes process events sent from the peers to the
  //     gateway.  These classes works as follows:
  //   
  // 1. IO_Handler_Connector creates a number of connections with the set of 
  //    peers specified in a configuration file.  
  //
  // 2. For each peer that connects successfully, IO_Handler_Connector
  //    creates an IO_Handler object.  Each object assigns a unique routing
  //    id to its associated peer.  The Handlers are used by gatewayd
  //    that to receive, route, and forward events from source peer(s)
  //    to destination peer(s).
{
public:
  IO_Handler (Consumer_Map *, 
	      IO_Handler_Connector *, 
	      ACE_Thread_Manager * = 0,
	      int socket_queue_size = 0);

  virtual int open (void * = 0);
  // Initialize and activate a single-threaded IO_Handler (called by
  // ACE_Connector::handle_output()).

  int bind (const ACE_INET_Addr &remote_addr, 
	    const ACE_INET_Addr &local_addr, 
	    CONN_ID);
  // Set the peer's addressing and routing information.

  ACE_INET_Addr &remote_addr (void);
  // Returns the peer's routing address.

  ACE_INET_Addr &local_addr (void);
  // Returns our local address.

  // = Set/get routing id.
  CONN_ID id (void);
  void id (CONN_ID);

  // = Set/get the current state of the IO_Handler.
  enum State
  {
    IDLE = 1,      // Prior to initialization.
    CONNECTING,    // During connection establishment.
    ESTABLISHED,   // IO_Handler is established and active.
    DISCONNECTING, // IO_Handler is in the process of connecting.
    FAILED	   // IO_Handler has failed.
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

  // = Set/get IO_Handler activity status.
  int active (void);
  void active (int);

  // = Set/get direction (necessary for error checking).
  char direction (void);
  void direction (char);

  // = The total number of bytes sent/received on this channel.
  size_t total_bytes (void);
  void total_bytes (size_t bytes);
  // Increment count by <bytes>.

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg);
  // Perform timer-based IO_Handler reconnection.

protected:
  enum
  {
    MAX_RETRY_TIMEOUT = 300 // 5 minutes is the maximum timeout.
  };

  int initialize_connection (void);
  // Perform the first-time initiation of a connection to the peer.

  int reinitiate_connection (void);
  // Reinitiate a connection asynchronously when peers fail.

  void socket_queue_size (void);
  // Set the socket queue size.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::RWE_MASK);
  // Perform IO_Handler termination.

  Consumer_Map *consumer_map_;
  // Pointer to table that maps an event
  // to a Set of IO_Handler *'s for output.

  ACE_INET_Addr remote_addr_;
  // Address of peer.

  ACE_INET_Addr local_addr_;
  // Address of us.

  CONN_ID id_;  
  // The assigned routing ID of this entry.

  size_t total_bytes_;
  // The total number of bytes sent/received on this channel.

  State state_;
  // The current state of the channel.

  IO_Handler_Connector *connector_;
  // Back pointer to IO_Handler_Connector to reestablish broken
  // connections.

  int timeout_;
  // Amount of time to wait between reconnection attempts.

  int max_timeout_;
  // Maximum amount of time to wait between reconnection attempts.

  char direction_;
  // Indicates which direction data flows through the channel ('O' ==
  // output and 'I' == input).

  int socket_queue_size_;
  // Size of the socket queue (0 means "use default").
};

class Supplier_Handler : public IO_Handler
  // = TITLE
  //     Handle reception of Peer events arriving as events.
{
public:
  Supplier_Handler (Consumer_Map *, 
		    IO_Handler_Connector *, 
		    ACE_Thread_Manager * = 0,
		    int socket_queue_size = 0);
  // Constructor sets the consumer map pointer.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Receive and process peer events.

protected:
  virtual int recv (ACE_Message_Block *&);
  // Receive an event from a Supplier.

  int forward (ACE_Message_Block *event);
  // Forward the Event to a Consumer.

  ACE_Message_Block *msg_frag_;
  // Keep track of event fragment to handle non-blocking recv's from
  // Suppliers.
};

class Consumer_Handler : public IO_Handler
  // = TITLE
  //     Handle transmission of events to other Peers using a
  //     single-threaded approach.
{
public:
  Consumer_Handler (Consumer_Map *, 
		    IO_Handler_Connector *, 
		    ACE_Thread_Manager * = 0,
		    int socket_queue_size = 0);

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);
  // Send an event to a Consumer (may be queued if necessary).

protected:
  // = We'll allow up to 16 megabytes to be queued per-output
  // channel.
  enum {QUEUE_SIZE = 1024 * 1024 * 16};

  virtual int handle_input (ACE_HANDLE);
  // Receive and process shutdowns from a Consumer.

  virtual int handle_output (ACE_HANDLE);
  // Finish sending event when flow control conditions abate.

  int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put().

  virtual int send (ACE_Message_Block *);
  // Send an event to a Consumer.
};

#endif /* _IO_HANDLER */
