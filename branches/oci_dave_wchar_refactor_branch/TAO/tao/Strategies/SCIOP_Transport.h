// -*- C++ -*-

// $Id$
// ===================================================================
/**
 *  @file   SCIOP_Transport.h
 *
 *  @author  Jason Cohen, Lockheed Martin ATL  <jcohen@atl.lmco.com>
 *  @author  Keith O'Hara, Lockheed Martin ATL
 */
// ===================================================================

#ifndef TAO_SCIOP_TRANSPORT_H
#define TAO_SCIOP_TRANSPORT_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_SCIOP == 1

#include "tao/Strategies/strategies_export.h"
#include "tao/Transport.h"
#include "tao/IIOPC.h"
#include "ace/SOCK_SEQPACK_Association.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.
class TAO_SCIOP_Connection_Handler;
class TAO_ORB_Core;
class TAO_Operation_Details;
class TAO_Pluggable_Messaging;
class TAO_Acceptor;

/**
 * @class TAO_SCIOP_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  SCIOP protocol.
 *
 * Specialization of the base TAO_Transport class to handle the SCIOP
 * protocol.
 */
class TAO_Strategies_Export TAO_SCIOP_Transport : public TAO_Transport
{
public:

  /// Constructor.
  TAO_SCIOP_Transport (TAO_SCIOP_Connection_Handler *handler,
                      TAO_ORB_Core *orb_core,
                      CORBA::Boolean flag);

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * the reference counting mechanism.
   */
  virtual ~TAO_SCIOP_Transport (void);

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
  /// @todo These methods IMHO should have more meaningful
  /// names. The names seem to indicate nothing.
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &association,
                            int message_semantics,
                            ACE_Time_Value *max_wait_time);

  virtual int send_message (TAO_OutputCDR &association,
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
  TAO_SCIOP_Connection_Handler *connection_handler_;

  /// Our messaging object.
  TAO_Pluggable_Messaging *messaging_object_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SCIOP == 1 */

#include /**/ "ace/post.h"

#endif  /* TAO_SCIOP_TRANSPORT_H */
