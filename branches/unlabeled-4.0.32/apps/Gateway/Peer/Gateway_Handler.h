/* -*- C++ -*- */
// @(#)Gateway_Handler.h	1.1	10/18/96


/* These Gateway handler classes process Peer messages sent from the
   communication gateway daemon (gatewayd) to its various peers, e.g.,
   CF and ETS, (represented collectively in this prototype as peerd).
   .  These classes works as follows:
   
   1. Gateway_Acceptor creates a listener endpoint and waits passively
      for gatewayd to connect with it.

   2. When gatewayd connects, Gateway_Acceptor creates an
      Gateway_Handler object that sends/receives messages from
      gatewayd.

   3. Gateway_Handler waits for gatewayd to inform it of its routing
      ID, which is prepended to all outgoing messages send from peerd.

   4. Once the routing ID is set, peerd periodically sends messages to
      gatewayd.  Peerd also receives and "processes" messages
      forwarded to it from gatewayd.  In this program, peerd
      "processes" messages by writing them to stdout. */

#if !defined (GATEWAY_HANDLER)
#define GATEWAY_HANDLER

#include "ace/Service_Config.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Map_Manager.h"
#include "Peer_Message.h"

// Forward declaration.
class Gateway_Handler;

// Maps a ACE_HANDLE onto a Gateway_Handler *.
typedef ACE_Map_Manager <ACE_HANDLE, Gateway_Handler *, ACE_Null_Mutex> HANDLER_MAP;
typedef ACE_Map_Iterator<ACE_HANDLE, Gateway_Handler *, ACE_Null_Mutex> HANDLER_ITERATOR;
typedef ACE_Map_Entry <ACE_HANDLE, Gateway_Handler *> MAP_ENTRY;

// Handle Peer messages arriving as events. 

class Gateway_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  Gateway_Handler (ACE_Thread_Manager * = 0);

  virtual int open (void * = 0);
  // Initialize the handler (called by ACE_Acceptor::handle_input())

  virtual int handle_input (ACE_HANDLE);
  // Receive and process peer messages.

  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);
  // Send a message to a gateway (may be queued if necessary).

  virtual int handle_output (ACE_HANDLE);
  // Finish sending a message when flow control conditions abate.

  virtual int handle_timeout (const ACE_Time_Value &, 
			      const void *arg);
  // Periodically send messages via ACE_Reactor timer mechanism.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE, 
			    ACE_Reactor_Mask = ACE_Event_Handler::RWE_MASK);
  // Perform object termination.

  void map (HANDLER_MAP *);
  // Cache a binding to the HANDLER_MAP.

protected:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;

  // We'll allow up to 16 megabytes to be queued per-output
  // channel!!!!  This is clearly a policy in search of refinement...
  enum { QUEUE_SIZE = 1024 * 1024 * 16 };

  int handle_signal (int signum, siginfo_t *, ucontext_t *);

  Peer_Header::ROUTING_ID routing_id_;
  // Routing ID of the peer (obtained from gatewayd).

  virtual int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put().

  virtual int recv_peer (ACE_Message_Block *&);
  // Receive an Peer message from gatewayd.

  virtual int send_peer (ACE_Message_Block *);
  // Send an Peer message to gatewayd.

  int xmit_stdin (void);
  // Receive a message from stdin and send it to the gateway.

  int (Gateway_Handler::*do_action_) (void);
  // Pointer-to-member-function for the current action to run in this state.

  int await_route_id (void);
  // Action that receives the route id.  

  int await_messages (void);
  // Action that receives messages.

  ACE_Message_Block *msg_frag_;
  // Keep track of message fragment to handle non-blocking recv's from gateway.

  size_t total_bytes_;
  // The total number of bytes sent/received to the gateway.

  HANDLER_MAP *map_;
  // Maps the ACE_HANDLE onto the Gateway_Handler *.
};

// A factory class that accept connections from gatewayd and
// dynamically creates a new Gateway_Handler object to do the dirty work.

class Gateway_Acceptor : public ACE_Acceptor<Gateway_Handler, ACE_SOCK_ACCEPTOR>
{
public:  
  // = Initialization methods, called when dynamically linked.
  Gateway_Acceptor (Gateway_Handler *handler);
  virtual int init (int argc, char *argv[]);
  // Initialize the acceptor.

  virtual int info (char **, size_t) const;
  // Return info about this service.

  virtual int fini (void);
  // Perform termination.

  virtual Gateway_Handler *make_svc_handler (void);
  // Factory method that creates the Gateway_Handler once.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle various signals (e.g., SIGPIPE)

  HANDLER_MAP map_;
  // Maps the ACE_HANDLE onto the Gateway_Handler *.
  
  Gateway_Handler *gateway_handler_;
  // Pointer to memory allocated exactly once.

  typedef ACE_Acceptor<Gateway_Handler, ACE_SOCK_ACCEPTOR> inherited;
};

// Factory function that allocates a new Peer daemon.
extern "C" ACE_Service_Object *_alloc_peerd (void);

#endif /* GATEWAY_HANDLER */

