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

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <openssl/ssl.h>

#include "orbsvcs/SSLIOPC.h"
#include "tao/ORB_Core.h"

/**
 * @class TAO_SSLIOP_Current_Impl
 *
 * @brief TSS portion of the TAO SSLIOP::Current extension.
 *
 * This class encapsulates the thread-specific state of an SSL
 * session during a given upcall.
 */
class TAO_SSLIOP_Export TAO_SSLIOP_Current_Impl
{
public:

  /// Constructor.
  TAO_SSLIOP_Current_Impl (void);

  /// Destructor
  /// Protected to force allocation on the heap.
  ~TAO_SSLIOP_Current_Impl (void);

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
