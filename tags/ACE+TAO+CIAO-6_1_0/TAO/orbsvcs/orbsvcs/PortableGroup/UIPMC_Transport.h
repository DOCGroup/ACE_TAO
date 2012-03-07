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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.
class TAO_ORB_Core;
class TAO_Operation_Details;
class TAO_Acceptor;

/**
 * @class TAO_UIPMC_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  MIOP protocol.
 */
template<typename CONNECTION_HANDLER>
class TAO_UIPMC_Transport : public TAO_Transport
{
public:

  /// Constructor.
  TAO_UIPMC_Transport (CONNECTION_HANDLER *handler,
                       TAO_ORB_Core *orb_core);

  /// Default destructor.
  ~TAO_UIPMC_Transport (void);

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
                            TAO_Message_Semantics message_semantics =
                              TAO_Message_Semantics (),
                            ACE_Time_Value *max_time_wait = 0);
  //@}

private:
  /// Construct and write a unique ID to the MIOP header.
  void write_unique_id (TAO_OutputCDR &miop_hdr, unsigned long unique);

private:

  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  CONNECTION_HANDLER *connection_handler_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/PortableGroup/UIPMC_Transport.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("orbsvcs/PortableGroup/UIPMC_Transport.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif  /* TAO_UIPMC_TRANSPORT_H */
