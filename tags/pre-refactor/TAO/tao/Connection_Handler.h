
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

#include "ace/pre.h"

#include "LF_CH_Event.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_ORB_Core;
class TAO_ORB_Core_TSS_Resources;
class TAO_Transport;
class ACE_SOCK;
class ACE_Lock;
class ACE_Event_Handler;

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
class TAO_Export TAO_Connection_Handler : public TAO_LF_CH_Event
{
public:

  /// Constructor
  TAO_Connection_Handler (void);

  /// Constructor
  TAO_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Return the underlying transport object
  TAO_Transport *transport (void);

  /// Set the underlying transport object
  void transport (TAO_Transport* transport);

# if 0
  /// NOTE: NO longer used
  /// Did the process of non-blocking connection initialization
  /// complete?
  int is_connect_complete (void) const;

  /// Was the non-blocking connection initialization successful?
  int is_connect_successful (void) const;
#endif /*if 0*/
  /// Is the state final?
  int is_finalized (void);

  /// Increment and decrement the refcount. The object is deleted when
  /// the refcount reaches zero.
  long incr_refcount (void);
  long decr_refcount (void);

  /// Set the handler in <CODE>LF_EVENT_CONNECTION_CLOSE_WAIT </CODE>
  /// state
  void connection_close_wait (void);

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

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * the reference counting mechanism.
   */
  virtual ~TAO_Connection_Handler (void);

  /// Return our TAO_ORB_Core pointer
  TAO_ORB_Core *orb_core (void);

  /// Return our TSS Resources pointer
  TAO_ORB_Core_TSS_Resources* tss_resources (void);

  /// Set options on the socket
  int set_socket_option (ACE_SOCK &sock,
                         int snd_size,
                         int rcv_size);

  /// This method is invoked from the svc () method of the Svc_Handler
  /// Object.
  int svc_i (void);

  /****
   * Not sure where they are defined and used.. Looks legacy.
   *
   * Increment and decrement the number of upcalls that have gone
   * this handler. Returns the upcall count. The calls are
   * safe..
   * int incr_pending_upcalls (void);
   * int decr_pending_upcalls (void);
   * int pending_upcalls (void) const;
   */

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

  /// Implement the handle_close() callback
  virtual int handle_close_eh (ACE_HANDLE h,
                               unsigned long reactor_mask,
                               ACE_Event_Handler * eh);

  /// Implement the handle_output() callback
  int handle_output_eh (ACE_HANDLE h, ACE_Event_Handler * eh);

  /// Implement the handle_input() callback
  int handle_input_eh (ACE_HANDLE h, ACE_Event_Handler * eh);

  /// Implement close_connection() for Connection_Handlers that are
  /// also Event_Handlers.
  int close_connection_eh (ACE_Event_Handler * eh);

  /// Release the OS resources related to this handler, used in
  /// handle_close_eh()
  virtual int release_os_resources (void);

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
  TAO_ORB_Core *orb_core_;

  /// Transport object reference
  TAO_Transport* transport_;

  /// Cached tss resources of the ORB that activated this object.
  TAO_ORB_Core_TSS_Resources *tss_resources_;

  /// Pretty obvious
  long reference_count_;

  /// Lock for the reference count
  ACE_Lock *refcount_lock_;

  /// Internal state lock, needs to be separate from the reference
  /// count / pending upcalls lock because they interleave.
  ACE_Lock * lock_;
};

#if defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /*TAO_CONNECTION_HANDLER_H*/
