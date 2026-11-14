/* -*- C++ -*- */
#ifndef _SUPPLIER_ROUTER_H
#define _SUPPLIER_ROUTER_H

#include "ace/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "ace/Map_Manager.h"
#include "ace/Svc_Handler.h"
#include "Peer_Router.h"

/**
 * @class Supplier_Router
 * @brief Routes messages between Suppliers and the Event Server ACE_Stream.
 *
 * The Supplier_Router normally sits below the stream and sends messages from
 * Suppliers upstream to Consumers connected to the Consumer_Router. On the
 * reader side, it forwards all messages up the stream. On the writer side, it
 * queues outgoing messages and sends them on a separate thread. This path is
 * expected to be uncommon, so the separate thread avoids taxing the main
 * thread that processes normal messages. ACE_Stream calls the task hooks
 * through base-class pointers.
 */
class Supplier_Router : public Peer_Router
{
public:
  /// Construct a router using the shared peer-router context.
  explicit Supplier_Router (Peer_Router_Context *prc);
  Supplier_Router (Supplier_Router const &) = delete;
  Supplier_Router (Supplier_Router &&) = delete;
  Supplier_Router &operator= (Supplier_Router const &) = delete;
  Supplier_Router &operator= (Supplier_Router &&) = delete;

protected:
  /// @name ACE_Task hooks
  /// @{

  /// Initialize the router when it is added to the stream.
  int open (void *a = nullptr) override;

  /// Shut down the router when it is removed from the stream.
  int close (u_long flags = 0) override;

  /// Queue a message received from a peer for forwarding up the stream.
  int put (ACE_Message_Block *msg, ACE_Time_Value * = nullptr) override;

  /// Dequeue messages and pass them up the stream.
  int svc () override;

  /// @}

  /// Dynamic linking hook.
  int info (ACE_TCHAR **info_string, size_t length) const override;
};

#endif /* _SUPPLIER_ROUTER_H */
