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
#include "Config_Files.h"
#include "Event.h"

// Forward declaration.
class ACE_Event_Channel;

class Proxy_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
  // = TITLE
  //     Proxy_Handler contains info about connection state and addressing.
  //
  // = DESCRIPTION
  //     The Proxy_Handler classes process events sent to the Event
  //     Channel from Suppliers and forward them to Consumers.
{
public:
  Proxy_Handler (const Proxy_Config_Info &);

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

class Proxy_Handler_Factory : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //     Creates the appropriate type of <Proxy_Handler>
  //
  // = DESCRIPTION
  //     <Proxy_Handler>s can include <Consumer_Proxy>,
  //     <Supplier_Proxy>, <Thr_Consumer_Proxy>, or
  //     <Thr_Supplier_Proxy>).
{
public:
  Proxy_Handler *make_proxy_handler (const Proxy_Config_Info &);
  // Make the appropriate type of <Proxy_Handler>.
};

#endif /* _PROXY_HANDLER */
