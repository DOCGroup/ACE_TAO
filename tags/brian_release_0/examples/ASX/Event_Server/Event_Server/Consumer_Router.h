/* -*- C++ -*- */
// $Id$

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

class Consumer_Router : public Peer_Router
{
  // = TITLE
  //     Provides the interface between one or more Consumers and the
  //     Event Server <ACE_Stream>.
  //
  // = DESCRIPTION
  //     This class normally sits on "top" of the Stream and routes
  //     messages coming from "downstream" to all the Consumers
  //     connected to it via its "read" <Task>.  Normally, the messages
  //     flow up the stream from <Supplier_Router>s.  However, if
  //     Consumers transmit data to the <Consumer_Router>, we dutifully
  //     push it out to the Suppliers via the <Supplier_Router>.
  //
  //     When used on the "reader" side of a Stream, the
  //     <Consumer_Router> simply forwards all messages up the stream.
  //     When used on the "writer" side, the <Consumer_Router> queues
  //     up outgoing messages to suppliers and sends them down to the
  //     <Supplier_Router> in a separate thread.  The reason for this
  //     is that it's really an "error" for a <Consumer_Router> to
  //     send messages to Suppliers, so we don't expect this to happen
  //     very much.  When it does we use a separate thread to avoid
  //     taxing the main thread, which processes "normal" messages.
  //
  //     All of the methods in this class except the constructor are
  //     called via base class pointers by the <ACE_Stream>.
  //     Therefore, we can put them in the protected section.
public:
  Consumer_Router (Peer_Router_Context *prc);
  // Initialization method.

protected:
  // = ACE_Task hooks.
  virtual int open (void *a = 0);
  // Called by the Stream to initialize the router.

  virtual int close (u_long flags = 0);
  // Called by the Stream to shutdown the router.

  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);
  // Called by the <Peer_Handler> to pass a message to the
  // <Consumer_Router>.  The <Consumer_Router> queues up this message,
  // which is then processed in the <svc> method in a separate thread.

  virtual int svc (void);
  // Runs in a separate thread to dequeue messages and pass them up
  // the stream.

  // = Dynamic linking hooks.
  virtual int info (char **info_string, size_t length) const;
  // Returns information about this service.
};

#endif /* _CONSUMER_ROUTER_H */
