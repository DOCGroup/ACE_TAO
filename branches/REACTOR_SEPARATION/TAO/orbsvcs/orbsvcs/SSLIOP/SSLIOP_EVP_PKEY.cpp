// -*- C++ -*-

#include "SSLIOP_EVP_PKEY.h"

#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>
#include "params_dup.h"


ACE_RCSID (SSLIOP,
           SSLIOP_EVP_PKEY,
           "$Id$")


::EVP_PKEY *
TAO::SSLIOP::OpenSSL_traits< ::EVP_PKEY >::copy (::EVP_PKEY const & key)
{
  ::EVP_PKEY * pkey = const_cast< ::EVP_PKEY *> (&key);

  // We're using the EVP_PKEY_var even though it depends on this
  // trait function.  This works since we're not actually using
  // any of the EVP_PKEY_var methods that call this copy()
  // trait.  This allows us to maintain exception safety.
  TAO::SSLIOP::EVP_PKEY_var p = ::EVP_PKEY_new ();

  switch (::EVP_PKEY_type (pkey->type))
    {
    case EVP_PKEY_RSA:
      {
        RSA * rsa = ::EVP_PKEY_get1_RSA (pkey);
        if (rsa != 0)
          {
            // Not exception safe!
            ::EVP_PKEY_set1_RSA (p.in (), RSAPrivateKey_dup (rsa));
            ::RSA_free (rsa);
          }
      }
      break;

    case EVP_PKEY_DSA:
      {
        DSA * dsa = ::EVP_PKEY_get1_DSA (pkey);
        if (dsa != 0)
          {
            // Not exception safe!
            ::EVP_PKEY_set1_DSA (p.in (), DSAparams_dup_wrapper (dsa));
            ::DSA_free (dsa);
          }
      }
      break;

    case EVP_PKEY_DH:
      {
        DH * dh = ::EVP_PKEY_get1_DH (pkey);
        if (dh != 0)
          {
            // Not exception safe!
            ::EVP_PKEY_set1_DH (p.in (), DHparams_dup_wrapper (dh));
            ::DH_free (dh);
          }
      }
      break;

    default:
      // We should never get here!
      return 0;
    }

  return p._retn ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::SSLIOP::OpenSSL_st_var< ::EVP_PKEY >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO::SSLIOP::OpenSSL_st_var< ::EVP_PKEY >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
