/* -*- C++ -*- */
// $Id$

#if !defined (_PEER_ROUTER_H)
#define _PEER_ROUTER_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Map_Manager.h"

// Type of search key for CONSUMER_MAP
typedef ACE_HANDLE ROUTING_KEY;

// Forward declarations.
class Peer_Router;
class Peer_Router_Context;

#if defined (ACE_MT_SAFE)
typedef ACE_RW_Mutex ROUTER_MUTEX;
#else
typedef ACE_Null_Mutex ROUTER_MUTEX;
#endif /* ACE_MT_SAFE */

class Peer_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
  // = TITLE
  //     Receive input from a Peer and forward to the appropriate
  //     <Peer_Router>.
{
public:
  Peer_Handler (Peer_Router_Context * = 0);
  // Initialization method.

  virtual int open (void * = 0);
  // Called by the ACE_Acceptor::handle_input() to activate this
  // object.

  virtual int handle_input (ACE_HANDLE);
  // Receive input from the peer.

  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);  
  // Send output to a peer.

protected:
  Peer_Router_Context *prc_;  
  // Pointer to router context.
};

class Peer_Router_Context : public ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>
  // = TITLE
  //     Defines state and behavior shared between both Tasks in a
  //     Peer_Router Module.  
  //
  // = DESCRIPTION
  //     This class also serves as an Acceptor, which creates
  //     Peer_Handlers when Peers connect.
{
public:
  // = Initialization and termination methods.
  Peer_Router_Context (u_short port);

  virtual int unbind_peer (ROUTING_KEY);
  // Remove the <Peer_Handler *> from the <PEER_MAP> that corresponds
  // to the <ROUTING_KEY>.

  virtual int bind_peer (ROUTING_KEY, Peer_Handler *);
  // Add a <Peer_Handler> to the <PEER_MAP> that's associated with the
  // <ROUTING_KEY>.

  int send_peers (ACE_Message_Block *mb);
  // Send the <ACE_Message_Block> to the peer(s).

  int make_svc_handler (Peer_Handler *&sh);
  // Create a new <Peer_Handler> for each connection.

  // = Set/Get Router Task.
  Peer_Router *peer_router ();
  void peer_router (Peer_Router *);

  void release (void);
  // Decrement the reference count and delete <this> when count == 0;

private:
  Peer_Router *peer_router_;
  // Pointer to the <Peer_Router> that we are accepting for.

  // = Useful typedefs
  typedef ACE_Map_Manager <ROUTING_KEY, Peer_Handler *, ROUTER_MUTEX> PEER_MAP;
  typedef ACE_Map_Iterator<ROUTING_KEY, Peer_Handler *, ROUTER_MUTEX> PEER_ITERATOR;
  typedef ACE_Map_Entry<ROUTING_KEY, Peer_Handler *> PEER_ENTRY;

  PEER_MAP peer_map_;
  // Map used to keep track of active peers.

  int reference_count_;
  // Keep track of when we can delete ourselves.

  ~Peer_Router_Context (void);
  // Private to ensure dynamic allocation.

  friend class Friend_Of_Peer_Router_Context;
  // declare a friend class to avoid compiler warnings because the
  // destructor is private.
};

class Peer_Router : public ACE_Task<ACE_SYNCH>
  // = TITLE
  //     This abstract base class provides mechanisms for routing
  //     messages to/from a ACE_Stream from/to one or more peers (which
  //     are typically running on remote hosts).
  //
  // = DESCRIPTION
  //     A subclass of Peer_Router overrides the open(), close(), and
  //     put() methods in order to specialize the behavior of the router
  //     to meet application-specific requirements.
{
protected:
  Peer_Router (Peer_Router_Context *prc);
  // Initialization method.

  virtual int control (ACE_Message_Block *); 
  // Handle control messages arriving from adjacent Modules.

  Peer_Router_Context *context (void) const;
  // Returns the routing context.

  typedef ACE_Task<ACE_SYNCH> inherited;
  // Helpful typedef.

private:
  Peer_Router_Context *prc_;
  // Reference to the context shared by the writer and reader Tasks in
  // the Consumer and Supplier Modules.

  // = Prevent copies and pass-by-value.
  Peer_Router (const Peer_Router &) {}
  void operator= (const Peer_Router &) {}
};

#endif /* _PEER_ROUTER_H */
