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

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace IIOP
{
  class ListenPointList;
}

// Forward decls.
class TAO_IIOP_Connection_Handler;
class TAO_ORB_Core;
class TAO_Operation_Details;
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
                      TAO_ORB_Core *orb_core);

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

  virtual ssize_t send (iovec *iov, int iovcnt,
                        size_t &bytes_transferred,
                        const ACE_Time_Value *timeout = 0);

#if TAO_HAS_SENDFILE == 1
  virtual ssize_t sendfile (TAO_MMAP_Allocator * allocator,
                            iovec * iov,
                            int iovcnt,
                            size_t &bytes_transferred,
                            TAO::Transport::Drain_Constraints const & dc);
#endif  /* TAO_HAS_SENDFILE==1 */


  virtual ssize_t recv (char *buf, size_t len, const ACE_Time_Value *s = 0);


public:

  /// Bridge method to call a similar method on the connection handler
  void update_protocol_properties (int send_buffer_size,
                                   int recv_buffer_size,
                                   int no_delay,
                                   int enable_network_priority);

  /// @todo These methods IMHO should have more meaningful
  /// names. The names seem to indicate nothing.
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            TAO_Message_Semantics message_semantics,
                            ACE_Time_Value *max_wait_time);

  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            TAO_ServerRequest *request = 0,
                            TAO_Message_Semantics message_semantics = TAO_Message_Semantics (),
                            ACE_Time_Value *max_time_wait = 0);

  virtual int tear_listen_point_list (TAO_InputCDR &cdr);

  virtual TAO_Connection_Handler * connection_handler_i (void);

  //@}

private:

  /// Set the Bidirectional context info in the service context list
  void set_bidir_context_info (TAO_Operation_Details &opdetails);

  /// Add the listen points in @a acceptor to the @a listen_point_list
  /// if this connection is in the same interface as that of the
  /// endpoints in the @a acceptor
  int get_listen_point (IIOP::ListenPointList &listen_point_list,
                        TAO_Acceptor *acceptor);
private:

  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_IIOP_Connection_Handler *connection_handler_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_IIOP_TRANSPORT_H */
