/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Acceptor.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_ACCEPTOR_H
#define ACE_ACCEPTOR_H
#include "ace/pre.h"

#include "ace/Svcconf/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svcconf/Service_Object.h"
#include "ace/Svcconf/Svc_Handler.h"
#include "ace/Connection/Strategies_T.h"

/**
 * @class ACE_Acceptor
 *
 * @brief Abstract factory for creating a service handler
 * (SVC_HANDLER), accepting into the SVC_HANDLER, and
 * activating the SVC_HANDLER.
 *
 * Implements the basic strategy for passively establishing
 * connections with clients.  An ACE_Acceptor is parameterized
 * by concrete types that conform to the interfaces of
 * PEER_ACCEPTOR and SVC_HANDLER.  The PEER_ACCEPTOR is
 * instantiated with a transport mechanism that passively
 * establishes connections.  The SVC_HANDLER is instantiated
 * with a concrete type that performs the application-specific
 * service.  An ACE_Acceptor inherits from ACE_Service_Object,
 * which in turn inherits from ACE_Event_Handler.  This enables
 * the ACE_Reactor to dispatch the ACE_Acceptor's handle_input
 * method when connection events occur.  The handle_input method
 * performs the ACE_Acceptor's default creation, connection
 * establishment, and service activation strategies.  These
 * strategies can be overridden by subclasses individually or as
 * a group.
 */
template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Acceptor : public ACE_Service_Object
{
public:
  // = Initialization and termination methods.
  /// "Do-nothing" constructor.
  ACE_Acceptor (ACE_Reactor * = 0,
                int use_select = 1);

  /**
   * Initialize and register <this> with the Reactor and listen for
   * connection requests at the designated <local_addr>.  <flags>
   * indicates how <SVC_HANDLER>'s should be initialized prior to
   * being activated.  Right now, the only flag that is processed is
   * <ACE_NONBLOCK>, which enabled non-blocking I/O on the
   * <SVC_HANDLER> when it is opened.  If <use_select> is non-zero
   * then <select> is used to determine when to break out of the
   * <accept> loop.  <reuse_addr> is passed down to the
   * <PEER_ACCEPTOR>.  If it is non-zero this will allow the OS to
   * reuse this listen port.
   */
  ACE_Acceptor (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                ACE_Reactor * = ACE_Reactor::instance (),
                int flags = 0,
                int use_select = 1,
                int reuse_addr = 1);

  /**
   * Initialize and register <this> with the Reactor and listen for
   * connection requests at the designated <local_addr>.  <flags>
   * indicates how <SVC_HANDLER>'s should be initialized prior to
   * being activated.  Right now, the only flag that is processed is
   * <ACE_NONBLOCK>, which enabled non-blocking I/O on the
   * <SVC_HANDLER> when it is opened.  If <use_select> is non-zero
   * then <select> is used to determine when to break out of the
   * <accept> loop.  <reuse_addr> is passed down to the
   * <PEER_ACCEPTOR>.  If it is non-zero this will allow the OS to
   * reuse this listen port.
   */
  int open (const ACE_PEER_ACCEPTOR_ADDR &,
            ACE_Reactor * = ACE_Reactor::instance (),
            int flags = 0,
            int use_select = 1,
            int reuse_addr = 1);

  /// Close down the Acceptor's resources.
  virtual ~ACE_Acceptor (void);

  /// Return the underlying PEER_ACCEPTOR object.
  virtual operator ACE_PEER_ACCEPTOR &() const;

  /// Return the underlying PEER_ACCEPTOR object.
  virtual ACE_PEER_ACCEPTOR &acceptor (void) const;

  /// Returns the listening acceptor's <ACE_HANDLE>.
  virtual ACE_HANDLE get_handle (void) const;

  /// Close down the Acceptor
  virtual int close (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = The following three methods define the Acceptor's strategies
  // for creating, accepting, and activating SVC_HANDLER's,
  // respectively.

  /**
   * Bridge method for creating a SVC_HANDLER.  The default is to
   * create a new <SVC_HANDLER> if <sh> == 0, else <sh> is unchanged.
   * However, subclasses can override this policy to perform
   * SVC_HANDLER creation in any way that they like (such as creating
   * subclass instances of SVC_HANDLER, using a singleton, dynamically
   * linking the handler, etc.).  Returns -1 on failure, else 0.
   */
  virtual int make_svc_handler (SVC_HANDLER *&sh);

  /**
   * Bridge method for accepting the new connection into the
   * <svc_handler>.  The default behavior delegates to the
   * PEER_ACCEPTOR::accept.
   */
  virtual int accept_svc_handler (SVC_HANDLER *svc_handler);

  /**
   * Bridge method for activating a <svc_handler> with the appropriate
   * concurrency strategy.  The default behavior of this method is to
   * activate the SVC_HANDLER by calling its <open> method (which
   * allows the SVC_HANDLER to define its own concurrency strategy).
   * However, subclasses can override this strategy to do more
   * sophisticated concurrency activations (such as making the
   * SVC_HANDLER as an "active object" via multi-threading or
   * multi-processing).
   */
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);

  // = Demultiplexing hooks.
  /// Perform termination activities when <this> is removed from the
  /// <reactor>.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  /// Accepts all pending connections from clients, and creates and
  /// activates SVC_HANDLERs.
  virtual int handle_input (ACE_HANDLE);

  // = Dynamic linking hooks.
  /// Default version does no work and returns -1.  Must be overloaded
  /// by application developer to do anything meaningful.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Calls <handle_close>.
  virtual int fini (void);

  /// Default version returns address info in <buf>.
  virtual int info (ACE_TCHAR **buf, size_t) const;

public:
  // = Service management hooks.
  /// This method calls <Reactor::suspend>.
  virtual int suspend (void);

  /// This method calls <Reactor::resume>.
  virtual int resume (void);

protected:
  /// Concrete factory for accepting connections from clients...
  ACE_PEER_ACCEPTOR peer_acceptor_;

  /// Needed to reopen the socket if <accept> fails.
  ACE_PEER_ACCEPTOR_ADDR peer_acceptor_addr_;

  /**
   * Flags that indicate how <SVC_HANDLER>'s should be initialized
   * prior to being activated.  Right now, the only flag that is
   * processed is <ACE_NONBLOCK>, which enabled non-blocking I/O on
   * the <SVC_HANDLER> when it is opened.
   */
  int flags_;

  /// Flag that indicates whether it shall use <select> in the
  /// <accept>-loop.
  int use_select_;

  /// Needed to reopen the socket if <accept> fails.
  int reuse_addr_;
};

/**
 * @class ACE_Strategy_Acceptor
 *
 * @brief Abstract factory for creating a service handler
 * (SVC_HANDLER), accepting into the SVC_HANDLER, and activating
 * the SVC_HANDLER.
 *
 * Implements a flexible and extensible set of strategies for
 * passively establishing connections with clients.  There are
 * three main strategies: (1) creating a SVC_HANDLER, (2)
 * passively accepting a new connection from a client into the
 * SVC_HANDLER, and (3) activating the SVC_HANDLER with a
 * particular concurrency mechanism.
 */
template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Strategy_Acceptor : public ACE_Acceptor <SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  ACE_Strategy_Acceptor (const ACE_TCHAR service_name[] = 0,
                         const ACE_TCHAR service_description[] = 0,
                         int use_select = 1);

  /**
   * Initialize the appropriate strategies for creation, passive
   * connection acceptance, and concurrency, and then register <this>
   * with the Reactor and listen for connection requests at the
   * designated <local_addr>.
   */
  ACE_Strategy_Acceptor (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                         ACE_Reactor * = ACE_Reactor::instance (),
                         ACE_Creation_Strategy<SVC_HANDLER> * = 0,
                         ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> * = 0,
                         ACE_Concurrency_Strategy<SVC_HANDLER> * = 0,
                         ACE_Scheduling_Strategy<SVC_HANDLER> * = 0,
                         const ACE_TCHAR service_name[] = 0,
                         const ACE_TCHAR service_description[] = 0,
                         int use_select = 1);

  /**
   * Initialize the appropriate strategies for creation, passive
   * connection acceptance, and concurrency, and then register <this>
   * with the Reactor and listen for connection requests at the
   * designated <local_addr>.
   */
  int open (const ACE_PEER_ACCEPTOR_ADDR &,
            ACE_Reactor * = ACE_Reactor::instance (),
            ACE_Creation_Strategy<SVC_HANDLER> * = 0,
            ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> * = 0,
            ACE_Concurrency_Strategy<SVC_HANDLER> * = 0,
            ACE_Scheduling_Strategy<SVC_HANDLER> * = 0,
            const ACE_TCHAR *service_name = 0,
            const ACE_TCHAR *service_description = 0,
            int use_select = 1);

  /// Close down the Strategy_Acceptor's resources.
  virtual ~ACE_Strategy_Acceptor (void);

  /// Return the underlying PEER_ACCEPTOR object.
  virtual operator ACE_PEER_ACCEPTOR &() const;

  /// Return the underlying PEER_ACCEPTOR object.
  virtual ACE_PEER_ACCEPTOR &acceptor (void) const;

  /// Returns the listening acceptor's <ACE_HANDLE>.
  virtual ACE_HANDLE get_handle (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Service management hooks.

  /// This method delegates to the <Scheduling_Strategy>'s <suspend>
  /// method.
  virtual int suspend (void);

  /// This method delegates to the <Scheduling_Strategy>'s <resume>
  /// method.
  virtual int resume (void);

  /// Calls <handle_close> when dynamically unlinked.
  virtual int fini (void);

  /// Default version returns address info in <buf>.
  virtual int info (ACE_TCHAR **buf, size_t) const;

  // = The following three methods define the <Acceptor>'s strategies
  // for creating, accepting, and activating <SVC_HANDLER>'s,
  // respectively.

  /**
   * Bridge method for creating a <SVC_HANDLER>.  The strategy for
   * creating a <SVC_HANDLER> are configured into the Acceptor via
   * it's <creation_strategy_>.  The default is to create a new
   * <SVC_HANDLER> if <sh> == 0, else <sh> is unchanged.  However,
   * subclasses can override this policy to perform <SVC_HANDLER>
   * creation in any way that they like (such as creating subclass
   * instances of <SVC_HANDLER>, using a singleton, dynamically
   * linking the handler, etc.).  Returns -1 on failure, else 0.
   */
  virtual int make_svc_handler (SVC_HANDLER *&);

  /**
   * Bridge method for accepting the new connection into the
   * <SVC_HANDLER>.  The default behavior delegates to the
   * <PEER_ACCEPTOR::accept> in the <Acceptor_Strategy>.
   */
  virtual int accept_svc_handler (SVC_HANDLER *svc_handler);

  /**
   * Bridge method for activating a <SVC_HANDLER> with the appropriate
   * concurrency strategy.  The default behavior of this method is to
   * activate the <SVC_HANDLER> by calling its <open> method (which
   * allows the <SVC_HANDLER> to define its own concurrency strategy).
   * However, subclasses can override this strategy to do more
   * sophisticated concurrency activations (such as creating the
   * <SVC_HANDLER> as an "active object" via multi-threading or
   * multi-processing).
   */
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);

  // = Demultiplexing hooks.
  /// Perform termination activities when <this> is removed from the
  /// <Reactor>.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  /// Handle SIGINT.
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  // = These data members are "logically private" but are put in the
  // protected part in case subclasses want to access them.

  // = Define some useful typedefs.
  typedef ACE_Creation_Strategy<SVC_HANDLER> CREATION_STRATEGY;
  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY;
  typedef ACE_Concurrency_Strategy<SVC_HANDLER> CONCURRENCY_STRATEGY;
  typedef ACE_Scheduling_Strategy<SVC_HANDLER> SCHEDULING_STRATEGY;

  // = Strategy objects.

  /// Creation strategy for an Acceptor.
  CREATION_STRATEGY *creation_strategy_;

  /// 1 if <Acceptor> created the creation strategy and thus should
  /// delete it, else 0.
  int delete_creation_strategy_;

  /// Accept strategy for an <Acceptor>.
  ACCEPT_STRATEGY *accept_strategy_;

  /// 1 if <Acceptor> created the accept strategy and thus should delete
  /// it, else 0.
  int delete_accept_strategy_;

  /// Concurrency strategy for an <Acceptor>.
  CONCURRENCY_STRATEGY *concurrency_strategy_;

  /// 1 if <Acceptor> created the concurrency strategy and thus should
  /// delete it, else 0.
  int delete_concurrency_strategy_;

  /// Scheduling strategy for an <Acceptor>.
  SCHEDULING_STRATEGY *scheduling_strategy_;

  /// 1 if <Acceptor> created the scheduling strategy and thus should
  /// delete it, else 0.
  int delete_scheduling_strategy_;

  // = Service information objects.

  /// Name of the service.
  ACE_TCHAR *service_name_;

  /// Description of the service.
  ACE_TCHAR *service_description_;

  /// Port number for the server.
  u_short service_port_;

  /// Address that the <Strategy_Acceptor> uses to listen for
  /// connections.
  ACE_PEER_ACCEPTOR_ADDR service_addr_;
};

/**
 * @class ACE_Oneshot_Acceptor
 *
 * @brief Generic factory for passively connecting clients and creating
 * exactly one service handler (SVC_HANDLER).
 *
 * This class works similarly to the regular <ACE_Acceptor>,
 * with the following differences:
 * 1. This class doesn't automagically register <this> with the
 * <ACE_Reactor> since it expects to have its <accept> method
 * called directly.  However, it stashes the <ACE_Reactor>
 * pointer away in case it's needed later to finish accepting
 * a connection asynchronously.
 * 2. The class doesn't need an <ACE_Creation_Strategy> (since
 * the user supplies the SVC_HANDLER) or an
 * <ACE_Accept_Strategy> (since this class only accepts one
 * connection and then removes all traces of itself from the
 * <ACE_Reactor> if it was registered for asynchronous
 * accepts).
 */
template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Oneshot_Acceptor : public ACE_Service_Object
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  ACE_Oneshot_Acceptor (void);

  /**
   * Initialize the appropriate strategies for concurrency and then
   * open the <peer_acceptor> at the designated <local_addr>.  Note
   * that unlike the <ACE_Acceptor> and <ACE_Strategy_Acceptor>, this
   * method does NOT register <this> acceptor with the <reactor> at
   * this point -- it just stashes the <reactor> away in case it's
   * needed later.
   */
  ACE_Oneshot_Acceptor (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                        ACE_Reactor *reactor = ACE_Reactor::instance (),
                        ACE_Concurrency_Strategy<SVC_HANDLER> * = 0);

  /**
   * Initialize the appropriate strategies for concurrency and then
   * open the <peer_acceptor> at the designated <local_addr>.  Note
   * that unlike the <ACE_Acceptor> and <ACE_Strategy_Acceptor>, this
   * method does NOT register <this> acceptor with the <reactor> at
   * this point -- it just stashes the <reactor> away in case it's
   * needed later.
   */
  int open (const ACE_PEER_ACCEPTOR_ADDR &,
            ACE_Reactor *reactor = ACE_Reactor::instance (),
            ACE_Concurrency_Strategy<SVC_HANDLER> * = 0);

  /// Close down the <Oneshot_Acceptor>.
  virtual ~ACE_Oneshot_Acceptor (void);

  // = Explicit factory operation.
  /// Create a <SVC_HANDLER>, accept the connection into the
  /// <SVC_HANDLER>, and activate the <SVC_HANDLER>.
  virtual int accept (SVC_HANDLER * = 0,
                      ACE_PEER_ACCEPTOR_ADDR *remote_addr = 0,
                      const ACE_Synch_Options &synch_options = ACE_Synch_Options::defaults,
                      int restart = 1,
                      int reset_new_handle = 0);

  /// Cancel a oneshot acceptor that was started asynchronously.
  virtual int cancel (void);

  /// Return the underlying <PEER_ACCEPTOR> object.
  virtual operator ACE_PEER_ACCEPTOR &() const;

  /// Return the underlying <PEER_ACCEPTOR> object.
  virtual ACE_PEER_ACCEPTOR &acceptor (void) const;

  /// Close down the <Oneshot_Acceptor>.
  virtual int close (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /**
   * Bridge method for activating a <svc_handler> with the appropriate
   * concurrency strategy.  Default behavior is to activate the
   * <SVC_HANDLER> as a "passive object."  However, subclasses can
   * override this strategy to do more sophisticated concurrency
   * activations (such as creating the <SVC_HANDLER> as an "active
   * object" via multi-threading or multi-processing).
   */
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);

  /// Factors out the code shared between the <accept> and
  /// <handle_input> methods.
  int shared_accept (SVC_HANDLER *svc_handler,
                     ACE_PEER_ACCEPTOR_ADDR *remote_addr,
                     ACE_Time_Value *timeout,
                     int restart,
                     int reset_new_handle);

  // = Demultiplexing hooks.
  /// Returns the listening acceptor's <ACE_HANDLE>.
  virtual ACE_HANDLE get_handle (void) const;

  /// Perform termination activities when <this> is removed from the
  /// <reactor>.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  /// Accept one connection from a client and activates the
  /// SVC_HANDLER.
  virtual int handle_input (ACE_HANDLE);

  /// Called when an acceptor times out...
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  // = Dynamic linking hooks.
  /// Default version does no work and returns -1.  Must be overloaded
  /// by application developer to do anything meaningful.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Default version does no work and returns -1.  Must be overloaded
  /// by application developer to do anything meaningful.
  virtual int fini (void);

  /// Default version returns address info in <buf>.
  virtual int info (ACE_TCHAR **, size_t) const;

  // = Service management hooks.
  /// Default version does no work and returns -1.  Must be overloaded
  /// by application developer to do anything meaningful.
  virtual int suspend (void);

  /// Default version does no work and returns -1.  Must be overloaded
  /// by application developer to do anything meaningful.
  virtual int resume (void);

private:
  /**
   * Insert ourselves into the <ACE_Reactor> so that we can continue
   * accepting this connection asynchronously.  This method should NOT
   * be called by developers directly.
   */
  int register_handler (SVC_HANDLER *svc_handler,
                        const ACE_Synch_Options &options,
                        int restart);

  /// Hold the svc_handler_ across asynchrony boundaries.
  SVC_HANDLER *svc_handler_;

  /// Hold the restart flag across asynchrony boundaries.
  int restart_;

  /// Factory that establishes connections passively.
  ACE_PEER_ACCEPTOR peer_acceptor_;

  /// Concurrency strategy for an Acceptor.
  ACE_Concurrency_Strategy<SVC_HANDLER> *concurrency_strategy_;

  /// 1 if Acceptor created the concurrency strategy and thus should
  /// delete it, else 0.
  int delete_concurrency_strategy_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Connection/Acceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Acceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_ACCEPTOR_H */
