/* -*- C++ -*- */
// @(#)Peer_Router.h	1.1	10/18/96


#if !defined (_PEER_ROUTER_H)
#define _PEER_ROUTER_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/Map_Manager.h"

#if defined (ACE_HAS_THREADS)

// Forward declaration.
template <class PEER_HANDLER, class KEY>
class Peer_Router;

template <class PEER_HANDLER, class KEY> 
class Acceptor_Factory : public ACE_Acceptor<PEER_HANDLER, ACE_SOCK_ACCEPTOR>
  // = TITLE
  //     Creates <PEER_HANDLERs>, which route events between peers.
{
public:
  Acceptor_Factory (Peer_Router<PEER_HANDLER, KEY> *pr);
  Peer_Router<PEER_HANDLER, KEY> *router (void);

  int init (int argc, char *argv[]);
  // Initialize the acceptor when it's linked dynamically.

private:
  Peer_Router<PEER_HANDLER, KEY> *pr_;
};

template <class ROUTER, class KEY>
class Peer_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
  // = TITLE
  // Receive input from a Peer.
{
public:
  Peer_Handler (ACE_Thread_Manager * = 0);

  virtual int open (void * = 0);
  // Called by the ACE_Acceptor::handle_input() to activate this object.

  virtual int handle_input (ACE_HANDLE);
  // Receive input from the peer.

  virtual int put (ACE_Message_Block *, ACE_Time_Value *tv = 0);  
  // Send output to a peer. 

protected:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> inherited;

  ROUTER *router_task_;  
  // Pointer to write task.

private:
  virtual int svc (void);
  // Don't need this method here...
};

template <class PEER_HANDLER, class PEER_KEY>
class Peer_Router : public ACE_Task<ACE_MT_SYNCH>
  // = TITLE
  //   This abstract base class provides mechanisms for routing
  //   messages to/from a ACE_Stream from/to one or more peers (which
  //   are typically running on remote hosts).
  //
  // = DESCRIPTION
  //   A subclass of Peer_Router overrides the open(), close(), and
  //   put() methods in order to specialize the behavior of the router
  //   to meet application-specific requirements.
{
public:
  // = Initialization and termination methods.
  Peer_Router (ACE_Thread_Manager * = 0);
  ~Peer_Router (void);

  typedef Peer_Handler<Peer_Router<PEER_HANDLER, PEER_KEY>, PEER_KEY> HANDLER;

  virtual int unbind_peer (PEER_KEY);
  // Remove a PEER_HANDLER from the PEER_MAP.

  virtual int bind_peer (PEER_KEY, HANDLER *);
  // Add a PEER_HANDLER to the PEER_MAP 

  int send_peers (ACE_Message_Block *mb);
  // Send the message block to the peer(s). 

protected:
  virtual int control (ACE_Message_Block *); 
  // Handle control messages arriving from adjacent Modules.

  // = Useful typedefs
  typedef ACE_Map_Manager <PEER_KEY, PEER_HANDLER *, ACE_RW_Mutex> PEER_MAP;
  typedef ACE_Map_Iterator<PEER_KEY, PEER_HANDLER *, ACE_RW_Mutex> PEER_ITERATOR;

  PEER_MAP peer_map_;
  // Map used to keep track of active peers.

  // = Dynamic linking initialization hooks inherited from ACE_Task
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);

  typedef Acceptor_Factory<PEER_HANDLER, PEER_KEY> ACCEPTOR;

  ACCEPTOR *acceptor_;
  // Factory for accepting new PEER_HANDLERs.

private:
  // = Prevent copies and pass-by-value.
  Peer_Router (const Peer_Router<PEER_HANDLER, PEER_KEY> &) {}
  void operator= (const Peer_Router<PEER_HANDLER, PEER_KEY> &) {}
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Peer_Router.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#endif /* ACE_HAS_THREADS */
#endif /* _PEER_ROUTER_H */
