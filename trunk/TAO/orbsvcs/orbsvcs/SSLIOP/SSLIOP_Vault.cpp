#include "SSLIOP_Vault.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Vault,
           "$Id$")


#include <openssl/x509.h>
#include <openssl/pem.h>

#include "SSLIOP_Credentials.h"


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

  const char *password = ACE_static_cast (const char *, userdata);

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

Security::OIDList *
TAO_SSLIOP_Vault::supported_mech_oids (TAO_ENV_SINGLE_ARG_DECL)
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
    TAO_ENV_ARG_DECL)
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

#ifndef NO_RSA
  if (data->key_type == SSLIOP::RSA)
    {
      TAO_SSLIOP_RSA_var rsa = this->make_RSA (data->key);
      ACE_NEW_THROW_EX (creds,
                        TAO_SSLIOP_Credentials (x509.in (), rsa.in ()),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (Security::SecAuthFailure);

      return Security::SecAuthSuccess;
    }
#endif  /* !NO_RSA */

// #ifndef NO_DSA
//   if (data->key_type == SSLIOP::DSA)
//     {
//       TAO_SSLIOP_DSA_var dsa = this->make_DSA (data->key);
//       ACE_NEW_THROW_EX (creds,
//                         TAO_SSLIOP_Credentials (x509.in (), dsa.in ()),
//                         CORBA::NO_MEMORY ());
//       ACE_CHECK_RETURN (Security::SecAuthFailure);

//       return Security::SecAuthSuccess;
//     }
// #endif  /* !NO_DSA */

  ACE_NEW_THROW_EX (creds,
                    TAO_SSLIOP_Credentials (x509.in ()),
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
    TAO_ENV_ARG_DECL)
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
    TAO_ENV_ARG_DECL)
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
    TAO_ENV_ARG_DECL)
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
    TAO_ENV_SINGLE_ARG_DECL)
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

  FILE *f = 0;
  X509 *x = 0;

  if (certificate.type == SSLIOP::ASN1)
    {
      // ASN.1/DER encoded certificate

      // No password is used or needed when reading ASN.1 encoded
      // certificates.

      const char *filename = certificate.filename.in ();

      if (filename == 0)
        return 0;

      FILE * f = ACE_OS::fopen (filename, "rb");

      if (f == 0)
        return 0;

      // Read ASN.1 / DER encoded X.509 certificate from a file, and
      // convert it to OpenSSL's internal X.509 format.
      x = ::d2i_X509_fp (f, 0);
    }
  else
    {
      // PEM encoded certificate

      f = ACE_OS::fopen (filename, "r");

      if (f == 0)
        return 0;

      const char *password = certificate.password.in ();

      // Read PEM encoded X.509 certificate from a file, and convert
      // it to OpenSSL's internal X.509 format.
      x = PEM_read_X509 (f,
                         0,
                         TAO_SSLIOP_password_callback,
                         ACE_const_cast (char *, password));
    }

  (void) ACE_OS::close (f);

  return x;
}

#ifndef NO_RSA
RSA *
TAO_SSLIOP_Vault::make_RSA (const SSLIOP::File &key)
{
  // No password is used or needed when reading ASN.1 encoded
  // private keys.

  const char *filename = key.filename.in ();

  if (filename == 0)
    return 0;

  FILE *f = 0;
  RSA *r = 0;

  if (key.type == SSLIOP::ASN1)
    {
      // ASN.1/DER encoded RSA private key

      // No password is used or needed when reading ASN.1 encoded
      // certificates.

      const char *filename = key.filename.in ();

      if (filename == 0)
        return 0;

      FILE * f = ACE_OS::fopen (filename, "rb");

      if (f == 0)
        return 0;

      // Read ASN.1 / DER encoded RSA private key from a file, and
      // convert it to OpenSSL's internal RSA format.
      r = ::d2i_RSAPrivateKey_fp (f, 0);
    }
  else
    {
      // PEM encoded RSA private key

      f = ACE_OS::fopen (filename, "r");

      if (f == 0)
        return 0;

      const char *password = key.password.in ();

      // Read PEM encoded RSA private key from a file, and convert it
      // to OpenSSL's internal RSA format.
      r = PEM_read_RSAPrivateKey (f,
                                  0,
                                  TAO_SSLIOP_password_callback,
                                  ACE_const_cast (char *, password));
    }

  (void) ACE_OS::close (f);

  return r;
}
#endif  /* !NO_RSA */

// #ifndef NO_DSA
// DSA *
// TAO_SSLIOP_Vault::make_DSA (const SSLIOP::File &key)
// {
//   // No password is used or needed when reading ASN.1 encoded
//   // private keys.

//   const char *filename = key.filename.in ();

//   if (filename == 0)
//     return 0;

//   FILE *f = 0;
//   DSA *d = 0;

//   if (key.type == SSLIOP::ASN1)
//     {
//       // ASN.1/DER encoded DSA private key

//       // No password is used or needed when reading ASN.1 encoded
//       // certificates.

//       const char *filename =
//         key.filename.in ();

//       if (filename == 0)
//         return 0;

//       FILE * f = ACE_OS::fopen (filename, "rb");

//       if (f == 0)
//         return 0;

//       // Read ASN.1 / DER encoded DSA private key from a file, and
//       // convert it to OpenSSL's internal DSA format.
//       d = ::d2i_DSAPrivateKey_fp (f, 0);
//     }
//   else
//     {
//       // PEM encoded DSA private key

//       f = ACE_OS::fopen (filename, "r");

//       if (f == 0)
//         return 0;

//       const char *password = key.password.in ();

//       // Read PEM encoded DSA private key from a file, and convert it
//       // to OpenSSL's internal DSA format.
//       d = PEM_read_DSAPrivateKey (f,
//                                   0,
//                                   TAO_SSLIOP_passwd_callback,
//                                   password);
//     }

//   (void) ACE_OS::close (f);

//   return d;
// }
// #endif  /* !NO_DSA */
