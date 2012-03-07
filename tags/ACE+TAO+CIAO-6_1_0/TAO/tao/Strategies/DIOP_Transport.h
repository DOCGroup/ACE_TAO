// -*- C++ -*-

//=============================================================================
/**
 *  @file   DIOP_Transport.h
 *
 *  $Id$
 *
 *  @author  Michael Kircher
 */
//=============================================================================

#ifndef TAO_DIOP_TRANSPORT_H
#define TAO_DIOP_TRANSPORT_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Transport.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Svc_Handler.h"

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Strategies_Export ACE_Svc_Handler<ACE_SOCK_DGRAM, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.
class TAO_DIOP_Connection_Handler;
class TAO_ORB_Core;
class TAO_Operation_Details;
class TAO_Acceptor;

// Service Handler for this transport
typedef ACE_Svc_Handler<ACE_SOCK_DGRAM, ACE_NULL_SYNCH>
        TAO_DIOP_SVC_HANDLER;

/**
 * @class TAO_DIOP_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  DIOP protocol.
 */
class TAO_Strategies_Export TAO_DIOP_Transport : public TAO_Transport
{
public:

  /// Constructor.
  TAO_DIOP_Transport (TAO_DIOP_Connection_Handler *handler,
                      TAO_ORB_Core *orb_core);

  /// Default destructor.
  ~TAO_DIOP_Transport (void);

  /// Look for the documentation in Transport.h.
  virtual int handle_input (TAO_Resume_Handle &rh,
                            ACE_Time_Value *max_wait_time = 0);
protected:
  /** @name Overridden Template Methods
   *
   * These are implementations of template methods declared by TAO_Transport.
   */
  //@{

  virtual ACE_Event_Handler * event_handler_i (void);
  virtual TAO_Connection_Handler *connection_handler_i (void);

  /// Write the complete Message_Block chain to the connection.
  virtual ssize_t send (iovec *iov, int iovcnt,
                        size_t &bytes_transferred,
                        const ACE_Time_Value *max_wait_time);

  /// Read len bytes from into buf.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0);

  virtual int register_handler (void);

  ///@}
public:
  /// @todo These methods IMHO should have more meaningful names.
  /// The names seem to indicate nothing.
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            TAO_Message_Semantics message_semantics,
                            ACE_Time_Value *max_wait_time);

  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            TAO_Message_Semantics message_semantics = TAO_Message_Semantics (),
                            ACE_Time_Value *max_time_wait = 0);

private:

  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_DIOP_Connection_Handler *connection_handler_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_DIOP_TRANSPORT_H */
