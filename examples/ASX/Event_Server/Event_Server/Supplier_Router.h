/* -*- C++ -*- */
// $Id$

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

class Supplier_Router : public Peer_Router
{
  // = TITLE
  //     Provides the interface between one or more Suppliers and the
  //     Event Server ACE_Stream.
  //
  // = DESCRIPTION
  //     This class normally sits on "bottom" of the Stream and sends
  //     all messages coming from Suppliers via its "write" <Task>
  //     "upstream" to all the Consumers connected to the
  //     <Consumer_Router>.  Normally, the messages flow up the
  //     stream to <Consumer_Router>s.  However, if Consumers
  //     transmit data to the <Consumer_Router>, we dutifully push it
  //     out to the Suppliers via the <Supplier_Router>.
  //
  //     When used on the "reader" side of a Stream, the
  //     <Supplier_Router> simply forwards all messages up the stream.
  //     When used on the "writer" side, the <Supplier_Router> queues
  //     up outgoing messages to suppliers and sends them in a
  //     separate thread.  The reason for this is that it's really an
  //     "error" for a <Supplier_Router> to send messages to
  //     Suppliers, so we don't expect this to happen very much.  When
  //     it does we use a separate thread to avoid taxing the main
  //     thread, which processes "normal" messages.
  //
  //     All of these methods are called via base class pointers by
  //     the <ACE_Stream> apparatus.  Therefore, we can put them in
  //     the protected section.
public:
  Supplier_Router (Peer_Router_Context *prc);
  // Initialization method.

protected:
  // = ACE_Task hooks.

  virtual int open (void *a = 0);
  // Called by the Stream to initialize the router.

  virtual int close (u_long flags = 0);
  // Called by the Stream to shutdown the router.

  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);
  // Called by the <SUPPLIER_HANDLER> to pass a message to the Router.
  // The Router queues up this message, which is then processed in the
  // <svc> method in a separate thread.

  virtual int svc (void);
  // Runs in a separate thread to dequeue messages and pass them up
  // the stream.

  virtual int info (char **info_string, size_t length) const;
  // Dynamic linking hook.
};

#endif /* _SUPPLIER_ROUTER_H */
