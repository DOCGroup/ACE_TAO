/* -*- C++ -*- */
// $Id$

#if !defined (_CONSUMER_ROUTER_H)
#define _CONSUMER_ROUTER_H

#include "ace/SOCK_Acceptor.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "Peer_Router.h"

class Consumer_Router : public Peer_Router
  // = TITLE
  //     Provides the interface between one or more Consumers and the
  //     Event Server ACE_Stream.  
{
public:
  Consumer_Router (Peer_Router_Context *prc);
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
  // Called by the Consumer_Handler to pass a message to the Router.
  // The Router queues up this message, which is then processed in the
  // <svc> method in a separate thread.

  virtual int svc (void);
  // Runs in a separate thread to dequeue messages and pass them up
  // the stream.

  // = Dynamic linking hooks.

  virtual int info (char **info_string, size_t length) const;
  // Returns information about this service.
};

#endif /* _CONSUMER_ROUTER_H */
