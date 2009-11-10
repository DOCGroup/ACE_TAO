// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_Credentials.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_CREDENTIALS_H
#define TAO_SSLIOP_CREDENTIALS_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_X509.h"
#include "orbsvcs/SSLIOP/SSLIOP_EVP_PKEY.h"
#include "orbsvcs/SecurityLevel3C.h"
#include "tao/LocalObject.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class SSLIOP_Credentials;

   namespace SSLIOP
   {
     typedef SSLIOP_Credentials* Credentials_ptr;
     typedef TAO_Pseudo_Var_T<SSLIOP_Credentials> Credentials_var;
     typedef TAO_Pseudo_Out_T<SSLIOP_Credentials> Credentials_out;
   }

    /**
     * @class SSLIOP_Credentials
     *
     * @brief SSLIOP-specific implementation of the
     *        SecurityLevel3::Credentials interface.
     *
     * This class encapsulates the X.509 certificate associated with a
     * given a principal.
     *
     * @note Why is this class not the TAO::SSLIOP namespace?  Because
     *       brain damaged MSVC++ 6 cannot call a base class
     *       constructor of class declared in a namespace that is more
     *       than one level deep in a sub-class base member
     *       initializer list.
     */
    class TAO_SSLIOP_Export SSLIOP_Credentials
      : public virtual SecurityLevel3::Credentials,
        public virtual ::CORBA::LocalObject
    {
    public:
      typedef SSLIOP::Credentials_ptr _ptr_type;
      typedef SSLIOP::Credentials_var _var_type;
      typedef SSLIOP::Credentials_out _out_type;

      /// Constructor
      SSLIOP_Credentials (::X509 * cert, ::EVP_PKEY * evp);

      /**
       * @name SecurityLevel3::Credentials Methods
       *
       * Methods required by the SecurityLevel3::Credentials
       * interface.
       */
      //@{
      virtual char * creds_id (void);

      virtual SecurityLevel3::CredentialsType creds_type (void) = 0;

      virtual SecurityLevel3::CredentialsUsage creds_usage ();

      virtual TimeBase::UtcT expiry_time (void);

      virtual SecurityLevel3::CredentialsState creds_state ();

      virtual char * add_relinquished_listener (
          SecurityLevel3::RelinquishedCredentialsListener_ptr listener);

      virtual void remove_relinquished_listener (const char * id);
      //@}


      //@{
      /// Return a pointer to the underlying X.509 certificate.
      /**
       * @note Caller owns the returned object.  Use a
       *       TAO::SSLIOP::X509_var.
       */
      ::X509 *x509 (void);
      //@}

      /// Return a pointer to the underlying private key.
      /**
       * @return Non-zero value if private key is used.
       *
       * @note Caller owns the returned object.  Use a
       *       TAO::SSLIOP::EVP_PKEY_var.
       */
      ::EVP_PKEY *evp (void);
      //@}

      bool operator== (const SSLIOP_Credentials &rhs);

      CORBA::ULong hash (void) const;

      // The static operations.
      static SSLIOP::Credentials_ptr _duplicate (SSLIOP::Credentials_ptr obj);

      static SSLIOP::Credentials_ptr _narrow (CORBA::Object_ptr obj);

      static SSLIOP::Credentials_ptr _nil (void)
      {
        return (SSLIOP::Credentials_ptr) 0;
      }

      //@}

    protected:

      /// Destructor.
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting  mechanism.
       */
      ~SSLIOP_Credentials (void);

    protected:

      /// Reference to the X.509 certificate associated with this SSLIOP
      /// Credentials object.
      SSLIOP::X509_var x509_;

      /// Reference to the private key associated with the X.509
      /// certificate.
      SSLIOP::EVP_PKEY_var evp_;

      /// Credentials Identifier.
      CORBA::String_var id_;

      /// The intended usage of the Credentials.
      SecurityLevel3::CredentialsUsage creds_usage_;

      /// The time these Credentials expire.
      TimeBase::UtcT expiry_time_;

      /// The validity of the Credentials.
      SecurityLevel3::CredentialsState creds_state_;

    };

//   } // End SSLIOP namespace
}   // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "orbsvcs/SSLIOP/SSLIOP_Credentials.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CREDENTIALS_H */
