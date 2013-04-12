// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/SSLIOP/SSLIOP_CredentialsAcquirer.h"
#include "orbsvcs/SSLIOP/SSLIOP_OwnCredentials.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/SSL/SSL_Context.h"

#include "ace/OS_NS_stdio.h"

#include <openssl/x509.h>
#include <openssl/pem.h>

// -------------------------------------------------------

#if (defined (TAO_HAS_VERSIONED_NAMESPACE) && TAO_HAS_VERSIONED_NAMESPACE == 1)
# define TAO_SSLIOP_PASSWORD_CALLBACK_NAME ACE_PREPROC_CONCATENATE(TAO_VERSIONED_NAMESPACE_NAME, _TAO_SSLIOP_password_callback)
#else
# define TAO_SSLIOP_PASSWORD_CALLBACK_NAME TAO_SSLIOP_password_callback
#endif  /* TAO_HAS_VERSIONED_NAMESPACE == 1 */


extern "C"
int
TAO_SSLIOP_PASSWORD_CALLBACK_NAME (char *buf,
                                   int size,
                                   int /* rwflag */,
                                   void *userdata)
{
  // @@ I'm probably over complicating this implementation, but that's
  //    what you get when you try to be overly efficient.  :-)
  //        -Ossama

  const char * password = static_cast<char *> (userdata);

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

// -------------------------------------------------------

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::CredentialsAcquirer::CredentialsAcquirer (
   TAO::SL3::CredentialsCurator_ptr curator,
   const CORBA::Any & acquisition_arguments)
  : lock_ (),
    curator_ (TAO::SL3::CredentialsCurator::_duplicate (curator)),
    acquisition_arguments_ (acquisition_arguments),
    destroyed_ (false)
{
}

TAO::SSLIOP::CredentialsAcquirer::~CredentialsAcquirer (void)
{
}

char *
TAO::SSLIOP::CredentialsAcquirer::acquisition_method (void)
{
  this->check_validity ();

  return CORBA::string_dup ("SL3TLS");
}

SecurityLevel3::AcquisitionStatus
TAO::SSLIOP::CredentialsAcquirer::current_status (void)
{
  this->check_validity ();

  return SecurityLevel3::AQST_Succeeded;  // @@ Really?
}

CORBA::ULong
TAO::SSLIOP::CredentialsAcquirer::nth_iteration (void)
{
  this->check_validity ();

  // SSL/TLS credentials is single-step process from the point-of-view
  // of the caller.
  return 1;
}

CORBA::Any *
TAO::SSLIOP::CredentialsAcquirer::get_continuation_data ()
{
  // SSL/TLS credentials acquisition does generate continuation data.
  throw CORBA::BAD_INV_ORDER ();
}

SecurityLevel3::AcquisitionStatus
TAO::SSLIOP::CredentialsAcquirer::continue_acquisition (
    const CORBA::Any & /* acquisition_arguments */)
{
  // SSL/TLS credentials acquisition does generate continuation data.
  throw CORBA::BAD_INV_ORDER ();
}

SecurityLevel3::OwnCredentials_ptr
TAO::SSLIOP::CredentialsAcquirer::get_credentials (CORBA::Boolean on_list)
{
  this->check_validity ();

  ::SSLIOP::AuthData *data = 0;

  if (!(this->acquisition_arguments_ >>= data))
    throw CORBA::BAD_PARAM ();

  TAO::SSLIOP::X509_var x509 = this->make_X509 (data->certificate);

  if (x509.in () == 0)
    throw CORBA::BAD_PARAM ();

  TAO::SSLIOP::EVP_PKEY_var evp = this->make_EVP_PKEY (data->key);

  if (evp.in () == 0)
    throw CORBA::BAD_PARAM ();

  // Verify that the private key is consistent with the certificate.
  if (::X509_check_private_key (x509.in (), evp.in ()) != 1)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ERROR: Private key is not ")
                    ACE_TEXT ("consistent with X.509 certificate")));

      throw CORBA::BAD_PARAM ();
    }

  TAO::SSLIOP::OwnCredentials * creds = 0;
  ACE_NEW_THROW_EX (creds,
                    TAO::SSLIOP::OwnCredentials (x509.in (), evp.in ()),
                    CORBA::NO_MEMORY ());

  SecurityLevel3::OwnCredentials_var credentials = creds;

  if (on_list)
    {
      this->curator_->_tao_add_own_credentials (creds);
    }

  this->destroy ();

  return credentials._retn ();
}

void
TAO::SSLIOP::CredentialsAcquirer::destroy (void)
{
  this->check_validity ();

  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  if (!this->destroyed_)
    {
      this->destroyed_ = true;

      // Release our reference to the CredentialsCurator.
      (void) this->curator_.out ();
    }
}

void
TAO::SSLIOP::CredentialsAcquirer::check_validity (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  if (this->destroyed_)
    throw CORBA::BAD_INV_ORDER ();
}

::X509 *
TAO::SSLIOP::CredentialsAcquirer::make_X509 (const ::SSLIOP::File &certificate)
{
  // No password is used or needed when reading ASN.1 encoded
  // certificates.

  const char *filename = certificate.filename.in ();

  if (filename == 0)
    return 0;

  FILE *fp = 0;
  ::X509 *x = 0;

  if (certificate.type == ::SSLIOP::ASN1)
    {
      // ASN.1/DER encoded certificate

      // No password is used or needed when reading ASN.1 encoded
      // certificates.

      const char *filename = certificate.filename.in ();

      if (filename == 0)
        return 0;

      fp = ACE_OS::fopen (filename, "rb");

      if (fp == 0)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) SSLIOP::CredentialsAcquirer::make_X509 - %p\n"),
                        ACE_TEXT ("fopen")));

          return 0;
        }

      // Read ASN.1 / DER encoded X.509 certificate from a file, and
      // convert it to OpenSSL's internal X.509 format.
      x = ::d2i_X509_fp (fp, 0);
    }
  else
    {
      // PEM encoded certificate

      fp = ACE_OS::fopen (filename, "r");

      if (fp == 0)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) SSLIOP::CredentialsAcquirer::make_X509 - %p\n"),
                        ACE_TEXT ("fopen")));

          return 0;
        }

      const char *password = certificate.password.in ();

      // Read PEM encoded X.509 certificate from a file, and convert
      // it to OpenSSL's internal X.509 format.
      x = PEM_read_X509 (fp,
                         0,
                         TAO_SSLIOP_PASSWORD_CALLBACK_NAME,
                         const_cast<char *> (password));
    }

  (void) ACE_OS::fclose (fp);

  if (x == 0 && TAO_debug_level > 0)
    ACE_SSL_Context::report_error ();

  return x;
}

::EVP_PKEY *
TAO::SSLIOP::CredentialsAcquirer::make_EVP_PKEY (const ::SSLIOP::File &key)
{
  // No password is used or needed when reading ASN.1 encoded
  // private keys.

  const char *filename = key.filename.in ();

  if (filename == 0)
    return 0;

  FILE *fp = 0;
  ::EVP_PKEY *evp = 0;

  if (key.type == ::SSLIOP::ASN1)
    {
      // ASN.1/DER encoded private key

      // No password is used or needed when reading ASN.1 encoded
      // private keys.

      const char *filename = key.filename.in ();

      if (filename == 0)
        return 0;

      fp = ACE_OS::fopen (filename, "rb");

      if (fp == 0)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) SSLIOP::CredentialsAcquirer::make_EVP_PKEY ")
                        ACE_TEXT ("- %p\n"),
                        ACE_TEXT ("fopen")));

          return 0;
        }

      // Read ASN.1 / DER encoded private key from a file, and convert
      // it to OpenSSL's internal private key format.
      evp = ::d2i_PrivateKey_fp (fp, 0);
    }
  else
    {
      // PEM encoded private key

      fp = ACE_OS::fopen (filename, "r");

      if (fp == 0)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) SSLIOP::CredentialsAcquirer::make_EVP_PKEY ")
                        ACE_TEXT ("- %p\n"),
                        ACE_TEXT ("fopen")));

          return 0;
        }

      const char *password = key.password.in ();

      // Read PEM encoded private key from a file, and convert it to
      // OpenSSL's internal private key format.
      evp = PEM_read_PrivateKey (fp,
                                 0,
                                 TAO_SSLIOP_PASSWORD_CALLBACK_NAME,
                                 const_cast<char *> (password));
    }

  (void) ACE_OS::fclose (fp);

  if (evp == 0 && TAO_debug_level > 0)
    ACE_SSL_Context::report_error ();

  return evp;
}

TAO_END_VERSIONED_NAMESPACE_DECL
