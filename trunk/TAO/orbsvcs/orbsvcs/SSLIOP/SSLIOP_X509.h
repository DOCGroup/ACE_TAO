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

#include "tao/varbase.h"

#include <openssl/x509.h>
#include <openssl/crypto.h>

namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @name CORBA-style Reference Count Manipulation Methods
     */
    /// Increase the reference count on the given X509 structure.
    ::X509 *_duplicate (::X509 *cert);

    /// Decrease the reference count on the given X509 structure.
    void release (::X509 *cert);

    /**
     * @class X509_var
     *
     * @brief "_var" class for the OpenSSL @param X509 structure.
     *
     * This class is simply used to make operations on instances of
     * the OpenSSL @c X509 structures exception safe.  It is only used
     * internally by the SSLIOP pluggable protocol.
     */
    class X509_var : private TAO_Base_var
    {
    public:

      /**
       * @name Constructors
       */
      //@{
      X509_var (void);
      X509_var (::X509 *x);
      X509_var (const X509_var &);
      X509_var (const ::X509 &x);
      //@}

      /// Destructor
      ~X509_var (void);

      X509_var &operator= (::X509 *);
      X509_var &operator= (const X509_var &);
      X509_var &operator= (const ::X509 &);
      const ::X509 *operator-> (void) const;
      ::X509 *operator-> (void);

      operator const ::X509 &() const;
      operator ::X509 &();

      ::X509 *in (void) const;
      ::X509 *&inout (void);
      ::X509 *&out (void);
      ::X509 *_retn (void);
      ::X509 *ptr (void) const;

    private:

      // Unimplemented - prevents widening assignment.
      X509_var (const TAO_Base_var &rhs);
      X509_var &operator= (const TAO_Base_var &rhs);

    private:

      /// The OpenSSL X509 structure that represents a X.509 certificate.
      ::X509 *x509_;

    };
  }  // End SSLIOP namespace.
}  // End TAO namespace.


#if defined (__ACE_INLINE__)
#include "SSLIOP_X509.inl"
#endif  /* __ACE_INLINE__ */


#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_X509_H */
