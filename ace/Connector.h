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

#if !defined (ACE_CONNECTOR_H)
#define ACE_CONNECTOR_H

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Timer_Queue.h"
#include "ace/Map_Manager.h"
#include "ace/Svc_Handler.h"
#include "ace/Strategies.h"

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define ACE_PEER_CONNECTOR_1 class PEER_CONNECTOR
#define ACE_PEER_CONNECTOR_2 PEER_CONNECTOR
#define ACE_PEER_CONNECTOR PEER_CONNECTOR
#define ACE_PEER_CONNECTOR_ADDR PEER_CONNECTOR::PEER_ADDR
#else
#define ACE_PEER_CONNECTOR_1 class PEER_CONNECTOR, class PEER_ADDR
#define ACE_PEER_CONNECTOR_2 PEER_CONNECTOR, PEER_ADDR
#define ACE_PEER_CONNECTOR PEER_CONNECTOR
#define ACE_PEER_CONNECTOR_ADDR PEER_ADDR
#endif /* ACE_TEMPLATE_TYPEDEFS */

template <class SVC_HANDLER>
class ACE_Svc_Tuple
  // = TITLE
  //    Holds the ACE_Svc_Handler and its argument and
  //    <ACE_Timer_Handle> until an asynchronous connection completes. 
  //
  // = DESCRIPTION
  //    This is a no-brainer...
{
public:
  // = Initialization methods.
  ACE_Svc_Tuple (SVC_HANDLER *, 
		 ACE_HANDLE,
		 const void * = 0, 
		 int timer_id = 0);

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

  // = Set/get cancellation handle.
  int cancellation_id (void);
  // Get cancellation id.
  void cancellation_id (int timer_id);
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

  int cancellation_id_;
  // Associated cancellation id.
};

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
class ACE_Connector : public ACE_Service_Object
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
{
public:
  // = Initialization and termination methods.

  ACE_Connector (ACE_Reactor *r = ACE_Service_Config::reactor ());
  // Initialize a connector.

  virtual int open (ACE_Reactor *r = ACE_Service_Config::reactor ());
  // Initialize a connector.

  ~ACE_Connector (void);
  // Shutdown a connector and release resources.

  // = Connection establishment method

  virtual int connect (SVC_HANDLER *svc_handler,
		       const ACE_PEER_CONNECTOR_ADDR &remote_addr,
		       const ACE_Synch_Options &synch_options = ACE_Synch_Options::defaults,
		       const ACE_PEER_CONNECTOR_ADDR &local_addr 
		         = (ACE_PEER_CONNECTOR_ADDR &) ACE_PEER_CONNECTOR_ADDR::sap_any,
		       int reuse_addr = 0,
		       int flags = O_RDWR,
		       int perms = 0);
  // Initiate connection to <peer> at <remote_addr> using
  // <synch_options>.  If the caller wants to designate the selected
  // <local_addr> they can (and can also insist that the <local_addr>
  // be reused by passing a value <reuse_addr> == 1). <flags> and
  // <perms> can be used to pass any flags that are needed to perform
  // specific operations such as opening a file within connect with
  // certain permissions.

  virtual int cancel (SVC_HANDLER *svc_handler);
  // Cancel a <svc_handler> that was started asynchronously.

  ACE_PEER_CONNECTOR &connector (void) const;
  // Return the underlying PEER_CONNECTOR object.

  ACE_Reactor *reactor (void) const;
  // Get the underlying Reactor *.

  void reactor (ACE_Reactor *);
  // Set the underlying Reactor *.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Helpful typedefs.

  typedef ACE_Svc_Tuple<SVC_HANDLER> AST;

#if defined (ACE_MT_SAFE)
  typedef ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *, ACE_RW_Mutex> MAP_MANAGER;
  typedef ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *, ACE_RW_Mutex> MAP_ITERATOR;
  typedef ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *> MAP_ENTRY;
//  ACE_Thread_Mutex lock_;
#else
  typedef ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *, ACE_Null_Mutex> MAP_MANAGER;
  typedef ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *, ACE_Null_Mutex> MAP_ITERATOR;
  typedef ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<SVC_HANDLER> *> MAP_ENTRY;
#endif /* ACE_MT_SAFE */

  // = The following two methods define the Connector's strategies for
  // connecting and activating SVC_HANDLER's, respectively.

  virtual int connect_svc_handler (SVC_HANDLER *svc_handler,
				   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
				   const ACE_Synch_Options &synch_options,
				   const ACE_PEER_CONNECTOR_ADDR &local_addr,
				   int reuse_addr,
				   int flags,
				   int perms);
  // Bridge method for connecting the <svc_handler> to the
  // <remote_addr>.  The default behavior delegates to the
  // PEER_CONNECTOR::connect.

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

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int fini (void);
  // Calls <handle_close> to shutdown the Connector gracefully.

  virtual int info (char **, size_t) const;
  // Default version returns address info in <buf>.

  // = Demultiplexing hooks.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Terminate the Client ACE_Connector by iterating over any
  // unconnected ACE_Svc_Handler's and removing them from the
  // ACE_Reactor.

  virtual ACE_HANDLE get_handle (void) const;
  // Return the underlying handle that's associated with the
  // SVC_HANDLER we're currently trying to connect (or
  // ACE_INVALID_HANDLER if there isn't one).

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
  // Factor that establishes connections actively.

  ACE_Reactor *reactor_;
  // Event demultiplex associated with this object.
};

#include "ace/Connector.i"

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Connector.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Connector.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_CONNECTOR_H */
