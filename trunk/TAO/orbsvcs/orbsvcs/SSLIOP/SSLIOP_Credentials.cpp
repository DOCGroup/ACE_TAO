// -*- C++ -*-

#include "SSLIOP_Credentials.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Credentials,
           "$Id$")

#include <openssl/asn1.h>
#include <openssl/x509.h>

#include "SSLIOP_X509.h"


// SSLIOP does not support delegation, nor does it support detection
// of replayed or misordered GIOP messages.
const Security::AssociationOptions TAO_SSLIOP_DISALLOWED_ASSOCIATIONS =
  Security::DetectReplay
  | Security::DetectMisordering
  | Security::SimpleDelegation
  | Security::CompositeDelegation;


TAO_SSLIOP_Credentials::TAO_SSLIOP_Credentials (X509 *cert)
  : accepting_options_supported_ (Security::Integrity
                                  | Security::Confidentiality
                                  | Security::EstablishTrustInTarget
                                  | Security::NoDelegation),
    accepting_options_required_ (Security::Integrity
                                 | Security::Confidentiality
                                 | Security::NoDelegation),
    invocation_options_supported_ (accepting_options_supported_),
    invocation_options_required_ (Security::NoDelegation),
    x509_ (TAO_SSLIOP_X509::_duplicate (cert))
{
}

TAO_SSLIOP_Credentials::~TAO_SSLIOP_Credentials (void)
{
  TAO_SSLIOP_X509::release (this->x509_);
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_Credentials::copy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_SSLIOP_Credentials *c = 0;
  ACE_NEW_THROW_EX (c,
                    TAO_SSLIOP_Credentials (this->x509_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (SecurityLevel2::Credentials::_nil ());

  return c;
}

void
TAO_SSLIOP_Credentials::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_SSLIOP_X509::release (this->x509_);
  this->x509_ = 0;
}

Security::InvocationCredentialsType
TAO_SSLIOP_Credentials::credentials_type (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Security::SecOwnCredentials;
}

Security::AuthenticationStatus
TAO_SSLIOP_Credentials::authentication_state (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Security::UtcT expiry_time;

  CORBA::Boolean is_valid =
    this->is_valid (expiry_time
                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Security::SecAuthFailure);

  // @@ Is the following statement correct?

  // No "continued authentication" occurs in SSL.  Authentication is
  // done in one step during SSL session establishment.

  if (is_valid)
    return Security::SecAuthSuccess;
  else
    return Security::SecAuthExpired;
}

char *
TAO_SSLIOP_Credentials::mechanism (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @todo Is the following correct?

  // Mechanism ID is the same as the stringified component ID.  For
  // example, SSLIOP's IOP:ComponentId is 20
  // (SSLIOP::TAG_SSL_SEC_TRANS) meaning that its mechanism ID is
  // "20."
  return CORBA::string_dup ("20");
}

Security::AssociationOptions
TAO_SSLIOP_Credentials::accepting_options_supported (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->accepting_options_supported_;
}

void
TAO_SSLIOP_Credentials::accepting_options_supported (
    Security::AssociationOptions accepting_options_supported
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @todo Verify that the given accepting options are valid

  // SSLIOP does not support delegation, nor does it support detection
  // of replayed or misordered GIOP messages.

  // "accepting_options_supported" should not have "less" capability
  // then "accepting_options_required."

  if ((accepting_options_supported & ::TAO_SSLIOP_DISALLOWED_ASSOCIATIONS)
      || (accepting_options_supported ^ this->accepting_options_required_))
    ACE_THROW (CORBA::BAD_PARAM (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  this->accepting_options_supported_ = accepting_options_supported;
}

Security::AssociationOptions
TAO_SSLIOP_Credentials::accepting_options_required (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->accepting_options_required_;
}

void
TAO_SSLIOP_Credentials::accepting_options_required (
    Security::AssociationOptions accepting_options_required
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // "accepting_options_required" should not have "more" capability
  // then "accepting_options_supported."

  if ((accepting_options_required & ::TAO_SSLIOP_DISALLOWED_ASSOCIATIONS)
      || (accepting_options_required ^ this->accepting_options_supported_))
    ACE_THROW (CORBA::BAD_PARAM (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  this->accepting_options_required_ = accepting_options_required;
}

Security::AssociationOptions
TAO_SSLIOP_Credentials::invocation_options_supported (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->invocation_options_supported_;
}

void
TAO_SSLIOP_Credentials::invocation_options_supported (
    Security::AssociationOptions invocation_options_supported
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if ((invocation_options_supported & ::TAO_SSLIOP_DISALLOWED_ASSOCIATIONS)
      || (invocation_options_supported ^ this->invocation_options_required_))
    ACE_THROW (CORBA::BAD_PARAM (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  this->invocation_options_supported_ = invocation_options_supported;
}

Security::AssociationOptions
TAO_SSLIOP_Credentials::invocation_options_required (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->invocation_options_required_;
}

void
TAO_SSLIOP_Credentials::invocation_options_required (
    Security::AssociationOptions invocation_options_required
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if ((invocation_options_required & ::TAO_SSLIOP_DISALLOWED_ASSOCIATIONS)
      || (invocation_options_required ^ this->invocation_options_supported_))
    ACE_THROW (CORBA::BAD_PARAM (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  this->invocation_options_required_ = invocation_options_required;
}

CORBA::Boolean
TAO_SSLIOP_Credentials::get_security_feature (
    Security::CommunicationDirection /* direction */,
    Security::SecurityFeature /* feature */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

CORBA::Boolean
TAO_SSLIOP_Credentials::set_attributes (
    const Security::AttributeList & /* requested_attributes */,
    Security::AttributeList_out /* actual_attributes */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

Security::AttributeList *
TAO_SSLIOP_Credentials::get_attributes (
    const Security::AttributeTypeList & /* attributes */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

CORBA::Boolean
TAO_SSLIOP_Credentials::is_valid (
    Security::UtcT_out expiry_time
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
#if 0
  // The pointer to the underlying X509 structure should only be zero
  // if destroy() was called on this Credentials object.
  if (this->x509_ == 0)
    ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);

  const ASN1_UTCTIME *expiration = X509_get_notAfter (this->x509_);

  // @todo Fill in expiry_time.

  // @todo Use of ACE_OS::time() may not be appropriate since it
  //       represents a 32-bit value on some platforms.

  return (::ASN1_UTCTIME_cmp_time_t (expiration,
                                     ACE_OS::time (0)) == -1 ? 0 : 1);
#else
  ACE_UNUSED_ARG (expiry_time);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
#endif
}

CORBA::Boolean
TAO_SSLIOP_Credentials::refresh (const CORBA::Any & /* refresh_data */
                                 TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Until it is clear the code below is correct, throw a
  // CORBA::NO_IMPLEMENT exception.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);

  // @@ I "if zeroed" out the following code since I'm not sure if the
  //    code below is correct, or if it possible to to refresh the
  //    X.509 certificates for the given SSL session by renegotiating
  //    the that session.

#if 0
  // @@ What about the private key?  Do we need to refresh it as well?

  SSLIOP::ASN_1_Cert *der_cert = 0;
  char *cert_filename = 0;

  // Attempt to extract a certificate in DER format from the Any.
  if (refresh_data >>= der_cert)
    {
      if (::SSL_use_certificate_ASN1 (this->ssl_,
                                      der_cert->get_buffer (),
                                      der_cert->length ()) <= 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) ERROR: Unable to refresh SSLIOP "
                        "credentials using certificate in ASN.1 "
                        "format.\n"));

          ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
        }
    }

  // Attempt to extract a filename that contains the certificate from
  // the Any.
  else if (refresh_data >>= cert_filename)
    {
      // @@ Should we do this sort of thing, i.e. first try to open a
      //    PEM file, and then an ASN.1 file?

      // Try extracting a PEM formatted certificate from the given
      // file.
      if (::SSL_use_certificate_file (this->ssl_,
                                      cert_filename,
                                      SSL_FILETYPE_PEM) <= 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_WARNING,
                        "(%P|%t) WARNING: Unable to refresh SSLIOP "
                        "credentials using PEM certificate in file \"%s\"\n",
                        cert_filename));
        }

      // Try extracted an ASN.1 formatted certificate from the given
      // file.
      else if (::SSL_use_certificate_file (this->ssl_,
                                           cert_filename,
                                           SSL_FILETYPE_ASN1) <= 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_WARNING,
                        "(%P|%t) WARNING: Unable to refresh SSLIOP "
                        "credentials using ASN.1 certificate in file \"%s\"\n",
                        cert_filename));
        }

      else
        ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);
    }
  else
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  // Renegotiate the SSL session to force the new settings to be used.
  if (::SSL_renegotiate (this->ssl_) <= 0)
    ACE_THROW_RETURN (CORBA::BAD_OPERATION (), 0);

  return 1;
#endif  /* 0 */
}
