/* -*- C++ -*- */
#ifndef _CONSUMER_ROUTER_H
#define _CONSUMER_ROUTER_H

#include "ace/SOCK_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/UPIPE_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/RW_Thread_Mutex.h"
#include "Peer_Router.h"

/**
 * @class Consumer_Router
 * @brief Routes messages between Consumers and the Event Server ACE_Stream.
 *
 * The Consumer_Router normally sits above the stream and routes messages from
 * downstream to Consumers. Messages normally flow up the stream from
 * Supplier_Routers. If Consumers transmit data to this router, it forwards
 * those messages to Suppliers through the Supplier_Routers.
 *
 * On the reader side, it forwards all messages up the stream. On the writer
 * side, it queues outgoing messages and sends them to Supplier_Routers on a
 * separate thread. This path is expected to be uncommon, so the separate
 * thread avoids taxing the main thread that processes normal messages.
 * ACE_Stream calls the task hooks through base-class pointers.
 */
class Consumer_Router : public Peer_Router
{
public:
  /// Construct a router using the shared peer-router context.
  explicit Consumer_Router (Peer_Router_Context *prc);
  Consumer_Router (Consumer_Router const &) = delete;
  Consumer_Router (Consumer_Router &&) = delete;
  Consumer_Router &operator= (Consumer_Router const &) = delete;
  Consumer_Router &operator= (Consumer_Router &&) = delete;

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

  /// @name Dynamic linking hooks
  /// @{

  int info (ACE_TCHAR **info_string, size_t length) const override;

  /// @}
};

#endif /* _CONSUMER_ROUTER_H */
