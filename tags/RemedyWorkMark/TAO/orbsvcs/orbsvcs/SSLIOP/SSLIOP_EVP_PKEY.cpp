// -*- C++ -*-
// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_EVP_PKEY.h"

#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>
#include "orbsvcs/SSLIOP/params_dup.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
            ::EVP_PKEY_set1_DSA (p.in (), DSAPARAMS_DUP_WRAPPER_NAME (dsa));
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
            ::EVP_PKEY_set1_DH (p.in (), DHPARAMS_DUP_WRAPPER_NAME (dh));
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

TAO_END_VERSIONED_NAMESPACE_DECL
