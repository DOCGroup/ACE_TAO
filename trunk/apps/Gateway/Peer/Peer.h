/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    gateway
//
// = FILENAME
//    Peer.h
//
// = DESCRIPTION
//    These classes process Supplier/Consumer events sent from the
//    gateway (gatewayd) to its various peers (peerd).  The general
//    collaboration works as follows:
//
//    1. <Peer_Acceptor> creates a listener endpoint and waits
//       passively for gatewayd to connect with it.
//
//    2. When a gatewayd connects, <Peer_Acceptor> creates an
//       <Peer_Handler> object that sends/receives events from
//       gatewayd on that connection.
//  
//    3. The <Peer_Handler> waits for gatewayd to inform it of its
//       supplier ID, which is prepended to all subsequent outgoing
//       events sent from peerd.
//
//    4. Once the supplier ID is set, peerd periodically sends events
//       to gatewayd.  Peerd also receives and "processes" events
//       forwarded to it from gatewayd.  In this program, peerd
//       "processes" the events sent to it by writing them to stdout.
//
//    Note that in the current peerd implementation, one Peer process
//    cannot serve as both a Consumer and Supplier of Events.  This is
//    because the gatewayd establishes a separate connection for
//    Suppliers and Consumers and the peerd only maintains a single
//    <Peer_Handler> object to handle this one connection.  Enhancing
//    this implementation to be both a Consumer and Supplier
//    simultaneously is straightforward, however.  In addition,
//    multiple peerd processes can already work together to play these
//    different roles.
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#if !defined (PEER_H)
#define PEER_H

#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/Message_Block.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"

ACE_SVC_FACTORY_DECLARE (Peer_Acceptor)

class ACE_Svc_Export Peer_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //     Handle Peer events arriving from a Gateway.
public:
  // = Initialization and termination methods.
  Peer_Handler (void);
  // Initialize the peer.

  ~Peer_Handler (void);
  // Shutdown the Peer.

  virtual int open (void * = 0);
  // Initialize the handler when called by
  // <ACE_Acceptor::handle_input>.

  virtual int handle_input (ACE_HANDLE);
  // Receive and process peer events.

  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);
  // Send a event to a gateway (may be queued if necessary due to flow
  // control).

  virtual int handle_output (ACE_HANDLE);
  // Finish sending a event when flow control conditions abate.

  virtual int handle_timeout (const ACE_Time_Value &,
			      const void *arg);
  // Periodically send events via <ACE_Reactor> timer mechanism.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Perform object termination.

protected:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;

  enum 
  { 
    MAX_QUEUE_SIZE = 1024 * 1024 * 16,
    // We'll allow up to 16 megabytes to be queued per-output
    // channel!!!!  This is clearly a policy in search of
    // refinement...

    DEFAULT_TIMEOUT = 60
    // By default, disconnect the <Peer_Handler> every minute.
  };

  virtual int recv (ACE_Message_Block *&);
  // Receive an Peer event from gatewayd.

  virtual int send (ACE_Message_Block *);
  // Send an Peer event to gatewayd, using <nonblk_put>.

  virtual int nonblk_put (ACE_Message_Block *mb);
  // Perform a non-blocking put(), which tries to send an event to the
  // gatewayd, but only if it isn't flow controlled.

  // = Event/state/action handlers.
  int xmit_stdin (void);
  // Receive a event from stdin and send it to the gateway.

  int await_supplier_id (void);
  // Action that receives the route id.

  int await_events (void);
  // Action that receives events.

  int (Peer_Handler::*do_action_)(void);
  // Pointer-to-member-function for the current action to run in this
  // state.  This points to one of the preceding 3 methods.

  ACE_INT32 proxy_id_;
  // Proxy ID of the peer, which is obtained from the gatewayd.  For
  // simplicity, in this implementation we also use the Proxy ID as
  // the Supplier ID.  This might change in future releases.

  ACE_Message_Block *msg_frag_;
  // Keep track of event fragments that arrive in non-blocking recv's
  // from the gatewayd.

  size_t total_bytes_;
  // The total number of bytes sent/received to the gatewayd thus far.
};

class ACE_Svc_Export Peer_Acceptor : public ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>
{
  // = TITLE
  //     A factory class that accept connections from gatewayd and
  //     dynamically creates a new <Peer_Handler> object to
  //     communicate with the gatewayd.
public:
  // = Initialization and termination methods.
  Peer_Acceptor (void);
  // Create the Peer.

  virtual int init (int argc, char *argv[]);
  // Initialize the acceptor.

  virtual int info (char **, size_t) const;
  // Return info about this service.

  virtual int fini (void);
  // Perform termination activities.

  virtual int make_svc_handler (Peer_Handler *&);
  // Factory method that creates a <Peer_Handler> just once.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle various signals (e.g., SIGPIPE, SIGINT, and SIGQUIT).

  void parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

private:
  Peer_Handler *peer_handler_;
  // Pointer to <Peer_Handler> allocated just once.

  ACE_INET_Addr addr_;
  // Our addr.

  typedef ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR> inherited;
};

#endif /* PEER_H */

