/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Event_Channel.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_EVENT_CHANNEL)
#define ACE_EVENT_CHANNEL

#include "Proxy_Handler_Connector.h"

class ACE_Svc_Export ACE_Event_Channel_Options
  // = TITLE
  //    Maintains the options for an <ACE_Event_Channel>.
{
public:
  ACE_Event_Channel_Options (void);
  // Initialization.

  int performance_window_;
  // Number of seconds after connection establishment to report
  // throughput.
  
  int blocking_semantics_;
  // 0 == blocking connects, ACE_NONBLOCK == non-blocking connects.

  int socket_queue_size_;
  // Size of the socket queue (0 means "use default").
};

class ACE_Svc_Export ACE_Event_Channel : public ACE_Task<SYNCH_STRATEGY>
  // = TITLE
  //    Define a generic Event_Channel.
  //
  // = DESCRIPTION
{
public:
  // = Initialization and termination methods.
  ACE_Event_Channel (void);
  ~ACE_Event_Channel (void);

  virtual int open (void * = 0);
  // Open the channel.

  virtual int close (u_long = 0);
  // Close down the Channel.

  // = Proxy management methods.
  int initiate_proxy_connection (Proxy_Handler *, 
				 ACE_Synch_Options & = ACE_Synch_Options::synch);
  // Initiate the connection of the <Proxy_Handler> to its peer.

  int complete_proxy_connection (Proxy_Handler *);
  // Complete the initialization of the <Proxy_Handler> once it's
  // connected to its Peer.

  int reinitiate_proxy_connection (Proxy_Handler *);
  // Reinitiate a connection asynchronously when the Peer fails.

  int bind_proxy (Proxy_Handler *);
  // Bind the <Proxy_Handler> to the <connection_map_>.

  int find_proxy (ACE_INT32 conn_id, Proxy_Handler *&);
  // Locate the <Proxy_Handler> with <conn_id>.

  int subscribe (const Event_Key &event_addr, 
		 Consumer_Dispatch_Set *cds);
  // Subscribe the <Consumer_Dispatch_Set> to receive events that
  // match <Event_Key>.

  // = Event forwarding method.
  virtual int put (ACE_Message_Block *mb, ACE_Time_Value * = 0);
  // Pass <mb> to the Event Channel so it can forward it to Consumers.

  ACE_Event_Channel_Options &options (void);
  // Points to the Event_Channel options.

  int initiate_connections (void);
  // Initiate connections to the peers.

private:
  virtual int svc (void);
  // Run as an active object.

  int parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

  virtual int handle_timeout (const ACE_Time_Value &, 
			      const void *arg);
  // Perform timer-based performance profiling.

  Proxy_Handler_Connector connector_;
  // Used to establish the connections actively.

  // Proxy_Handler_Acceptor acceptor_;
  // Used to establish the connections passively.

  // = Make life easier by defining typedefs.
  // Note that Proxy_Handler is assumed to the base class of
  // SUPPLIER_PROXY and CONSUMER_PROXY.
  typedef ACE_Map_Manager<ACE_INT32, Proxy_Handler *, MAP_MUTEX> CONNECTION_MAP;
  typedef ACE_Map_Iterator<ACE_INT32, Proxy_Handler *, MAP_MUTEX> CONNECTION_MAP_ITERATOR;
  typedef ACE_Map_Entry<ACE_INT32, Proxy_Handler *> CONNECTION_MAP_ENTRY;

  CONNECTION_MAP connection_map_;
  // Table that maps Connection IDs to Proxy_Handler *'s.

  Event_Forwarding_Discriminator efd_;
  // Map that associates an event to a set of Consumer_Proxy *'s.

  ACE_Event_Channel_Options options_;
  // The options for the channel.
};

#endif /* ACE_EVENT_CHANNEL */
