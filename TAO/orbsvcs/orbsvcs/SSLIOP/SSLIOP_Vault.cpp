#include "SSLIOP_Vault.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Vault,
           "$Id$")


#include <openssl/x509.h>
#include <openssl/pem.h>

#include "SSLIOP_Credentials.h"

#include "tao/debug.h"

extern "C"
int
TAO_SSLIOP_password_callback (char *buf,
                              int size,
                              int /* rwflag */,
                              void *userdata)
{
  // @@ I'm probably over complicating this implementation, but that's
  //    what you get when you try to be overly efficient.  :-)
  //        -Ossama

  const char *password = ACE_static_cast (char *, userdata);

  int pwlen = -1;

  if (password != 0)
    {
      pwlen = ACE_OS::strlen (password);

      int copy_len = pwlen + 1;  // Include the NULL terminator

      // Clear the portion of the buffer that exceeds the space that
      // will be occupied by the password.
      if (copy_len < size)
        ACE_OS::memset (buf + copy_len, 0, size - copy_len);

      // Make sure we don't overflow the OpenSSL supplied buffer.
      // Truncate the password if necessary.
      copy_len = (copy_len > size) ? size : copy_len;

      ACE_OS::memcpy (buf, password, copy_len);

      // NULL terminate the truncated password.
      if (copy_len > size)
        {
          pwlen = size - 1;
          buf[pwlen] = '\0';
        }
    }

  return pwlen;
}


TAO_SSLIOP_Vault::TAO_SSLIOP_Vault (void)
{
}

TAO_SSLIOP_Vault::~TAO_SSLIOP_Vault (void)
{
}

Security::AuthenticationMethodList *
TAO_SSLIOP_Vault::get_supported_authen_methods (
    const char * /* mechanism */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

Security::OIDList *
TAO_SSLIOP_Vault::supported_mech_oids (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

Security::AuthenticationStatus
TAO_SSLIOP_Vault::acquire_credentials (
    Security::AuthenticationMethod /* method */,
    const char * mechanism,
    const char * /* security_name */,
    const CORBA::Any & auth_data,
    const Security::AttributeList & /* privileges */,
    SecurityLevel2::Credentials_out creds,
    CORBA::Any_out /* continuation_data */,
    CORBA::Any_out /* auth_specific_data */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check if we support the given mechanism.
  //
  // @@ FIXME:  "SSL" may not be correct.  We really should figure out
  //    what mechanism(s) is/are valid.
  if (ACE_OS::strcmp (mechanism, "SSL") != 0)
    return Security::SecAuthFailure;

  SSLIOP::AuthData *data;

  if (!(auth_data >>= data))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), Security::SecAuthFailure);

  TAO_SSLIOP_X509_var x509 = this->make_X509 (data->certificate);

  if (x509.in () == 0)
    return Security::SecAuthFailure;

  TAO_SSLIOP_EVP_PKEY_var evp = this->make_EVP_PKEY (data->key);

  // Verify that the private key is consistent with the certificate.
  if (::X509_check_private_key (x509.in (), evp.in ()) != 1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ERROR: Private key is not ")
                    ACE_TEXT ("consistent with X.509 certificate")));

      return Security::SecAuthFailure;
    }

  ACE_NEW_THROW_EX (creds,
                    TAO_SSLIOP_Credentials (x509.in (), evp.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Security::SecAuthFailure);

  return Security::SecAuthSuccess;
}

Security::AuthenticationStatus
TAO_SSLIOP_Vault::continue_credentials_acquisition (
    const CORBA::Any & /* response_data */,
    SecurityLevel2::Credentials_ptr /* creds */,
    CORBA::Any_out /* continuation_data */,
    CORBA::Any_out /* auth_specific_data */
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    Security::SecAuthFailure);
}

IOP::TaggedComponentList *
TAO_SSLIOP_Vault::create_ior_components (
    SecurityLevel2::Credentials_ptr /* creds_list */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

Security::AssociationStatus
TAO_SSLIOP_Vault::init_security_context (
    SecurityLevel2::Credentials_ptr /* creds */,
    const char * /* target_security_name */,
    CORBA::Object_ptr /* target */,
    Security::DelegationMode /* delegation_mode */,
    const Security::OptionsDirectionPairList & /* association_options */,
    const char * /* mechanism */,
    const Security::Opaque & /* comp_data */,
    const Security::ChannelBindings & /* chan_binding */,
    Security::OpaqueBuffer_out /* security_token */,
    SecurityReplaceable::ClientSecurityContext_out /* security_context */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    Security::SecAssocFailure);
}

Security::AssociationStatus
TAO_SSLIOP_Vault::accept_security_context (
    const SecurityLevel2::CredentialsList & /* creds_list */,
    const Security::ChannelBindings & /* chan_bindings */,
    const Security::Opaque & /* in_token */,
    Security::Opaque_out /* out_token */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    Security::SecAssocFailure);
}

Security::MechandOptionsList *
TAO_SSLIOP_Vault::get_supported_mechs (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

X509 *
TAO_SSLIOP_Vault::make_X509 (const SSLIOP::File &certificate)
{
  // No password is used or needed when reading ASN.1 encoded
  // certificates.

  const char *filename = certificate.filename.in ();

  if (filename == 0)
    return 0;

  FILE *fp = 0;
  X509 *x = 0;

  if (certificate.type == SSLIOP::ASN1)
    {
      // ASN.1/DER encoded certificate

      // No password is used or needed when reading ASN.1 encoded
      // certificates.

      const char *filename = certificate.filename.in ();

      if (filename == 0)
        return 0;

      fp = ACE_OS::fopen (filename, "rb");

      if (fp == 0)
        return 0;

      // Read ASN.1 / DER encoded X.509 certificate from a file, and
      // convert it to OpenSSL's internal X.509 format.
      x = ::d2i_X509_fp (fp, 0);
    }
  else
    {
      // PEM encoded certificate

      fp = ACE_OS::fopen (filename, "r");

      if (fp == 0)
        return 0;

      const char *password = certificate.password.in ();

      // Read PEM encoded X.509 certificate from a file, and convert
      // it to OpenSSL's internal X.509 format.
      x = PEM_read_X509 (fp,
                         0,
                         TAO_SSLIOP_password_callback,
                         ACE_const_cast (char *, password));
    }

  (void) ACE_OS::fclose (fp);

  return x;
}

EVP_PKEY *
TAO_SSLIOP_Vault::make_EVP_PKEY (const SSLIOP::File &key)
{
  // No password is used or needed when reading ASN.1 encoded
  // private keys.

  const char *filename = key.filename.in ();

  if (filename == 0)
    return 0;

  FILE *fp = 0;
  EVP_PKEY *evp = 0;

  if (key.type == SSLIOP::ASN1)
    {
      // ASN.1/DER encoded private key

      // No password is used or needed when reading ASN.1 encoded
      // private keys.

      const char *filename = key.filename.in ();

      if (filename == 0)
        return 0;

      fp = ACE_OS::fopen (filename, "rb");

      if (fp == 0)
        return 0;

      // Read ASN.1 / DER encoded private key from a file, and convert
      // it to OpenSSL's internal private key format.
      evp = ::d2i_PrivateKey_fp (fp, 0);
    }
  else
    {
      // PEM encoded private key

      fp = ACE_OS::fopen (filename, "r");

      if (fp == 0)
        return 0;

      const char *password = key.password.in ();

      // Read PEM encoded private key from a file, and convert it to
      // OpenSSL's internal private key format.
      evp = PEM_read_PrivateKey (fp,
                                 0,
                                 TAO_SSLIOP_password_callback,
                                 ACE_const_cast (char *, password));
    }

  (void) ACE_OS::fclose (fp);

  return evp;
}
