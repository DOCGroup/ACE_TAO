// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_EVP_PKEY.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_EVP_PKEY_H
#define TAO_SSLIOP_EVP_PKEY_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_OpenSSL_st_T.h"

#include <openssl/evp.h>
#include <openssl/crypto.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    // OpenSSL @c EVP_PKEY structure traits specialization.
    template <>
    struct OpenSSL_traits< ::EVP_PKEY >
    {
      /// OpenSSL lock ID for use in OpenSSL CRYPTO_add() reference
      /// count manipulation function.
      enum { LOCK_ID = CRYPTO_LOCK_EVP_PKEY };

      /// Increase the reference count on the given OpenSSL structure.
      /**
       * @note This used to be in a function template but MSVC++ 6
       *       can't handle function templates correctly so reproduce
       *       the code in each specialization.  *sigh*
       */
      static ::EVP_PKEY * _duplicate (::EVP_PKEY * st)
      {
        if (st != 0)
          CRYPTO_add (&(st->references),
                      1,
                      LOCK_ID);

        return st;
      }

      /// Perform deep copy of the given OpenSSL structure.
      static ::EVP_PKEY * copy (::EVP_PKEY const & key);

      /// Decrease the reference count on the given OpenSSL
      /// structure.
      static void release (::EVP_PKEY * st)
      {
        ::EVP_PKEY_free (st);
      }
    };

    typedef OpenSSL_st_var< ::EVP_PKEY > EVP_PKEY_var;

  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_EVP_PKEY_H */
