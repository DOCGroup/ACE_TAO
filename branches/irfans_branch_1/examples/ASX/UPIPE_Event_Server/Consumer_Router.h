/* -*- C++ -*- */
// $Id$

// The interface between one or more consumers and an Event Server
// ACE_Stream.

#if !defined (_CONSUMER_ROUTER_H)
#define _CONSUMER_ROUTER_H

#include "ace/Thread_Manager.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Addr.h"
#include "ace/Svc_Handler.h"
#include "Peer_Router.h"

#if defined (ACE_HAS_THREADS)

class Consumer_Handler; // Forward declaration....

typedef ACE_HANDLE CONSUMER_KEY;

typedef Peer_Router<Consumer_Handler, CONSUMER_KEY> CONSUMER_ROUTER;

class Consumer_Handler 
  : public Peer_Handler<CONSUMER_ROUTER, CONSUMER_KEY>
{
public:
  Consumer_Handler (ACE_Thread_Manager *tm = 0);
  virtual int open (void *);
};

class Consumer_Router : public CONSUMER_ROUTER
{
public:
  Consumer_Router (ACE_Thread_Manager *thr_manager);

protected:
  // ACE_Task hooks..
  virtual int open (void *a = 0);
  virtual int close (u_long flags = 0);
  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);
  virtual int svc (void);

  // Dynamic linking hooks.
  virtual int info (char **info_string, size_t length) const;
};
#endif /* ACE_HAS_THREADS */
#endif /* _CONSUMER_ROUTER_H */
