/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Acceptor.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_ACCEPTOR_H)
#define ACE_ACCEPTOR_H

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Svc_Handler.h"
#include "ace/Strategies.h"

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Acceptor : public ACE_Service_Object
  // = TITLE
  //     Abstract factory for creating a service handler
  //     (SVC_HANDLER), accepting into the SVC_HANDLER, and
  //     activating the SVC_HANDLER.
  //
  // = DESCRIPTION
  //     Implements the basic strategy for passively establishing
  //     connections with clients.  An ACE_Acceptor is parameterized
  //     by concrete types that conform to the interfaces of
  //     PEER_ACCEPTOR and SVC_HANDLER.  The PEER_ACCEPTOR is
  //     instantiated with a transport mechanism that passively
  //     establishes connections.  The SVC_HANDLER is instantiated
  //     with a concrete type that performs the application-specific
  //     service.  An ACE_Acceptor inherits from ACE_Service_Object,
  //     which in turn inherits from ACE_Event_Handler.  This enables
  //     the ACE_Reactor to dispatch the ACE_Acceptor's handle_input
  //     method when connection events occur.  The handle_input method
  //     performs the ACE_Acceptor's default creation, connection
  //     establishment, and service activation strategies.  These
  //     strategies can be overridden by subclasses individually or as
  //     a group.
{
public:
  // = Initialization and termination methods.
  ACE_Acceptor (ACE_Reactor * = 0);
  // "Do-nothing" constructor.

  ACE_Acceptor (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
		ACE_Reactor * = ACE_Service_Config::reactor ());
  // Initialize and register <this> with the Reactor and listen for
  // connection requests at the designated <local_addr>.

  int open (const ACE_PEER_ACCEPTOR_ADDR &, 
	    ACE_Reactor * = ACE_Service_Config::reactor ());
  // Initialize and register <this> with the Reactor and listen for
  // connection requests at the designated <local_addr>.

  virtual ~ACE_Acceptor (void);
  // Close down the Acceptor's resources.

  virtual operator ACE_PEER_ACCEPTOR &() const;
  // Return the underlying PEER_ACCEPTOR object.

  virtual ACE_PEER_ACCEPTOR &acceptor (void) const;
  // Return the underlying PEER_ACCEPTOR object.

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the listening acceptor's <ACE_HANDLE>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = The following three methods define the Acceptor's strategies
  // for creating, accepting, and activating SVC_HANDLER's,
  // respectively.

  virtual SVC_HANDLER *make_svc_handler (void);
  // Bridge method for creating a SVC_HANDLER.  The default is to
  // create a new SVC_HANDLER.  However, subclasses can override this
  // policy to perform SVC_HANDLER creation in any way that they like
  // (such as creating subclass instances of SVC_HANDLER, using a
  // singleton, dynamically linking the handler, etc.).

  virtual int accept_svc_handler (SVC_HANDLER *svc_handler);
  // Bridge method for accepting the new connection into the
  // <svc_handler>.  The default behavior delegates to the
  // PEER_ACCEPTOR::accept.

  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);
  // Bridge method for activating a <svc_handler> with the appropriate
  // concurrency strategy.  The default behavior of this method is to
  // activate the SVC_HANDLER by calling its open() method (which
  // allows the SVC_HANDLER to define its own concurrency strategy).
  // However, subclasses can override this strategy to do more
  // sophisticated concurrency activations (such as making the
  // SVC_HANDLER as an "active object" via multi-threading or
  // multi-processing).

  // = Demultiplexing hooks.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE, 
			    ACE_Reactor_Mask = ACE_Event_Handler::RWE_MASK);
  // Perform termination activities when <this> is removed from the
  // <reactor>.

  virtual int handle_input (ACE_HANDLE);
  // Accepts all pending connections from clients, and creates and
  // activates SVC_HANDLERs.

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int fini (void);
  // Calls <handle_close>.

  virtual int info (char **buf, size_t) const;
  // Default version returns address info in <buf>.

  // = Service management hooks.
  virtual int suspend (void);
  // This method calls <Reactor::suspend>.

  virtual int resume (void);
  // This method calls <Reactor::resume>.

private:
  ACE_PEER_ACCEPTOR peer_acceptor_;
  // Concrete factory for accepting connections from clients...
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Strategy_Acceptor : public ACE_Acceptor <SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
  // = TITLE
  //     Abstract factory for creating a service handler
  //     (SVC_HANDLER), accepting into the SVC_HANDLER, and activating
  //     the SVC_HANDLER.
  //
  // = DESCRIPTION
  //     Implements a flexible and extensible set of strategies for
  //     passively establishing connections with clients.  There are
  //     three main strategies: (1) creating a SVC_HANDLER, (2)
  //     passively accepting a new connection from a client into the
  //     SVC_HANDLER, and (3) activating the SVC_HANDLER with a
  //     particular concurrency mechanism.
{
public:
  // = Initialization and termination methods.
  ACE_Strategy_Acceptor (const char service_name[] = 0,
			 const char service_description[] = 0);
  // Default constructor.

  ACE_Strategy_Acceptor (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
			 ACE_Reactor * = ACE_Service_Config::reactor (),
			 ACE_Creation_Strategy<SVC_HANDLER> * = 0,
			 ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> * = 0,
			 ACE_Concurrency_Strategy<SVC_HANDLER> * = 0,
			 ACE_Scheduling_Strategy<SVC_HANDLER> * = 0,
			 const char service_name[] = 0,
			 const char service_description[] = 0);
  // Initialize the appropriate strategies for creation, passive
  // connection acceptance, and concurrency, and then register <this>
  // with the Reactor and listen for connection requests at the
  // designated <local_addr>.

  int open (const ACE_PEER_ACCEPTOR_ADDR &, 
	    ACE_Reactor * = ACE_Service_Config::reactor (),
	    ACE_Creation_Strategy<SVC_HANDLER> * = 0,
	    ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> * = 0,
	    ACE_Concurrency_Strategy<SVC_HANDLER> * = 0,
	    ACE_Scheduling_Strategy<SVC_HANDLER> * = 0,
	    const char service_name[] = 0,
	    const char service_description[] = 0);
  // Initialize the appropriate strategies for creation, passive
  // connection acceptance, and concurrency, and then register <this>
  // with the Reactor and listen for connection requests at the
  // designated <local_addr>.

  virtual ~ACE_Strategy_Acceptor (void);
  // Close down the Strategy_Acceptor's resources.

  virtual operator ACE_PEER_ACCEPTOR &() const;
  // Return the underlying PEER_ACCEPTOR object.

  virtual ACE_PEER_ACCEPTOR &acceptor (void) const;
  // Return the underlying PEER_ACCEPTOR object.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Service management hooks.

  virtual int suspend (void);
  // This method delegates to the <Scheduling_Strategy>'s <suspend>
  // method.

  virtual int resume (void);
  // This method delegates to the <Scheduling_Strategy>'s <resume>
  // method.

  virtual int fini (void);
  // Calls <handle_close> when dynamically unlinked.

  virtual int info (char **buf, size_t) const;
  // Default version returns address info in <buf>.

  // = The following three methods define the <Acceptor>'s strategies
  // for creating, accepting, and activating <SVC_HANDLER>'s,
  // respectively.

  virtual SVC_HANDLER *make_svc_handler (void);
  // Bridge method for creating a <SVC_HANDLER>.  The strategy for
  // creating a <SVC_HANDLER> are configured into the Acceptor via
  // it's <creation_strategy_>.  The default is to create a new
  // <SVC_HANDLER>.  However, subclasses can override this policy to
  // perform <SVC_HANDLER> creation in any way that they like (such as
  // creating subclass instances of <SVC_HANDLER>, using a singleton,
  // dynamically linking the handler, etc.).

  virtual int accept_svc_handler (SVC_HANDLER *svc_handler);
  // Bridge method for accepting the new connection into the
  // <SVC_HANDLER>.  The default behavior delegates to the
  // <PEER_ACCEPTOR::accept> in the <Acceptor_Strategy>.

  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);
  // Bridge method for activating a <SVC_HANDLER> with the appropriate
  // concurrency strategy.  The default behavior of this method is to
  // activate the <SVC_HANDLER> by calling its <open> method (which
  // allows the <SVC_HANDLER> to define its own concurrency strategy).
  // However, subclasses can override this strategy to do more
  // sophisticated concurrency activations (such as creating the
  // <SVC_HANDLER> as an "active object" via multi-threading or
  // multi-processing).

  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  // Returns the listening acceptor's <ACE_HANDLE>.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::RWE_MASK);
  // Perform termination activities when <this> is removed from the
  // <Reactor>.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle SIGINT.

  // = These data members are "logically private" but are put in the
  // protected part in case subclasses want to access them.

  // = Define some useful typedefs.
  typedef ACE_Creation_Strategy<SVC_HANDLER> CREATION_STRATEGY;
  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY;
  typedef ACE_Concurrency_Strategy<SVC_HANDLER> CONCURRENCY_STRATEGY;
  typedef ACE_Scheduling_Strategy<SVC_HANDLER> SCHEDULING_STRATEGY;

  // = Strategy objects.

  CREATION_STRATEGY *creation_strategy_;
  // Creation strategy for an Acceptor.

  int delete_creation_strategy_;
  // 1 if <Acceptor> created the creation strategy and thus should
  // delete it, else 0.

  ACCEPT_STRATEGY *accept_strategy_;
  // Accept strategy for an <Acceptor>.

  int delete_accept_strategy_;
  // 1 if <Acceptor> created the accept strategy and thus should delete
  // it, else 0.

  CONCURRENCY_STRATEGY *concurrency_strategy_;
  // Concurrency strategy for an <Acceptor>.

  int delete_concurrency_strategy_;
  // 1 if <Acceptor> created the concurrency strategy and thus should
  // delete it, else 0.

  SCHEDULING_STRATEGY *scheduling_strategy_;
  // Scheduling strategy for an <Acceptor>.

  int delete_scheduling_strategy_;
  // 1 if <Acceptor> created the scheduling strategy and thus should
  // delete it, else 0.

  // = Service information objects.

  char *service_name_;
  // Name of the service.

  char *service_description_;
  // Description of the service.

  u_short service_port_;
  // Port number for the server.

  ACE_PEER_ACCEPTOR_ADDR service_addr_;
  // Address that the <Strategy_Acceptor> uses to listen for
  // connections.
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Oneshot_Acceptor : public ACE_Service_Object
  // = TITLE
  //     Generic factory for passively connecting clients and creating
  //     exactly one service handler (SVC_HANDLER).
  //
  // = DESCRIPTION
  //     This class works similarly to the regular acceptor except
  //     that this class doesn't need a Creation_Strategy (since the
  //     user supplies the SVC_HANDLER) or an Accept_Strategy (since
  //     this class only accepts one connection and then removes all
  //     traces (e.g., from the ACE_Reactor).
{
public:
  // = Initialization and termination methods.
  ACE_Oneshot_Acceptor (void);
  // "Do-nothing" constructor.

  ACE_Oneshot_Acceptor (const ACE_PEER_ACCEPTOR_ADDR &,
			ACE_Reactor * = ACE_Service_Config::reactor (),
			ACE_Concurrency_Strategy<SVC_HANDLER> * = 0);
  // Initialize the appropriate strategies for concurrency and
  // creation and then register <this> at the designated <local_addr>.

  int open (const ACE_PEER_ACCEPTOR_ADDR &, 
	    ACE_Reactor * = ACE_Service_Config::reactor (),
	    ACE_Concurrency_Strategy<SVC_HANDLER> * = 0);
  // Initialize the appropriate strategies for concurrency and
  // creation and then register <this> at the designated <local_addr>.

  virtual ~ACE_Oneshot_Acceptor (void);
  // Close down the <Oneshot_Acceptor>.

  // = Explicit factory operation.
  virtual int accept (SVC_HANDLER * = 0,
		      ACE_PEER_ACCEPTOR_ADDR *remote_addr = 0, 
		      const ACE_Synch_Options &synch_options = ACE_Synch_Options::defaults,
		      int restart = 1);
  // Create a SVC_HANDLER, accept the connection into the SVC_HANDLER,
  // and activate the SVC_HANDLER.
  
  virtual int cancel (void);
  // Cancel a oneshot acceptor that was started asynchronously.

  virtual operator ACE_PEER_ACCEPTOR &() const;
  // Return the underlying PEER_ACCEPTOR object.

  virtual ACE_PEER_ACCEPTOR &acceptor (void) const;
  // Return the underlying PEER_ACCEPTOR object.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);
  // Bridge method for activating a <svc_handler> with the appropriate
  // concurrency strategy.  Default behavior is to activate the
  // SVC_HANDLER as a "passive object."  However, subclasses can
  // override this strategy to do more sophisticated concurrency
  // activations (such as creating the SVC_HANDLER as an "active
  // object" via multi-threading or multi-processing).

  int shared_accept (SVC_HANDLER *svc_handler,
		     ACE_PEER_ACCEPTOR_ADDR *remote_addr,
		     ACE_Time_Value *timeout,
		     int restart);
  // Factors out the code shared between the <accept> and
  // <handle_input> methods.

  int register_handler (SVC_HANDLER *svc_handler,
			const ACE_Synch_Options &options,
			int restart);
  // Insert ourselves into the ACE_Reactor so that we can 
  // continue accepting this connection asynchronously.

  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  // Returns the listening acceptor's <ACE_HANDLE>.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::RWE_MASK);
  // Perform termination activities when <this> is removed from the
  // <reactor>.
       
  virtual int handle_input (ACE_HANDLE);
  // Accept one connection from a client and activates the
  // SVC_HANDLER. 

  virtual int handle_timeout (const ACE_Time_Value &tv, 
			      const void *arg);
  // Called when an acceptor times out...

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int fini (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int info (char **, size_t) const;
  // Default version returns address info in <buf>.

  // = Service management hooks.
  virtual int suspend (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int resume (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

private:
  SVC_HANDLER *svc_handler_;
  // Hold the svc_handler_ across asynchrony boundaries.

  int restart_;
  // Hold the restart flag across asynchrony boundaries.

  ACE_PEER_ACCEPTOR peer_acceptor_;
  // Factory that establishes connections passively.

  ACE_Concurrency_Strategy<SVC_HANDLER> *concurrency_strategy_;
  // Concurrency strategy for an Acceptor.

  int delete_concurrency_strategy_;
  // 1 if Acceptor created the concurrency strategy and thus should
  // delete it, else 0.
};

#include "ace/Acceptor.i"

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Acceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Acceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_ACCEPTOR_H */
