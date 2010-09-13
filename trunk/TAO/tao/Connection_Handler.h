// -*- C++ -*-

//=============================================================================
/**
 *  @file   Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan  <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONNECTION_HANDLER_H
#define TAO_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "ace/Copy_Disabled.h"
#include "tao/LF_CH_Event.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "ace/Event_Handler.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_SOCK;
class ACE_Lock;
class ACE_Event_Handler;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Transport;

/*
 * Hook to specialize the connection handler with the
 * concrete connection handler implementation.
 */
//@@ CONNECTION_HANDLER_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

/**
 * @class TAO_Connection_Handler
 *
 * @brief TAO_Connection_Handler
 *
 * This class is an abstraction for the connection handlers. The
 * connections handler in every protocol can derive from this
 * class as well as the ACE_Svc_Handler specialised for the
 * right protocol. This way, most of the common code for the
 * different protocols would be in this implementation.
 */
class TAO_Export TAO_Connection_Handler : public TAO_LF_CH_Event,
                                          private ACE_Copy_Disabled
{
public:

  /// Constructor
  explicit TAO_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Destructor
  virtual ~TAO_Connection_Handler (void);

  /// Return the underlying transport object
  TAO_Transport *transport (void);

  /// Set the underlying transport object
  void transport (TAO_Transport* transport);

  /// Is the handler closed or timed out?
  bool is_closed (void) const;

  /// Is the handler open?
  bool is_open (void) const;

  /// Closed due to timeout?
  bool is_timeout (void) const;

  /// Is the handler in the process of being connected?
  bool is_connecting (void) const;

  /// Close the underlying connection.
  /**
   * Used by the ORB to actively close connections that are idle,
   * stale or somehow are determined to be broken before the Reactor
   * does.
   *
   * @return Return 0 if the connection was already closed, non-zero
   * otherwise.
   */
  virtual int close_connection (void) = 0;

  /// The event handler calls, here so that other objects who hold a
  /// reference to this object can call the event handler methods.
  virtual int handle_input (ACE_HANDLE fd) = 0;

  /// This method is invoked from the svc () method of the Svc_Handler
  /// Object.
  int svc_i (void);

  /// A open () hook
  /**
   * See Thread_Per_Connection_Handler for a use case
   */
  virtual int open_handler (void *) = 0;

  /// A close() hook, called by the Transport Connector when they want to close
  /// this handler
  virtual int close_handler (u_long flags = 0);

  /// When waiting for an asynchronous connection to complete an
  /// additional reference must be maintained, related to bugzilla
  /// #2417. However once the connection is successfully established,
  /// this reference must be removed. Using connection_pending allows
  /// the connection handler to know that it is opening as a result of
  /// a delayed asynch connection rather than an immediate synch
  /// connection, which has no additional reference needs.
  void connection_pending (void);

  /// A pending connection may be canceled due to an error detected
  /// while the initiating thread is still in the Connector.
  void cancel_pending_connection (void);

  /// Set the Diff-Serv codepoint on outgoing packets.  Only has
  /// effect for remote protocols (e.g., IIOP); no effect for local
  /// protocols (UIOP).  Default implementation is for local
  /// protocols.  Remote protocols must overwrite implementation.
  virtual int set_dscp_codepoint (CORBA::Boolean set_network_priority);
  virtual int set_dscp_codepoint (CORBA::Long dscp_codepoint);

  /// Release the OS resources related to this handler.
  virtual int release_os_resources (void);

  virtual int handle_write_ready (const ACE_Time_Value *timeout);

  /*
   * Hook to add public methods from concrete connection handler
   * implementation onto the base connection handler.
   */

   //@@ CONNECTION_HANDLER_SPL_PUBLIC_METHODS_ADD_HOOK

protected:

  /// Return our TAO_ORB_Core pointer
  TAO_ORB_Core *orb_core (void);

  /// A common function called at the start of any protocol-specific
  /// open. Returns -1 on a failure (although no failure mode is
  /// currently defined).
  int shared_open (void);

  /// Set options on the socket
  int set_socket_option (ACE_SOCK &sock, int snd_size, int rcv_size);

  //@{
  /**
   * @name Helper methods for Event_Handler-based derived classes.
   *
   * Many (actually all so far) implementations of
   * TAO_Connection_Handler are a mixin of TAO_Connection_Handler and
   * some form of ACE_Event_Handler.  The following methods simplify
   * such implementations by capturing the common code in a single
   * place.
   */

  /// Implement the handle_output() callback
  int handle_output_eh (ACE_HANDLE h, ACE_Event_Handler * eh);

  /// Implement the handle_input() callback
  // We're actually going to pull the code from the protocol-specific
  // handlers back into this class, because they ALL look exactly the same.
  // If some other protocol comes along and needs to do something different,
  // it is always free to override handle_input() as it sees fit.
  int handle_input_eh (ACE_HANDLE h, ACE_Event_Handler * eh);
  int handle_input_internal (ACE_HANDLE h, ACE_Event_Handler *eh);

  /// Implement close_connection() for Connection_Handlers that are
  /// also Event_Handlers.
  int close_connection_eh (ACE_Event_Handler * eh);

  /// Pre-invocation hook for I/O operations (handle_input() &
  /// handle_output())
  /**
   * See the SSLIOP protocol for an interesting use-case
   */
  virtual void pre_io_hook (int & return_value);

  /// Post-invocation hook for I/O operations (handle_input() &
  /// handle_output())
  /**
   * See the SSLIOP protocol for an interesting use-case
   */
  virtual void pos_io_hook (int & return_value);
  //@}

private:
  /// Pointer to the TAO_ORB_Core
  TAO_ORB_Core * const orb_core_;

  /// Transport object reference
  TAO_Transport* transport_;

  /// Stores the connection pending state
  bool connection_pending_;

  /// Once closed make sure the transport is not added back to the cache.
  /// This is distinct from the leader-follower state so it cannot be reset.
  bool is_closed_;

  /*
   * Hook to add instance members from derived class
   * onto base Connection_Handler class. Any further
   * additions to this class should go before this
   * hook.
   */
  //@@ CONNECTION_HANDLER_SPL_PRIVATE_DATA_ADD_HOOK
};

//@@ CONNECTION_HANDLER_SPL_EXTERN_ADD_HOOK


/**
 * @class TAO_Auto_Reference
 *
 * @brief TAO_Auto_Reference acts as a "smart pointer" for
 * reference-countable instances.
 *
 * It increments the refrence count in the constructor and decrements
 * it in the destructor. The only requiement for the template
 * parameter is to be a class that provides add_reference() and
 * remove_reference().
 */
template <class T> class TAO_Auto_Reference
  : private ACE_Copy_Disabled
{
public:
  TAO_Auto_Reference (T& r): ref_ (r)
  {
    ref_.add_reference ();
  }

  ~TAO_Auto_Reference ()
  {
    ref_.remove_reference ();
  }

private:
  T& ref_;
};


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_CONNECTION_HANDLER_H*/
