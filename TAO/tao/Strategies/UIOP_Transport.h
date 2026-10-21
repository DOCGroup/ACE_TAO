// -*- C++ -*-

// ===================================================================
/**
 *  @file   UIOP_Transport.h
 *
 *  @author Originally by Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Ossama Othman <ossama@ece.uci.edu>
 *  @author Modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_UIOP_TRANSPORT_H
#define TAO_UIOP_TRANSPORT_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "tao/Strategies/strategies_export.h"
#include "ace/LSOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.

class TAO_ORB_Core;
class TAO_UIOP_Connection_Handler;

using TAO_UIOP_SVC_HANDLER = ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;

/**
 * @class TAO_UIOP_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  UIOP protocol.
 */

class TAO_Strategies_Export TAO_UIOP_Transport : public TAO_Transport
{
public:
  /// Constructor.
  TAO_UIOP_Transport (TAO_UIOP_Connection_Handler *handler,
                      TAO_ORB_Core *orb_core);

  /// Default destructor.
  ~TAO_UIOP_Transport () override = default;

protected:
  /** @name Overridden Template Methods
   *
   * These are implementations of template methods declared by TAO_Transport.
   */
  //@{
  ACE_Event_Handler *event_handler_i () override;
  TAO_Connection_Handler *connection_handler_i () override;

  /// Write the complete Message_Block chain to the connection.
  ssize_t send (iovec *iov, int iovcnt,
                size_t &bytes_transferred,
                ACE_Time_Value const *timeout = nullptr) override;

  /// Read len bytes from into buf.
  ssize_t recv (char *buf,
                size_t len,
                ACE_Time_Value const *s = nullptr) override;

public:
  /// @todo These methods IMHO should have more meaningful names.
  /// The names seem to indicate nothing.
  int send_request (TAO_Stub *stub,
                    TAO_ORB_Core *orb_core,
                    TAO_OutputCDR &stream,
                    TAO_Message_Semantics message_semantics,
                    ACE_Time_Value *max_wait_time) override;

  int send_message (TAO_OutputCDR &stream,
                    TAO_Stub *stub = nullptr,
                    TAO_ServerRequest *request = nullptr,
                    TAO_Message_Semantics message_semantics = TAO_Message_Semantics (),
                    ACE_Time_Value *max_time_wait = nullptr) override;
  //@}

private:
  TAO_UIOP_Transport (TAO_UIOP_Transport const &) = delete;
  TAO_UIOP_Transport &operator= (TAO_UIOP_Transport const &) = delete;
  TAO_UIOP_Transport (TAO_UIOP_Transport &&) = delete;
  TAO_UIOP_Transport &operator= (TAO_UIOP_Transport &&) = delete;

  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_UIOP_Connection_Handler *connection_handler_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

# endif  /* TAO_HAS_UIOP == 1 */

#include /**/ "ace/post.h"

#endif  /* TAO_UIOP_TRANSPORT_H */
