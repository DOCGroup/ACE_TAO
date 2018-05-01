// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_SSL.h
 *
 *  OpenSSL @c SSL data structure specializations and typedefs.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_SSL_H
#define TAO_SSLIOP_SSL_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_OpenSSL_st_T.h"

#include <openssl/ssl.h>
#include <openssl/crypto.h>


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    // OpenSSL @c SSL structure traits specialization.
    template <>
    struct OpenSSL_traits< ::SSL >
    {
      /// Increase the reference count on the given OpenSSL structure.
      /**
       * @note This used to be in a function template but MSVC++ 6
       *       can't handle function templates correctly so reproduce
       *       the code in each specialization.  *sigh*
       */
      static ::SSL * _duplicate (::SSL * st)
      {
        if (st != 0)
        {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
          ::SSL_up_ref(st);
#else
          CRYPTO_add (&(st->references),
                      1,
                      CRYPTO_LOCK_SSL);
#endif
        }

        return st;
      }

      /// Perform deep copy of the given OpenSSL structure.
      static ::SSL * copy (::SSL const & st)
      {
        return ::SSL_dup (const_cast< ::SSL * > (&st));
      }

      /// Decrease the reference count on the given OpenSSL
      /// structure.
      static void release (::SSL * st)
      {
        ::SSL_free (st);
      }
    };

    typedef OpenSSL_st_var< ::SSL > SSL_var;

  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_SSL_H */
