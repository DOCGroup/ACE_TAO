/* -*- C++ -*- */
// $Id$

// The interface between one or more peers and a stream.  A peer
// typically runs remotely on another machine.

#if !defined (_PEER_ROUTER_H)
#define _PEER_ROUTER_H

#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Addr.h"
#include "ace/Thread_Manager.h"
#include "ace/Map_Manager.h"

#if defined (ACE_HAS_THREADS)

// Forward declaration.
template <class PEER_HANDLER, class KEY>
class Peer_Router;

template <class PEER_HANDLER, class KEY> 
class Acceptor_Factory : public ACE_Acceptor<PEER_HANDLER, ACE_UPIPE_ACCEPTOR>
{
public:
  Acceptor_Factory (Peer_Router<PEER_HANDLER, KEY> *pr);
  Peer_Router<PEER_HANDLER, KEY> *router (void);

  int init (int argc, char *argv[]);
  // Initialize the acceptor when it's linked dynamically.

private:
  Peer_Router<PEER_HANDLER, KEY> *pr_;
};

// Receive input from a Peer..
template <class ROUTER, class KEY>
class Peer_Handler : public ACE_Svc_Handler<ACE_UPIPE_STREAM, ACE_MT_SYNCH>
{
public:
  Peer_Handler (ACE_Thread_Manager * = 0);

  virtual int open (void * = 0);
  // Called by the ACE_Acceptor::handle_input() to activate this object.

  virtual int handle_input (ACE_HANDLE);
  // Receive input from the peer..

  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);  
  // Send output to a peer. 

protected:
  ROUTER *router_task_;  
  // Pointer to write task..

private:
  // Don't need this method here...
  virtual int svc (void);
};

// This abstract base class provides mechanisms for routing messages
// to/from a ACE_Stream from/to one or more peers (which are typically
// running on remote hosts).  A subclass of Peer_Router overrides the
// open(), close(), and put() methods in order to specialize the
// behavior of the router to meet application-specific requirements.

template <class PEER_HANDLER, class PEER_KEY>
class Peer_Router : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Peer_Router (ACE_Thread_Manager * = 0);
  ~Peer_Router (void);

  typedef Peer_Handler<Peer_Router<PEER_HANDLER, PEER_KEY>, PEER_KEY> HANDLER;

  // Remove a PEER_HANDLER from the PEER_MAP.
  virtual int unbind_peer (PEER_KEY);

  // Add a PEER_HANDLER to the PEER_MAP.
  virtual int bind_peer (PEER_KEY, HANDLER *);

  // Send the message block to the peer(s)..
  int send_peers (ACE_Message_Block *mb);

protected:
// Handle control messages arriving from adjacent Modules.
  virtual int control (ACE_Message_Block *); 

  // Map used to keep track of active peers.
  ACE_Map_Manager <PEER_KEY, PEER_HANDLER *, ACE_RW_Mutex> peer_map_;

  // Dynamic linking initialization hooks inherited from ACE_Task.
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);

  // Factory for accepting new PEER_HANDLERs.
  Acceptor_Factory<PEER_HANDLER, PEER_KEY> *acceptor_;

private:
// Prevent copies and pass-by-value.
  Peer_Router (const Peer_Router<PEER_HANDLER, PEER_KEY> &);
  void operator= (const Peer_Router<PEER_HANDLER, PEER_KEY> &);
};

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#else
#define ACE_INLINE 
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Peer_Router.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#endif /* ACE_HAS_THREADS */
#endif /* _PEER_ROUTER_H */
