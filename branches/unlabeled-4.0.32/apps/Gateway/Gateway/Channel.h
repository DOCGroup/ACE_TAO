/* -*- C++ -*- */
// @(#)Channel.h	1.1	10/18/96


// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Channel.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CHANNEL)
#define _CHANNEL

#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"
#include "Routing_Table.h"
#include "Routing_Entry.h"
#include "Peer_Message.h"

// The following typedefs are used in order to parameterize the
// synchronization policies without changing the source code!

// If we don't have threads then use the single-threaded synchronization.
#if !defined (ACE_HAS_THREADS)
#define SYNCH ACE_NULL_SYNCH
typedef ACE_Null_Mutex MUTEX;
#define CHANNEL_PEER_STREAM ACE_SOCK_STREAM 
#define CHANNEL_PEER_CONNECTOR ACE_SOCK_CONNECTOR 
#else /* ACE_HAS_THREADS */

// Select communication mechanisms.
#if 0 // defined (ACE_HAS_TLI)
// Note that due to inconsistencies between the semantics of sockets
// and TLI with respect to establishing non-blocking connections it's
// not a good idea to use TLI...
#include "ace/TLI_Connector.h"
#define CHANNEL_PEER_STREAM ACE_TLI_STREAM
#define CHANNEL_PEER_CONNECTOR ACE_TLI_CONNECTOR 
#else
#define CHANNEL_PEER_STREAM ACE_SOCK_STREAM 
#define CHANNEL_PEER_CONNECTOR ACE_SOCK_CONNECTOR 
#endif /* 0 */

// Note that we only need to make the ACE_Task thread-safe if we
// are using the multi-threaded Thr_Output_Channel...
#if defined (USE_OUTPUT_MT) 
#define SYNCH ACE_MT_SYNCH
#else
#define SYNCH ACE_NULL_SYNCH
#endif /* USE_OUTPUT_MT || USE_INPUT_MT */

// Note that we only need to make the ACE_Map_Manager thread-safe if
// we are using the multi-threaded Thr_Input_Channel...
#if defined (USE_INPUT_MT)
typedef ACE_RW_Mutex MUTEX;
#else
typedef ACE_Null_Mutex MUTEX;
#endif /* USE_INPUT_MT */
#endif /* ACE_HAS_THREADS */

// Typedef for the routing table.
typedef Routing_Table<Peer_Addr, Routing_Entry, MUTEX>
	ROUTING_TABLE;

// Forward declaration.
class Channel_Connector;

class Channel : public ACE_Svc_Handler<CHANNEL_PEER_STREAM, SYNCH>
  // = TITLE
  //    Channel contains info about connection state and addressing.
  //
  // = DESCRIPTION
  //     The Channel classes process messages sent from the peers to the
  //     gateway.  These classes works as follows:
  //   
  // 1. Channel_Connector creates a number of connections with the set of 
  //    peers specified in a configuration file.  
  //
  // 2. For each peer that connects successfully, Channel_Connector
  //    creates an Channel object.  Each object assigns a unique routing
  //    id to its associated peer.  The Channels are used by gatewayd
  //    that to receive, route, and forward messages from source peer(s)
  //    to destination peer(s).
{
public:
  Channel (ROUTING_TABLE *, 
	   Channel_Connector *, 
	   ACE_Thread_Manager * = 0,
	   int socket_queue_size = 0);

  virtual int open (void * = 0);
  // Initialize and activate a single-threaded Channel (called by
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

  // = Set/get the current state of the Channel.
  enum State
  {
    IDLE = 1,      // Prior to initialization.
    CONNECTING,    // During connection establishment.
    ESTABLISHED,   // Channel is established and active.
    DISCONNECTING, // Channel is in the process of connecting.
    FAILED	   // Channel has failed.
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

  // = Set/get Channel activity status.
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
  // Perform timer-based Channel reconnection.

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
  // Perform Channel termination.

  ROUTING_TABLE *routing_table_;
  // Pointer to table that maps a Peer_Addr
  // to a Set of Channel *'s for output.

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

  Channel_Connector *connector_;
  // Back pointer to Channel_Connector to reestablish broken
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

class Input_Channel : public Channel
  // = TITLE
  //     Handle reception of Peer messages arriving as events.
{
public:
  Input_Channel (ROUTING_TABLE *, 
		 Channel_Connector *, 
		 ACE_Thread_Manager * = 0,
		 int socket_queue_size = 0);
  // Constructor sets the routing table pointer.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Receive and process peer messages.

protected:
  virtual int recv_peer (ACE_Message_Block *&);
  // Receive a message from a peer.

  int route_message (ACE_Message_Block *);
  // Action that receives messages from peerd.

  ACE_Message_Block *msg_frag_;
  // Keep track of message fragment to handle non-blocking recv's from
  // peers.

  virtual int svc (void);
  // This method is not used since we are single-threaded.

private:
  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);  
  // This methods should not be called to handle input.
};

class Output_Channel : public Channel
  // = TITLE
  //     Handle transmission of messages to other Peers using a
  //     single-threaded approach.
{
public:
  Output_Channel (ROUTING_TABLE *, 
		  Channel_Connector *, 
		  ACE_Thread_Manager * = 0,
		  int socket_queue_size = 0);

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);
  // Send a message to a gateway (may be queued if necessary).

protected:
  // = We'll allow up to 16 megabytes to be queued per-output
  // channel.
  enum {QUEUE_SIZE = 1024 * 1024 * 16};

  virtual int handle_input (ACE_HANDLE);
  // Receive and process shutdowns from peer.

  virtual int handle_output (ACE_HANDLE);
  // Finish sending a message when flow control conditions abate.

  int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put().

  virtual int send_peer (ACE_Message_Block *);
  // Send a message to a peer.

  virtual int svc (void);
  // This method is not used since we are single-threaded.
};

#endif /* _CHANNEL */
