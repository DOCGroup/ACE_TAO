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

#include "tao/IIOPC.h"
#include "ace/SOCK_Stream.h"
#include "ace/Synch.h"


// Forward decls.
class TAO_IIOP_Connection_Handler;
class TAO_ORB_Core;
class TAO_Operation_Details;
class TAO_Pluggable_Messaging;
class TAO_Acceptor;
class TAO_Adapter;

/**
 * @class TAO_IIOP_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  IIOP protocol.
 *
 * Specialization of the base TAO_Transport class to handle the IIOP
 * protocol.
 */
class TAO_Export TAO_IIOP_Transport : public TAO_Transport
{
public:

  /// Constructor.
  TAO_IIOP_Transport (TAO_IIOP_Connection_Handler *handler,
                      TAO_ORB_Core *orb_core,
                      CORBA::Boolean flag);

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * the reference counting mechanism.
   */
  virtual ~TAO_IIOP_Transport (void);

  /** @name Overridden Template Methods
   *
   * Please check the documentation in "tao/Transport.h" for more
   * details.
   */
  //@{

  virtual ACE_Event_Handler * event_handler_i (void);

  /// Access the underlying messaging object
  virtual TAO_Pluggable_Messaging *messaging_object (void);

  virtual ssize_t send (iovec *iov, int iovcnt,
                        size_t &bytes_transferred,
                        const ACE_Time_Value *timeout = 0);

  virtual ssize_t recv (char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0);

  virtual int send_message_shared (TAO_Stub *stub,
                                   int message_semantics,
                                   const ACE_Message_Block *message_block,
                                   ACE_Time_Value *max_wait_time);


public:

  /// Bridge method to call a similar method on the connection handler
  void update_protocol_properties (int send_buffer_size,
                                   int recv_buffer_size,
                                   int no_delay,
                                   int enable_network_priority);

  /// @@TODO: These methods IMHO should have more meaningful
  /// names. The names seem to indicate nothing.
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int message_semantics,
                            ACE_Time_Value *max_wait_time);

  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            int message_semantics = TAO_Transport::TAO_TWOWAY_REQUEST,
                            ACE_Time_Value *max_time_wait = 0);

  /*virtual int send_reply (TAO_OutputCDR &stream,
                          TAO_Adapter *poa = 0);*/

  virtual int generate_request_header (TAO_Operation_Details &opdetails,
                                       TAO_Target_Specification &spec,
                                       TAO_OutputCDR &msg);

  virtual int messaging_init (CORBA::Octet major,
                              CORBA::Octet minor);

  virtual int tear_listen_point_list (TAO_InputCDR &cdr);

  virtual TAO_Connection_Handler * connection_handler_i (void);

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
