#include "SSLIOP_Current_Impl.h"

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "SSLIOP_X509.h"
#include "SSLIOP_ClientCredentials.h"

#include "tao/ORB_Constants.h"

#include <openssl/x509.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Current_Impl::~Current_Impl ()
{
}

SecurityLevel3::ClientCredentials_ptr
TAO::SSLIOP::Current_Impl::client_credentials ()
{
#if (OPENSSL_VERSION_NUMBER >= 0x30000000L)
  TAO::SSLIOP::X509_var cert = ::SSL_get1_peer_certificate(this->ssl_);
#else
  TAO::SSLIOP::X509_var cert = ::SSL_get_peer_certificate(this->ssl_);
#endif
  if (cert.ptr () == 0)
    throw CORBA::BAD_OPERATION ();

  SecurityLevel3::ClientCredentials_ptr creds;
  ACE_NEW_THROW_EX (creds,
                    TAO::SSLIOP::ClientCredentials (cert.in (),
                                                    0,
                                                    this->ssl_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return creds;
}

CORBA::Boolean
TAO::SSLIOP::Current_Impl::request_is_local ()
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO::SSLIOP::Current_Impl::get_peer_certificate (
  ::SSLIOP::ASN_1_Cert *certificate)
{
  if (this->ssl_ == 0)
    return;

#if (OPENSSL_VERSION_NUMBER >= 0x30000000L)
  TAO::SSLIOP::X509_var cert = ::SSL_get1_peer_certificate(this->ssl_);
#else
  TAO::SSLIOP::X509_var cert = ::SSL_get_peer_certificate(this->ssl_);
#endif
  if (cert.ptr () == 0)
    return;

  // Get the size of the ASN.1 encoding.
  int const cert_length = ::i2d_X509 (cert.in (), 0);
  if (cert_length <= 0)
    return;

  certificate->length (cert_length);

  CORBA::Octet *buffer = certificate->get_buffer ();

  // Convert from the internal X509 representation to the DER encoding
  // representation.
  (void) ::i2d_X509 (cert.in (), &buffer);
}

void
TAO::SSLIOP::Current_Impl::get_peer_certificate_chain (
  ::SSLIOP::SSL_Cert *cert_chain)
{
  if (this->ssl_ == 0)
    return;

  STACK_OF (X509) *certs = ::SSL_get_peer_cert_chain (this->ssl_);
  if (certs == 0)
    return;

  int const chain_length = sk_X509_num (certs);
  cert_chain->length (chain_length);

  // Copy the peer certificate chain to the SSLIOP::SSL_Cert
  // sequence.
  for (int i = 0; i < chain_length; ++i)
    {
      // Extract the certificate from the OpenSSL X509 stack.
      ::X509 *x = sk_X509_value (certs, i);

      // Get the size of the ASN.1 encoding.
      int const cert_length = ::i2d_X509 (x, 0);
      if (cert_length <= 0)
        continue;  // @@ What do we do if there is an error?

      ::SSLIOP::ASN_1_Cert &certificate = (*cert_chain)[i];
      certificate.length (cert_length);

      CORBA::Octet *buffer = certificate.get_buffer ();

      // Convert from the internal X509 representation to the DER
      // encoding representation.
      (void) ::i2d_X509 (x, &buffer);
    }
}

CORBA::ULong
TAO::SSLIOP::Current_Impl::tag () const
{
  return ::SSLIOP::TAG_SSL_SEC_TRANS;
}

TAO_END_VERSIONED_NAMESPACE_DECL
