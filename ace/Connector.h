/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Connector.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_CONNECTOR_H
#define ACE_CONNECTOR_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/Map_Manager.h"
#include "ace/Svc_Handler.h"
#include "ace/Strategies.h"

template <class SVC_HANDLER>
class ACE_Svc_Tuple
{
  // = TITLE
  //    Holds the ACE_Svc_Handler and its argument and
  //    <ACE_Timer_Handle> until an asynchronous connection completes.
  //
  // = DESCRIPTION
  //    This is a no-brainer...
public:
  // = Initialization methods.
  ACE_Svc_Tuple (SVC_HANDLER *,
                 ACE_HANDLE,
                 const void * = 0,
                 long timer_id = -1);

  // = Get SVC_HANDLER.
  SVC_HANDLER *svc_handler (void);

  // = Get/set handle.
  ACE_HANDLE handle (void);
  // Get handle.
  void handle (ACE_HANDLE);
  // Set handle.

  // = Get/set argument.
  const void *arg (void);
  // Get argument.
  void arg (const void *);
  // Set argument.

  // = Set/get timer cancellation handle.
  long cancellation_id (void);
  // Get cancellation id.
  void cancellation_id (long timer_id);
  // Set cancellation id.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  SVC_HANDLER *svc_handler_;
  // Associated SVC_HANDLER.

  ACE_HANDLE handle_;
  // IPC <HANDLE> that we are trying to connect.

  const void *arg_;
  // Associated argument.

  long cancellation_id_;
  // Associated cancellation id.
};

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
class ACE_Connector : public ACE_Service_Object
{
  // = TITLE
  //     Generic factory for actively connecting clients and creating
  //     service handlers (SVC_HANDLERs).
  //
  // = DESCRIPTION
  //     Implements the strategy for actively establishing connections
  //     with clients.  An ACE_Connector is parameterized by concrete
  //     types that conform to the interfaces of PEER_CONNECTOR and
  //     SVC_HANDLER.  The PEER_CONNECTOR is instantiated with a
  //     transport mechanism that passively establishes connections.
  //     The SVC_HANDLER is instantiated with a concrete type that
  //     performs the application-specific service.  An ACE_Connector
  //     inherits from ACE_Service_Object, which in turn inherits from
  //     ACE_Event_Handler.  This enables the ACE_Reactor to dispatch
  //     the ACE_Connector's handle_output method when connections
  //     complete asynchronously.  The handle_output method performs
  //     the connector's active connection establishment and service
  //     activation strategy.
public:
  // = Initialization and termination methods.

  // typedef ACE_TYPENAME ACE_PEER_CONNECTOR_ADDR PEER_ADDR;
#if defined (ACE_HAS_TYPENAME_KEYWORD)
  typedef ACE_PEER_CONNECTOR_ADDR ACE_PEER_ADDR_TYPEDEF;
#endif /* ACE_HAS_TYPENAME_KEYWORD */

  ACE_Connector (ACE_Reactor *r = ACE_Reactor::instance (),
                 int flags = 0);
  // Initialize a connector.  <flags> indicates how <SVC_HANDLER>'s
  // should be initialized prior to being activated.  Right now, the
  // only flag that is processed is <ACE_NONBLOCK>, which enabled
  // non-blocking I/O on the <SVC_HANDLER> when it is opened.


  virtual int open (ACE_Reactor *r = ACE_Reactor::instance (),
                    int flags = 0);
  // Initialize a connector.  <flags> indicates how <SVC_HANDLER>'s
  // should be initialized prior to being activated.  Right now, the
  // only flag that is processed is <ACE_NONBLOCK>, which enabled
  // non-blocking I/O on the <SVC_HANDLER> when it is opened.

  virtual ~ACE_Connector (void);
  // Shutdown a connector and release resources.

  // = Connection establishment methods.

  virtual int connect (SVC_HANDLER *&svc_handler,
                       const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                       const ACE_Synch_Options &synch_options = ACE_Synch_Options::defaults,
                       const ACE_PEER_CONNECTOR_ADDR &local_addr
                         = (ACE_PEER_CONNECTOR_ADDR &) ACE_PEER_CONNECTOR_ADDR_ANY,
                       int reuse_addr = 0,
                       int flags = O_RDWR,
                       int perms = 0);
  // Initiate connection of <svc_handler> to peer at <remote_addr>
  // using <synch_options>.  If the caller wants to designate the
  // selected <local_addr> they can (and can also insist that the
  // <local_addr> be reused by passing a value <reuse_addr> ==
  // 1). <flags> and <perms> can be used to pass any flags that are
  // needed to perform specific operations such as opening a file
  // within connect with certain permissions.

  virtual int connect_n (size_t n,
                         SVC_HANDLER *svc_handlers[],
                         ACE_PEER_CONNECTOR_ADDR remote_addrs[],
                         ASYS_TCHAR *failed_svc_handlers = 0,
                         const ACE_Synch_Options &synch_options = ACE_Synch_Options::defaults);
  // Initiate connection of <n> <svc_handlers> to peers at
  // <remote_addrs> using <synch_options>.  Returns -1 if failure
  // occurs and 0 otherwise.  If <failed_svc_handlers> is non-NULL, a
  // 1 is placed in the corresponding index of <failed_svc_handler>
  // for each <svc_handlers[i]> that failed to connect, else a 0 is
  // placed in that index.

  virtual int cancel (SVC_HANDLER *svc_handler);
  // Cancel a <svc_handler> that was started asynchronously. Note that
  // this is the only case when the Connector does not actively close
  // the <svc_handler>. It is left up to the caller of <cancel> to
  // decide the fate of the <svc_handler>.

  virtual int close (void);
  // Close down the Connector

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Helpful typedefs.

  typedef ACE_Svc_Tuple<SVC_HANDLER> AST;

  typedef ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *, ACE_SYNCH_RW_MUTEX> MAP_MANAGER;
  typedef ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *, ACE_SYNCH_RW_MUTEX> MAP_ITERATOR;
  typedef ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *> MAP_ENTRY;

  // = The following two methods define the Connector's strategies for
  // creating, connecting, and activating SVC_HANDLER's, respectively.

  virtual int make_svc_handler (SVC_HANDLER *&sh);
  // Bridge method for creating a SVC_HANDLER.  The default is to
  // create a new SVC_HANDLER only if <sh> == 0, else <sh> is
  // unchanged.  However, subclasses can override this policy to
  // perform SVC_HANDLER creation in any way that they like (such as
  // creating subclass instances of SVC_HANDLER, using a singleton,
  // dynamically linking the handler, etc.).  Returns -1 if failure,
  // else 0.

  virtual int connect_svc_handler (SVC_HANDLER *&svc_handler,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  // Bridge method for connecting the <svc_handler> to the
  // <remote_addr>.  The default behavior delegates to the
  // <PEER_CONNECTOR::connect>.

  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);
  // Bridge method for activating a <svc_handler> with the appropriate
  // concurrency strategy.  The default behavior of this method is to
  // activate the SVC_HANDLER by calling its open() method (which
  // allows the SVC_HANDLER to define its own concurrency strategy).
  // However, subclasses can override this strategy to do more
  // sophisticated concurrency activations (such as creating the
  // SVC_HANDLER as an "active object" via multi-threading or
  // multi-processing).

  virtual int handle_input (ACE_HANDLE);
  // Called by ACE_Reactor when asynchronous connections fail.

  virtual int handle_output (ACE_HANDLE);
  // Called by ACE_Reactor when asynchronous connections succeed.

  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called by ACE_Reactor when asynchronous connections complete (on
  // some platforms only).

  // = Dynamic linking hooks.
  virtual int init (int argc, ASYS_TCHAR *argv[]);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int fini (void);
  // Calls <handle_close> to shutdown the Connector gracefully.

  virtual int info (ASYS_TCHAR **, size_t) const;
  // Default version returns address info in <buf>.

  // = Demultiplexing hooks.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Terminate the Client ACE_Connector by iterating over any
  // unconnected ACE_Svc_Handler's and removing them from the
  // ACE_Reactor.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);
  // This method is called if a connection times out before
  // completing.

  // = Service management hooks.
  virtual int suspend (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int resume (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  int create_AST (SVC_HANDLER *,
                  const ACE_Synch_Options &);
  // Creates and inserts an ACE_Svc_Tuple into the <handler_map_>.
  // so that we can continue accepting this connection asynchronously.

  int cleanup_AST (ACE_HANDLE, AST *&);
  // Cleanup the <handler_map_> and returns the appropriate
  // ACE_Svc_Tuple (which is 0 if there is no associated tuple).

  MAP_MANAGER handler_map_;
  // Lookup table that maps an I/O handle to a SVC_HANDLER *.

private:
  ACE_PEER_CONNECTOR connector_;
  // This is the concrete connector factory (it keeps no state so the
  // <ACE_Connector> is reentrant).

  char closing_;
  // Keeps track of whether we are in the process of closing (required
  // to avoid circular calls to <handle_close>).

  int flags_;
  // Flags that indicate how <SVC_HANDLER>'s should be initialized
  // prior to being activated.  Right now, the only flag that is
  // processed is <ACE_NONBLOCK>, which enabled non-blocking I/O on
  // the <SVC_HANDLER> when it is opened.
};

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
class ACE_Strategy_Connector : public ACE_Connector <SVC_HANDLER, ACE_PEER_CONNECTOR_2>
{
  // = TITLE
  //     Abstract factory for creating a service handler
  //     (SVC_HANDLER), connecting the SVC_HANDLER, and activating the
  //     SVC_HANDLER.
  //
  // = DESCRIPTION
  //     Implements a flexible and extensible set of strategies for
  //     actively establishing connections with clients.  There are
  //     three main strategies: (1) creating a SVC_HANDLER, (2)
  //     actively connecting a new connection from a client into the
  //     SVC_HANDLER, and (3) activating the SVC_HANDLER with a
  //     particular concurrency mechanism.
public:
  ACE_Strategy_Connector (ACE_Reactor *r = ACE_Reactor::instance (),
                          ACE_Creation_Strategy<SVC_HANDLER> * = 0,
                          ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2> * = 0,
                          ACE_Concurrency_Strategy<SVC_HANDLER> * = 0,
                          int flags = 0);
  // Initialize a connector.  <flags> indicates how <SVC_HANDLER>'s
  // should be initialized prior to being activated.  Right now, the
  // only flag that is processed is <ACE_NONBLOCK>, which enabled
  // non-blocking I/O on the <SVC_HANDLER> when it is opened.

  virtual int open (ACE_Reactor *r,
                    int flags);
  // Initialize a connector.  <flags> indicates how <SVC_HANDLER>'s
  // should be initialized prior to being activated.  Right now, the
  // only flag that is processed is <ACE_NONBLOCK>, which enabled
  // non-blocking I/O on the <SVC_HANDLER> when it is opened.
  // Default strategies would be created and used.

  virtual int open (ACE_Reactor *r = ACE_Reactor::instance (),
                    ACE_Creation_Strategy<SVC_HANDLER> * = 0,
                    ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2> * = 0,
                    ACE_Concurrency_Strategy<SVC_HANDLER> * = 0,
                    int flags = 0);
  // Initialize a connector.  <flags> indicates how <SVC_HANDLER>'s
  // should be initialized prior to being activated.  Right now, the
  // only flag that is processed is <ACE_NONBLOCK>, which enabled
  // non-blocking I/O on the <SVC_HANDLER> when it is opened.

  virtual ~ACE_Strategy_Connector (void);
  // Shutdown a connector and release resources.

  virtual int close (void);
  // Close down the Connector

  // = Define some useful typedefs traits.
  typedef ACE_Creation_Strategy<SVC_HANDLER>
          CREATION_STRATEGY;
  typedef ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
          CONNECT_STRATEGY;
  typedef ACE_Concurrency_Strategy<SVC_HANDLER>
          CONCURRENCY_STRATEGY;
  typedef ACE_Connector <SVC_HANDLER, ACE_PEER_CONNECTOR_2>
          SUPER;

  // = Strategies accessors
  virtual ACE_Creation_Strategy<SVC_HANDLER> *creation_strategy (void) const;
  virtual ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2> *connect_strategy (void) const;
  virtual ACE_Concurrency_Strategy<SVC_HANDLER> *concurrency_strategy (void) const;

protected:
  // = The following three methods define the <Connector>'s strategies
  // for creating, connecting, and activating <SVC_HANDLER>'s,
  // respectively.

  virtual int make_svc_handler (SVC_HANDLER *&sh);
  // Bridge method for creating a <SVC_HANDLER>.  The strategy for
  // creating a <SVC_HANDLER> are configured into the Connector via
  // it's <creation_strategy_>.  The default is to create a new
  // <SVC_HANDLER> only if <sh> == 0, else <sh> is unchanged.
  // However, subclasses can override this policy to perform
  // <SVC_HANDLER> creation in any way that they like (such as
  // creating subclass instances of <SVC_HANDLER>, using a singleton,
  // dynamically linking the handler, etc.).  Returns -1 if failure,
  // else 0.

  virtual int connect_svc_handler (SVC_HANDLER *&sh,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  // Bridge method for connecting the new connection into the
  // <SVC_HANDLER>.  The default behavior delegates to the
  // <PEER_CONNECTOR::connect> in the <Connect_Strategy>.

  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);
  // Bridge method for activating a <SVC_HANDLER> with the appropriate
  // concurrency strategy.  The default behavior of this method is to
  // activate the <SVC_HANDLER> by calling its <open> method (which
  // allows the <SVC_HANDLER> to define its own concurrency strategy).
  // However, subclasses can override this strategy to do more
  // sophisticated concurrency activations (such as creating the
  // <SVC_HANDLER> as an "active object" via multi-threading or
  // multi-processing).

  // = Strategy objects.

  CREATION_STRATEGY *creation_strategy_;
  // Creation strategy for an <Connector>.

  int delete_creation_strategy_;
  // 1 if <Connector> created the creation strategy and thus should
  // delete it, else 0.

  CONNECT_STRATEGY *connect_strategy_;
  // Connect strategy for a <Connector>.

  int delete_connect_strategy_;
  // 1 if <Connector> created the connect strategy and thus should
  // delete it, else 0.

  CONCURRENCY_STRATEGY *concurrency_strategy_;
  // Concurrency strategy for an <Connector>.

  int delete_concurrency_strategy_;
  // 1 if <Connector> created the concurrency strategy and thus should
  // delete it, else 0.
};

#include "ace/Connector.i"

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Connector.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Connector.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_CONNECTOR_H */
