/* -*- C++ -*- */
// $Id$

#if !defined (PEER)
#define PEER

#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/Message_Block.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"

ACE_SVC_FACTORY_DECLARE (Peer_Acceptor)

// Handle Peer events arriving as events.

class ACE_Svc_Export Peer_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  // = Initialization and termination methods.
  Peer_Handler (void);
  // Initialize the peer.

  ~Peer_Handler (void);
  // Shutdown the Peer.

  virtual int open (void * = 0);
  // Initialize the handler (called by ACE_Acceptor::handle_input())

  virtual int handle_input (ACE_HANDLE);
  // Receive and process peer events.

  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);
  // Send a event to a gateway (may be queued if necessary).

  virtual int handle_output (ACE_HANDLE);
  // Finish sending a event when flow control conditions abate.

  virtual int handle_timeout (const ACE_Time_Value &,
			      const void *arg);
  // Periodically send events via ACE_Reactor timer mechanism.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Perform object termination.

protected:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;

  // We'll allow up to 16 megabytes to be queued per-output
  // channel!!!!  This is clearly a policy in search of refinement...
  enum { MAX_QUEUE_SIZE = 1024 * 1024 * 16 };

  ACE_INT32 proxy_id_;
  // Proxy ID of the peer (obtained from gatewayd).  For simplicity,
  // in this implementation we also use the Proxy ID as the Supplier
  // ID.  This might change in future releases.

  virtual int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put().

  virtual int recv (ACE_Message_Block *&);
  // Receive an Peer event from gatewayd.

  virtual int send (ACE_Message_Block *);
  // Send an Peer event to gatewayd.

  int xmit_stdin (void);
  // Receive a event from stdin and send it to the gateway.

  int (Peer_Handler::*do_action_) (void);
  // Pointer-to-member-function for the current action to run in this state.

  int await_supplier_id (void);
  // Action that receives the route id.

  int await_events (void);
  // Action that receives events.

  ACE_Message_Block *msg_frag_;
  // Keep track of event fragment to handle non-blocking recv's from gateway.

  size_t total_bytes_;
  // The total number of bytes sent/received to the gateway.
};

#endif /* PEER */

