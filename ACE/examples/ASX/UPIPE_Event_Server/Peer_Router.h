/* -*- C++ -*- */

#ifndef _PEER_ROUTER_H
#define _PEER_ROUTER_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Addr.h"
#include "ace/Thread_Manager.h"
#include "ace/Map_Manager.h"
#include <memory>

#if defined (ACE_HAS_THREADS)
#include "ace/RW_Mutex.h"

template <class PEER_HANDLER, class KEY>
class Peer_Router;

template <class PEER_HANDLER, class KEY>
class Acceptor_Factory : public ACE_Acceptor<PEER_HANDLER, ACE_UPIPE_ACCEPTOR>
{
public:
  explicit Acceptor_Factory (Peer_Router<PEER_HANDLER, KEY> *pr);
  Acceptor_Factory (Acceptor_Factory const &) = delete;
  Acceptor_Factory (Acceptor_Factory &&) = delete;
  Acceptor_Factory &operator= (Acceptor_Factory const &) = delete;
  Acceptor_Factory &operator= (Acceptor_Factory &&) = delete;

  Peer_Router<PEER_HANDLER, KEY> *router () const;

  /// Initialize the acceptor when it is linked dynamically.
  int init (int argc, ACE_TCHAR *argv[]) override;

private:
  /// Non-owning back-reference; the router owns this factory.
  Peer_Router<PEER_HANDLER, KEY> *pr_ {};
};

/// Receives input from a peer.
template <class ROUTER, class KEY>
class Peer_Handler : public ACE_Svc_Handler<ACE_UPIPE_STREAM, ACE_MT_SYNCH>
{
public:
  explicit Peer_Handler (ACE_Thread_Manager * = nullptr);
  Peer_Handler (Peer_Handler const &) = delete;
  Peer_Handler (Peer_Handler &&) = delete;
  Peer_Handler &operator= (Peer_Handler const &) = delete;
  Peer_Handler &operator= (Peer_Handler &&) = delete;

  /// Called by ACE_Acceptor::handle_input() to activate this object.
  int open (void * = nullptr) override;

  /// Receive input from the peer.
  int handle_input (ACE_HANDLE) override;

  /// Send output to a peer.
  int put (ACE_Message_Block *, ACE_Time_Value *tv = nullptr) override;

protected:
  /// Non-owning pointer to the router task.
  ROUTER *router_task_ {};

private:
  /// Not used by this handler.
  int svc () override;
};

/**
 * @class Peer_Router
 * @brief Routes messages between a stream and one or more peer handlers.
 *
 * This abstract base class provides mechanisms for routing messages
 * to and from an ACE_Stream. A subclass overrides open(), close(), and
 * put() to specialize the router for its application.
 */
template <class PEER_HANDLER, class PEER_KEY>
class Peer_Router : public ACE_Task<ACE_MT_SYNCH>
{
public:
  explicit Peer_Router (ACE_Thread_Manager * = nullptr);
  ~Peer_Router () = default;
  Peer_Router (Peer_Router const &) = delete;
  Peer_Router (Peer_Router &&) = delete;
  Peer_Router &operator= (Peer_Router const &) = delete;
  Peer_Router &operator= (Peer_Router &&) = delete;

  using HANDLER = Peer_Handler<Peer_Router<PEER_HANDLER, PEER_KEY>, PEER_KEY>;

  /// Remove a PEER_HANDLER from the PEER_MAP.
  virtual int unbind_peer (PEER_KEY);

  /// Add a PEER_HANDLER to the PEER_MAP.
  virtual int bind_peer (PEER_KEY, HANDLER *);

  /// Send a message block to the peers.
  int send_peers (ACE_Message_Block *mb);

protected:
  /// Handle control messages arriving from adjacent modules.
  virtual int control (ACE_Message_Block *);

  /// Map of active peers.
  ACE_Map_Manager <PEER_KEY, PEER_HANDLER *, ACE_RW_Mutex> peer_map_;

  /// @name Dynamic linking initialization hooks inherited from ACE_Task
  /// @{

  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;

  /// @}

  /// Factory for accepting new PEER_HANDLERs.
  std::unique_ptr<Acceptor_Factory<PEER_HANDLER, PEER_KEY>> acceptor_ {};
};

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */

#include "Peer_Router.cpp"

#endif /* ACE_HAS_THREADS */
#endif /* _PEER_ROUTER_H */
