// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Current_Impl.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SSLIOP_CURRENT_IMPL_H
#define TAO_SSLIOP_CURRENT_IMPL_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <openssl/ssl.h>

#include "orbsvcs/SSLIOPC.h"
#include "orbsvcs/Security/Security_Current_Impl.h"
#include "tao/ORB_Core.h"

/**
 * @class TAO_SSLIOP_Current_Impl
 *
 * @brief TSS portion of the TAO SSLIOP::Current extension.
 *
 * This class encapsulates the thread-specific state of an SSL
 * session during a given upcall.
 */
class TAO_SSLIOP_Current_Impl
  : public TAO_Security_Current_Impl
{
public:

  /// Constructor.
  TAO_SSLIOP_Current_Impl (void);

  /// Destructor
  ~TAO_SSLIOP_Current_Impl (void);

  /// SSLIOP-specific version of the
  /// SecurityLevel1::Current::get_attributes() method.
  virtual Security::AttributeList * get_attributes (
      const Security::AttributeTypeList &attributes
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// SSLIOP-specific version of the
  /// SecurityLevel2::Current::received_credentials() method.
  virtual SecurityLevel2::ReceivedCredentials_ptr received_credentials (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the SSL peer certificate associated with the
  /// current request as an octet sequence, i.e. a DER encoded
  /// certificate.
  void get_peer_certificate (SSLIOP::ASN_1_Cert *certificate);

  /// Return the SSL peer certificate chain associated with the
  /// current request as a sequence of DER encoded certificates.
  void get_peer_certificate_chain (SSLIOP::SSL_Cert *cert_chain);

  /// Set the pointer to the underlying SSL session state.
  void ssl (SSL *s);

  /// Return pointer to the SSL session state for the current upcall.
  SSL *ssl (void);

protected:

  /// Return the unique tag that identifies the concrete subclass.
  virtual CORBA::ULong tag (void) const;

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  //@{
  ACE_UNIMPLEMENTED_FUNC (
    TAO_SSLIOP_Current_Impl (const TAO_SSLIOP_Current_Impl &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_SSLIOP_Current_Impl &))
  //@}

private:

  /// The SSL session state corresponding to the current upcall.
  SSL *ssl_;

};

#if defined (__ACE_INLINE__)
# include "SSLIOP_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_CURRENT_IMPL_H */
