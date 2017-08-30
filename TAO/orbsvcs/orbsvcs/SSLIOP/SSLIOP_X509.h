// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_X509.h
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_X509_H
#define TAO_SSLIOP_X509_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_OpenSSL_st_T.h"

#include <openssl/x509.h>
#include <openssl/crypto.h>


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    // OpenSSL @c X509 structure traits specialization.
    template <>
    struct OpenSSL_traits< ::X509 >
    {
      /// Increase the reference count on the given OpenSSL structure.
      /**
       * @note This used to be in a function template but MSVC++ 6
       *       can't handle function templates correctly so reproduce
       *       the code in each specialization.  *sigh*
       */
      static ::X509 * _duplicate (::X509 * st)
      {
        if (st != 0)
        {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
          ::X509_up_ref(st);
#else
          CRYPTO_add (&(st->references),
                      1,
                      CRYPTO_LOCK_X509);
#endif
        }

        return st;
      }

      /// Perform deep copy of the given OpenSSL structure.
      static ::X509 * copy (::X509 const & st)
      {
        return ::X509_dup (const_cast< ::X509 *> (&st));
      }

      /// Decrease the reference count on the given OpenSSL
      /// structure.
      static void release (::X509 * st)
      {
        ::X509_free (st);
      }
    };

    typedef OpenSSL_st_var< ::X509 > X509_var;

  }  // End SSLIOP namespace.
}  // End TAO namespace.


TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_X509_H */
