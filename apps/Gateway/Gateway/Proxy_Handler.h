/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Proxy_Handler.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_PROXY_HANDLER)
#define _PROXY_HANDLER

#include "ace/Service_Config.h"
#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"
#include "Event_Forwarding_Discriminator.h"
#include "Dispatch_Set.h"
#include "Event.h"

// Forward declaration.
class Proxy_Handler_Connector;

class Proxy_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, SYNCH_STRATEGY>
  // = TITLE
  //    Proxy_Handler contains info about connection state and addressing.
  //
  // = DESCRIPTION
  //     The Proxy_Handler classes process events sent to the Event
  //     Channel from Suppliers and forward them to Consumers.
{
public:
  Proxy_Handler (Event_Forwarding_Discriminator *, 
		 Proxy_Handler_Connector *, 
		 ACE_Thread_Manager * = 0,
		 int socket_queue_size = 0);

  virtual int open (void * = 0);
  // Initialize and activate a single-threaded Proxy_Handler (called by
  // ACE_Connector::handle_output()).

  int bind (const ACE_INET_Addr &remote_addr, 
	    const ACE_INET_Addr &local_addr, 
	    ACE_INT32);
  // Set the peer's addressing and routing information.

  ACE_INET_Addr &remote_addr (void);
  // Returns the peer's routing address.

  ACE_INET_Addr &local_addr (void);
  // Returns our local address.

  // = Set/get routing id.
  ACE_INT32 id (void);
  void id (ACE_INT32);

  // = The current state of the Proxy_Handler.
  enum State
  {
    IDLE = 1,      // Prior to initialization.
    CONNECTING,    // During connection establishment.
    ESTABLISHED,   // Proxy_Handler is established and active.
    DISCONNECTING, // Proxy_Handler is in the process of connecting.
    FAILED	   // Proxy_Handler has failed.
  };

  // = Set/get the current state.
  void state (State);
  State state (void);

  // = Set/get the current retry timeout delay.
  void timeout (int);
  int timeout (void);

  // = Set/get the maximum retry timeout delay.
  void max_timeout (int);
  int max_timeout (void);

  // = Set/get direction (i.e., 'S' for Supplier and 'C' for Consumer
  // (necessary for error checking).
  void direction (char);
  char direction (void);

  // = The total number of bytes sent/received on this channel.
  size_t total_bytes (void);
  void total_bytes (size_t bytes);
  // Increment count by <bytes>.

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg);
  // Perform timer-based Proxy_Handler reconnection.

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
  // Perform Proxy_Handler termination.

  Event_Forwarding_Discriminator *efd_;
  // Maps Events to a set of Consumers.

  ACE_INET_Addr remote_addr_;
  // Address of peer.

  ACE_INET_Addr local_addr_;
  // Address of us.

  ACE_INT32 id_;  
  // The assigned routing ID of this entry.

  size_t total_bytes_;
  // The total number of bytes sent/received on this channel.

  State state_;
  // The current state of the channel.

  Proxy_Handler_Connector *connector_;
  // Back pointer to Proxy_Handler_Connector to reestablish broken
  // connections.

  int timeout_;
  // Amount of time to wait between reconnection attempts.

  int max_timeout_;
  // Maximum amount of time to wait between reconnection attempts.

  char direction_;
  // Indicates which direction data flows through the channel ('S' ==
  // Supplier and 'C' == Consumer).

  int socket_queue_size_;
  // Size of the socket queue (0 means "use default").
};

class Supplier_Proxy : public Proxy_Handler
  // = TITLE
  //     Handles reception of Events from Suppliers
  //
  // = DESCRIPTION
  //     Performs framing and error checking.
{
public:
  Supplier_Proxy (Event_Forwarding_Discriminator *, 
		  Proxy_Handler_Connector *, 
		  ACE_Thread_Manager * = 0,
		  int socket_queue_size = 0);
  // Constructor sets the consumer map pointer.

protected:
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Receive and process peer events.

  virtual int recv (ACE_Message_Block *&);
  // Receive an event from a Supplier.

  int forward (ACE_Message_Block *event);
  // Forward the Event to a Consumer.

  ACE_Message_Block *msg_frag_;
  // Keep track of event fragment to handle non-blocking recv's from
  // Suppliers.
};

class Consumer_Proxy : public Proxy_Handler
  // = TITLE
  //     Handles transmission of events to Consumers.
  //
  // = DESCRIPTION
  //     Uses a single-threaded approach.
{
public:
  Consumer_Proxy (Event_Forwarding_Discriminator *, 
		  Proxy_Handler_Connector *, 
		  ACE_Thread_Manager * = 0,
		  int socket_queue_size = 0);

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);
  // Send an event to a Consumer (may be queued if necessary).

protected:
  // = We'll allow up to 16 megabytes to be queued per-output channel.
  enum {MAX_QUEUE_SIZE = 1024 * 1024 * 16};

  virtual int handle_output (ACE_HANDLE);
  // Finish sending event when flow control conditions abate.

  int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put().

  virtual ssize_t send (ACE_Message_Block *);
  // Send an event to a Consumer.

  virtual int handle_input (ACE_HANDLE);
  // Receive and process shutdowns from a Consumer.
};

#endif /* _PROXY_HANDLER */
