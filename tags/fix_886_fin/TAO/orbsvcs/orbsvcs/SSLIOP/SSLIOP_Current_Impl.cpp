// -*- C++ -*-
//
// $Id$

#include "SSLIOP_Current_Impl.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Current_Impl,
           "$Id$")

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

Security::AttributeList *
TAO_SSLIOP_Current_Impl::get_attributes (
    const Security::AttributeTypeList &attributes,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong len = attributes.length ();

  // A valid value must always be returned, so instantiate a sequence
  // regardless of whether or not it is populated.
  //
  // The maximum length of the AttributeList will never be larger than
  // the length of the AttributeTypeList.
  Security::AttributeList * attribute_list = 0;
  ACE_NEW_THROW_EX (attribute_list,
                    Security::AttributeList (len),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  Security::AttributeList_var safe_attribute_list = attribute_list;

  if (this->ssl_ == 0)
    return safe_attribute_list._retn ();

  // @@ @todo If the Security::AttributeTypeList length is zero, then
  //          return all of the attributes.

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const Security::AttributeType &attribute = attributes[i];

      // @@ Hacks just to get things going.  Cleanup soon!
      if (attribute.attribute_family.family_definer == 0  // OMG (?)
          && attribute.attribute_family.family == 1  // privileges
          && attribute.attribute_type == Security::AccessId)
        {
          CORBA::ULong j = attribute_list->length ();
          attribute_list->length (j + 1);

          // ----------------------------------------------------
          // Set the attribute_type field.
          // ----------------------------------------------------
          safe_attribute_list[j].attribute_type.attribute_type =
            attribute.attribute_type;

          // ----------------------------------------------------
          // Set the defining_authority field.
          // ----------------------------------------------------
          // SSLIOP uses X.509 certificates
          const char x509[] = "x509";
          safe_attribute_list[j].defining_authority.length (sizeof (x509));
          CORBA::Octet *buf =
            safe_attribute_list[j].defining_authority.get_buffer ();
          ACE_OS_String::memcpy (buf, x509, sizeof (x509));

          // ----------------------------------------------------
          // Set the DER encoded X.509 certificate as the value
          // field.
          // ----------------------------------------------------

          // @@ This code should be refactored.  The same operations
          //    are done in this->get_peer_certificate.

          X509 *cert = ::SSL_get_peer_certificate (this->ssl_);
          if (cert == 0)
            {
              // An error occurred, so do not include this attribute
              // in the AttributeList.  Drop the length to its
              // previous value.
              // @@ Not exactly exception-safe.  C'est la vie.
              attribute_list->length (j);
              continue;
            }

          // Get the size of the ASN.1 encoding.
          int cert_length = ::i2d_X509 (cert, 0);
          if (cert_length <= 0)
            {
              // An error occurred, so do not include this attribute
              // in the AttributeList.  Drop the length to its
              // previous value.
              // @@ Not exactly exception-safe.  C'est la vie.
              ::X509_free (cert);
              attribute_list->length (j);
              continue;
            }

          safe_attribute_list[j].value.length (cert_length);

          CORBA::Octet *buffer =
            safe_attribute_list[j].value.get_buffer ();

          // Convert from the internal X509 representation to the DER
          // encoding representation.
          (void) ::i2d_X509 (cert, &buffer);

          // Release the X509 certificate since it has already been
          // copied to the octet sequence.
          ::X509_free (cert);
        }
    }

  return safe_attribute_list._retn ();
}

SecurityLevel2::ReceivedCredentials_ptr
TAO_SSLIOP_Current_Impl::received_credentials (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    SecurityLevel2::ReceivedCredentials::_nil ());
}

void
TAO_SSLIOP_Current_Impl::get_peer_certificate (
  SSLIOP::ASN_1_Cert *certificate)
{
  if (this->ssl_ == 0)
    return;

  X509 *cert = ::SSL_get_peer_certificate (this->ssl_);
  if (cert == 0)
    return;

  // Get the size of the ASN.1 encoding.
  int cert_length = ::i2d_X509 (cert, 0);
  if (cert_length <= 0)
    return;

  certificate->length (cert_length);

  CORBA::Octet *buffer = certificate->get_buffer ();

  // Convert from the internal X509 representation to the DER encoding
  // representation.
  (void) ::i2d_X509 (cert, &buffer);

  // Release the X509 certificate since it has already been copied to
  // the ASN_1_Cert.
  ::X509_free (cert);
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

      // Convert from the internal X509 representation to the DER
      // encoding representation.
      (void) ::i2d_X509 (x, &buffer);
    }
}

CORBA::ULong
TAO_SSLIOP_Current_Impl::tag (void) const
{
  return SSLIOP::TAG_SSL_SEC_TRANS;
}
