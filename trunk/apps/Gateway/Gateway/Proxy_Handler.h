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
#include "Consumer_Dispatch_Set.h"
#include "Event.h"

// Forward declaration.
class Proxy_Handler_Connector;
class ACE_Event_Channel;

class Proxy_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, SYNCH_STRATEGY>
  // = TITLE
  //    Proxy_Handler contains info about connection state and addressing.
  //
  // = DESCRIPTION
  //     The Proxy_Handler classes process events sent to the Event
  //     Channel from Suppliers and forward them to Consumers.
{
public:
  Proxy_Handler (ACE_Event_Channel &, 
		 const ACE_INET_Addr &remote_addr,
		 const ACE_INET_Addr &local_addr,
		 ACE_INT32 conn_id);

  virtual int open (void * = 0);
  // Initialize and activate a single-threaded Proxy_Handler (called by
  // ACE_Connector::handle_output()).

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

  // = Set/get proxy role (i.e., 'S' for Supplier and 'C' for Consumer
  // (necessary for error checking).
  void proxy_role (char);
  char proxy_role (void);

  // = The total number of bytes sent/received on this proxy.
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

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Perform Proxy_Handler termination.

  ACE_INET_Addr remote_addr_;
  // Address of peer.

  ACE_INET_Addr local_addr_;
  // Address of us.

  ACE_INT32 id_;  
  // The assigned routing ID of this entry.

  size_t total_bytes_;
  // The total number of bytes sent/received on this proxy.

  State state_;
  // The current state of the proxy.

  int timeout_;
  // Amount of time to wait between reconnection attempts.

  int max_timeout_;
  // Maximum amount of time to wait between reconnection attempts.

  char proxy_role_;
  // Indicates which role the proxy plays ('S' == Supplier and 'C' ==
  // Consumer).

  ACE_Event_Channel &event_channel_;
  // Reference to the <ACE_Event_Channel> that we use to forward all
  // the events from Consumers and Suppliers.
};

class Supplier_Proxy : public Proxy_Handler
  // = TITLE
  //     Handles reception of Events from Suppliers
  //
  // = DESCRIPTION
  //     Performs framing and error checking.
{
public:
  // = Initialization method.
  Supplier_Proxy (ACE_Event_Channel &, 
		  const ACE_INET_Addr &remote_addr,
		  const ACE_INET_Addr &local_addr,
		  ACE_INT32 conn_id);

protected:
  // = All the following methods are upcalls, so they can be protected.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Receive and process peer events.

  virtual int recv (ACE_Message_Block *&);
  // Receive an event from a Supplier.

  int forward (ACE_Message_Block *event);
  // Forward the <event> to its appropriate Consumer.  This delegates
  // to the <ACE_Event_Channel> to do the actual forwarding.

  ACE_Message_Block *msg_frag_;
  // Keep track of event fragment to handle non-blocking recv's from
  // Suppliers.
};

class Consumer_Proxy : public Proxy_Handler
  // = TITLE
  //     Handles transmission of events to Consumers.
  //
  // = DESCRIPTION
  //     Performs queueing and error checking.  Uses a single-threaded
  //     Reactive approach to handle flow control.
{
public:
  // = Initialization method.
  Consumer_Proxy (ACE_Event_Channel &, 
		  const ACE_INET_Addr &remote_addr,
		  const ACE_INET_Addr &local_addr,
		  ACE_INT32 conn_id);

  virtual int put (ACE_Message_Block *event, 
		   ACE_Time_Value * = 0);
  // Send an event to a Consumer (may be queued if necessary).

protected:
  // = We'll allow up to 16 megabytes to be queued per-output proxy.
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
