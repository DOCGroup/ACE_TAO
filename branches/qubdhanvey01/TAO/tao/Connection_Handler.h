
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

#include "LF_Event.h"

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
class TAO_Export TAO_Connection_Handler : public TAO_LF_Event
{
public:

  /// Constructor
  TAO_Connection_Handler (void);

  /// Constructor
  TAO_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Destructor
  virtual ~TAO_Connection_Handler (void);

  /// Return the underlying transport object
  TAO_Transport *transport (void);

  /// Set the underlying transport object
  void transport (TAO_Transport* transport);

  /// Did the process of non-blocking connection initialization
  /// complete?
  int is_connect_complete (void) const;

  /// Was the non-blocking connection initialization successful?
  int is_connect_successful (void) const;

  /// Increment and decrement the refcount. The object is deleted when
  /// the refcount reaches zero.
  int incr_refcount (void);
  void decr_refcount (void);

  /// The event handler calls, here so that other objects who hold a
  /// reference to this object can call the event handler methods.
  virtual int handle_input (ACE_HANDLE fd) = 0;

  void update_protocol_properties (int send_buffer_size,
                                   int recv_buffer_size,
                                   int no_delay,
                                   int enable_network_priority);
protected:

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

  /// Increment and decrement the number of upcalls that have gone
  /// through this handler. Returns the upcall count. The calls are
  /// thread safe..
  int incr_pending_upcalls (void);

  int decr_pending_upcalls (void);

  /// Query the upcall count
  int pending_upcalls (void) const;

  /// Shutdown the object
  virtual void handle_close_i (void) = 0;

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

  /// Implement the handle_close_i() template method.
  void handle_close_i_eh (ACE_Event_Handler * eh);

  /// Implement the handle_output() callback
  int handle_output_eh (ACE_HANDLE h, ACE_Event_Handler * eh);

  /// Implement the handle_input() callback
  int handle_input_eh (ACE_HANDLE h, ACE_Event_Handler * eh);

  /// Release the OS resources related to this handler, used in handle_close_eh()
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

  /// Count nested upcalls on this
  /// svc_handler i.e., the connection can close during nested upcalls,
  /// you should not delete the svc_handler until the stack unwinds
  /// from the nested upcalls.
  long pending_upcalls_;

  /* Have a count of the number of references to the
   * handler. Theoretically this should be in the reactor. As we dont
   * have this in the reactor we are providing it here.
   *
   * NOTE: Please dont try to combine this with the pending
   * upcalls. They are for two completely different things.
   * @@todo: Need to be moved into the reactor at a later date
   */
  long reference_count_;

  /// Lock for the <pending_upcalls_>. We can have more than one
  /// thread trying to access.
  ACE_Lock *pending_upcall_lock_;
};

#if defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /*TAO_CONNECTION_HANDLER_H*/
