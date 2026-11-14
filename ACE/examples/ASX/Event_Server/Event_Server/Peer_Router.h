/* -*- C++ -*- */
#ifndef _PEER_ROUTER_H
#define _PEER_ROUTER_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Map_Manager.h"
#include "ace/RW_Thread_Mutex.h"

// Type of search key for CONSUMER_MAP
using ROUTING_KEY = ACE_HANDLE;

// Forward declarations.
class Peer_Router;
class Peer_Router_Context;

/**
 * @class Peer_Handler
 * @brief Receives input from a peer and forwards it to the appropriate router.
 */
class Peer_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  /// Initialization method.
  explicit Peer_Handler (Peer_Router_Context * = nullptr);
  Peer_Handler (Peer_Handler const &) = delete;
  Peer_Handler (Peer_Handler &&) = delete;
  Peer_Handler &operator= (Peer_Handler const &) = delete;
  Peer_Handler &operator= (Peer_Handler &&) = delete;
  /// Open the handler when the acceptor activates it.
  int open (void * = nullptr) override;

  /// Receive input from a peer.
  int handle_input (ACE_HANDLE) override;

  /**
   * Send output to a peer. This implementation blocks if flow control occurs,
   * which is undesirable for production applications. An active object, as in
   * ACE's Gateway application, can avoid blocking here.
   */
  int put (ACE_Message_Block *, ACE_Time_Value *tv = nullptr) override;

protected:
  /// Non-owning router context; reference counting keeps it alive.
  Peer_Router_Context *peer_router_context_ {};
};

/**
 * @class Peer_Router_Context
 * @brief Shares router state and accepts connections from peers.
 *
 * The context serves as an ACE_Acceptor and creates Peer_Handlers when peers
 * connect.
 */
class Peer_Router_Context : public ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  explicit Peer_Router_Context (u_short port);

  /// Remove the peer handler associated with the routing key.
  virtual int unbind_peer (ROUTING_KEY);

  /// Add a peer handler associated with the routing key.
  virtual int bind_peer (ROUTING_KEY, Peer_Handler *);

  /**
   * Send the message block to all peers. A real application would usually
   * select destinations using addressing information in the message block.
   */
  int send_peers (ACE_Message_Block *mb);

  /// Create a peer handler for a new connection.
  int make_svc_handler (Peer_Handler *&sh) override;

  /// @name Router task access
  /// @{

  Peer_Router *peer_router () const;
  void peer_router (Peer_Router *);

  /// @}

  /// Decrement the reference count and delete this context when it reaches zero.
  void release ();

  /// Increment the reference count.
  void duplicate ();

private:
  /// Non-owning pointer to the router that this context accepts connections for.
  Peer_Router *peer_router_ {};

  /// @name Peer map types
  /// @{

  using PEER_MAP = ACE_Map_Manager<ROUTING_KEY, Peer_Handler *, ACE_SYNCH_RW_MUTEX>;
  using PEER_ITERATOR = ACE_Map_Iterator<ROUTING_KEY, Peer_Handler *, ACE_SYNCH_RW_MUTEX>;
  using PEER_ENTRY = ACE_Map_Entry<ROUTING_KEY, Peer_Handler *>;

  /// @}

  /// Non-owning index of active peer handlers.
  PEER_MAP peer_map_;

  /// Number of references held by the router and peer handlers.
  int reference_count_ {};

  /// Private to ensure dynamic allocation.
  ~Peer_Router_Context ();

  /// Friend used to avoid compiler warnings for the private destructor.
  friend class Friend_Of_Peer_Router_Context;

  Peer_Router_Context (Peer_Router_Context const &) = delete;
  Peer_Router_Context (Peer_Router_Context &&) = delete;
  Peer_Router_Context &operator= (Peer_Router_Context const &) = delete;
  Peer_Router_Context &operator= (Peer_Router_Context &&) = delete;
};

/**
 * @class Peer_Router
 * @brief Routes messages between an ACE_Stream and remote peers.
 *
 * Consumer_Router and Supplier_Router specialize the task hooks for their
 * respective application-specific routing behavior.
 */
class Peer_Router : public ACE_Task<ACE_SYNCH>
{
protected:
  /// Initialization method.
  explicit Peer_Router (Peer_Router_Context *prc);

  /// Handle control messages arriving from adjacent modules.
  virtual int control (ACE_Message_Block *);

  /// Return the router's shared peer context.
  Peer_Router_Context *context () const;

  /// Base ACE_Task type.
  using inherited = ACE_Task<ACE_SYNCH>;

private:
  /// Non-owning context shared by the module's reader and writer.
  Peer_Router_Context *peer_router_context_ {};

  Peer_Router (Peer_Router const &) = delete;
  Peer_Router (Peer_Router &&) = delete;
  Peer_Router &operator= (Peer_Router const &) = delete;
  Peer_Router &operator= (Peer_Router &&) = delete;
};

#endif /* _PEER_ROUTER_H */
