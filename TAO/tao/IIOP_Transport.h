// This may look like C, but it's really -*- C++ -*-
// -*- C++ -*-
// ===================================================================
/**
 *  @file   IIOP_Transport.h
 *
 *  $Id$
 *
 *  @author Originally by Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_IIOP_TRANSPORT_H
#define TAO_IIOP_TRANSPORT_H
#include "ace/pre.h"

#include "tao/Transport.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "ace/SOCK_Stream.h"
#include "ace/Synch.h"
#include "ace/Svc_Handler.h"
#include "tao/IIOPC.h"


// Forward decls.
class TAO_IIOP_Connection_Handler;
class TAO_ORB_Core;
class TAO_Operation_Details;
class TAO_Pluggable_Messaging;
class TAO_Acceptor;

// Service Handler for this transport
typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
        TAO_IIOP_SVC_HANDLER;

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Export ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_IIOP_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  IIOP protocol.
 *
 *
 *
 */

class TAO_Export TAO_IIOP_Transport : public TAO_Transport
{
public:

  /// Constructor.
  TAO_IIOP_Transport (TAO_IIOP_Connection_Handler *handler,
                      TAO_ORB_Core *orb_core,
                      CORBA::Boolean flag);

  /// Default destructor.
  ~TAO_IIOP_Transport (void);

protected:
  /** @name Overridden Template Methods
   *
   * Please check the documentation in "tao/Transport.h" for more
   * details.
   */
  //@{

  virtual ACE_Event_Handler *event_handler_i (void);

  /// Access the underlying messaging object
  virtual TAO_Pluggable_Messaging *messaging_object (void);

  virtual ssize_t send_i (iovec *iov, int iovcnt,
                          size_t &bytes_transferred,
                          const ACE_Time_Value *timeout = 0);

  virtual ssize_t recv_i (char *buf,
                          size_t len,
                          const ACE_Time_Value *s = 0);

  virtual int register_handler_i (void);

  /// Method to do whatever it needs to do when the connection
  /// handler is being closed and destroyed.
  virtual void transition_handler_state_i (void);

public:
  /// @@TODO: These methods IMHO should have more meaningful
  /// names. The names seem to indicate nothing.
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_wait_time);

  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            int twoway = 1,
                            ACE_Time_Value *max_time_wait = 0);

  virtual int generate_request_header (TAO_Operation_Details &opdetails,
                                       TAO_Target_Specification &spec,
                                       TAO_OutputCDR &msg);

  virtual int messaging_init (CORBA::Octet major,
                              CORBA::Octet minor);

  virtual int tear_listen_point_list (TAO_InputCDR &cdr);

  //@}

private:

  /// Set the Bidirectional context info in the service context list
  void set_bidir_context_info (TAO_Operation_Details &opdetails);

  /// Add the listen points in <acceptor> to the <listen_point_list>
  /// if this connection is in the same interface as that of the
  /// endpoints in the <acceptor>
  int get_listen_point (IIOP::ListenPointList &listen_point_list,
                        TAO_Acceptor *acceptor);
private:

  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_IIOP_Connection_Handler *connection_handler_;

  /// Our messaging object.
  TAO_Pluggable_Messaging *messaging_object_;
};

#if defined (__ACE_INLINE__)
#include "tao/IIOP_Transport.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_IIOP_TRANSPORT_H */
