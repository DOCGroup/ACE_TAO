/* -*- C++ -*- */
// @(#)Supplier_Router.h	1.1	10/18/96

// The interface between a supplier and an Event Service ACE_Stream.

#if !defined (_SUPPLIER_ROUTER_H)
#define _SUPPLIER_ROUTER_H

#include "ace/UPIPE_Addr.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/Map_Manager.h"
#include "ace/Svc_Handler.h"
#include "Peer_Router.h"

#if defined (ACE_HAS_THREADS)

// Forward declaration.
class Supplier_Handler; 

// Type of search key for SUPPLIER_MAP.
typedef long SUPPLIER_KEY;

// Instantiated type for routing messages to suppliers.

typedef Peer_Router<Supplier_Handler, SUPPLIER_KEY> SUPPLIER_ROUTER;

class Supplier_Handler 
  : public Peer_Handler<SUPPLIER_ROUTER, SUPPLIER_KEY>
{
public:
  Supplier_Handler (ACE_Thread_Manager *tm = 0);
  virtual int open (void *);
};

class Supplier_Router : public SUPPLIER_ROUTER
{
public:
  Supplier_Router (ACE_Thread_Manager *);

protected:
  // ACE_Task hooks..
  virtual int open (void *a = 0);
  virtual int close (u_long flags = 0);
  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);
  virtual int svc (void);

  // Dynamic linking hooks inherited from Peer_Router.
  virtual int info (char **info_string, size_t length) const;
};

#endif /* ACE_HAS_THREADS */
#endif /* _SUPPLIER_ROUTER_H */
