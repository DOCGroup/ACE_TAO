// -*- C++ -*-
//
// $Id$

#include "SSLIOP_Current_Impl.h"

ACE_RCSID (TAO_SSLIOP, SSLIOP_Current_Impl, "$Id$")

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

#include <openssl/x509.h>

TAO_SSLIOP_Current_Impl::TAO_SSLIOP_Current_Impl (void) 
  : ssl_ (0)
{
}

TAO_SSLIOP_Current_Impl::~TAO_SSLIOP_Current_Impl (void)
{
}

void
TAO_SSLIOP_Current_Impl::get_peer_certificate_chain (
  SSLIOP::SSL_Cert *cert_chain)
{
  if (this->ssl_ == 0)
    return;

  STACK_OF(X509) *certs = ::SSL_get_peer_cert_chain (this->ssl_);
  if (certs == 0)
    return;

  int chain_length = sk_X509_num (certs);
  cert_chain->length (chain_length);

  // Copy the peer certificate chain to the SSLIOP::SSL_Cert
  // sequence.
  for (int i = 0; i < chain_length; ++i)
    {
      // Extract the certificate from the OpenSSL X509 stack.
      X509 *x = sk_X509_value (certs, i);

      // Get the size of the ASN.1 encoding.
      int cert_length = ::i2d_X509 (x, 0);
      if (cert_length <= 0)
        continue;  // @@ What do we do if there is an error?

      SSLIOP::ASN_1_Cert &certificate = (*cert_chain)[i];
      certificate.length (cert_length);

      CORBA::Octet *buffer = certificate.get_buffer ();
      (void) ::i2d_X509 (x, &buffer);
    }
}
