// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_X509.h
 *
 *  $Id$
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

#include "SSLIOP_OpenSSL_st_T.h"

#include <openssl/x509.h>
#include <openssl/crypto.h>


namespace TAO
{
  namespace SSLIOP
  {
    // OpenSSL @c X509 structure traits specialization.
    template <>
    struct OpenSSL_traits< ::X509 >
    {
      /// OpenSSL lock ID for use in OpenSSL CRYPTO_add() reference
      /// count manipulation function.
      enum { LOCK_ID = CRYPTO_LOCK_X509 };

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


#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_X509_H */
