/* -*- C++ -*- */
// $Id$

#if !defined (_SUPPLIER_ROUTER_H)
#define _SUPPLIER_ROUTER_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Map_Manager.h"
#include "ace/Svc_Handler.h"
#include "Peer_Router.h"

class Supplier_Router : public Peer_Router
  // = TITLE
  //     Provides the interface between one or more Suppliers and the
  //     Event Server ACE_Stream.  
  //     
  // = DESCRIPTION
  //     When used on the "reader" side of a Stream, this Router Task
  //     simply forwards all messages up the stream.  When used on the
  //     "writer" side, this Router Task queues up outgoing messages
  //     to suppliers and sends them in a separate thread.  The reason
  //     for this is that it's really an "error" for a
  //     <Supplier_Router> to send messages to Suppliers, so we don't
  //     expect this to happen very much.  when it does we use a
  //     separate thread to avoid taxing the main thread.
{
public:
  Supplier_Router (Peer_Router_Context *prc);
  // Initialization method.

protected:
  // = ACE_Task hooks.

  // All of these methods are called via base class pointers by the
  // ACE Stream apparatus.  Therefore, we can put them in the
  // protected section.

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
