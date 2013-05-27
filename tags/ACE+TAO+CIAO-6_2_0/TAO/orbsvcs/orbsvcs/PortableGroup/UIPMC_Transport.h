// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Transport.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_UIPMC_TRANSPORT_H
#define TAO_UIPMC_TRANSPORT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport.h"

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Refcountable_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.
class TAO_ORB_Core;
class TAO_UIPMC_Connection_Handler;

/**
 * @class TAO_UIPMC_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  client side MIOP protocol.
 */
class TAO_PortableGroup_Export TAO_UIPMC_Transport : public TAO_Transport
{
public:
  /// Constructor.
  TAO_UIPMC_Transport (TAO_UIPMC_Connection_Handler *handler,
                       TAO_ORB_Core *orb_core);

  /// Default destructor.
  ~TAO_UIPMC_Transport (void);

protected:
  /** @name Overridden Template Methods
   *
   * These are implementations of template methods declared by TAO_Transport.
   */
  //@{

  virtual ACE_Event_Handler *event_handler_i (void);
  virtual TAO_Connection_Handler *connection_handler_i (void);

  /// Write the complete Message_Block chain to the connection.
  virtual ssize_t send (iovec *iov, int iovcnt,
                        size_t &bytes_transferred,
                        const ACE_Time_Value *max_wait_time);

  /// Shouldn't ever be called on the client side (read len bytes into buf).
  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value const *s = 0);

  virtual int register_handler (void);

public:
  /// @@TODO: These methods IMHO should have more meaningful
  /// names. The names seem to indicate nothing.
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            TAO_Message_Semantics message_semantics,
                            ACE_Time_Value *max_wait_time);

  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            TAO_ServerRequest *request = 0,
                            TAO_Message_Semantics message_semantics =
                              TAO_Message_Semantics (),
                            ACE_Time_Value *max_time_wait = 0);
  //@}

private:
  /// Construct and write a unique ID to the MIOP header.
  bool write_unique_id (TAO_OutputCDR &miop_hdr) const;

  /// Throttle back clients send rate so as to not exceed client/server buffers
  /// and servant message processing time.
  void throttle_send_rate (
    ACE_UINT64 max_fragment_rate,
    u_long max_fragment_size,
    u_long this_send_size);

  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_UIPMC_Connection_Handler *connection_handler_;

  /// This UUID used by client for making unique MIOP IDs.
  u_long uuid_hash_;

  /// On the client side the total amount of outstanding data being
  /// transmitted and the time when this was last updated.
  u_long total_bytes_outstanding_;
  ACE_Time_Value time_last_sent_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_UIPMC_TRANSPORT_H */
