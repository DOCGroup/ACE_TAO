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

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_X509.h"
#include "SSLIOP_EVP_PKEY.h"
#include "orbsvcs/orbsvcs/SecurityLevel3C.h"
#include "tao/LocalObject.h"

namespace TAO
{
  namespace SSLIOP
  {

    class Credentials;
    typedef Credentials* Credentials_ptr;
    typedef TAO_Pseudo_Var_T<Credentials> Credentials_var;
    typedef TAO_Pseudo_Out_T<Credentials, Credentials_var> Credentials_out;

    /**
     * @class Credentials
     *
     * @brief SSLIOP-specific implementation of the
     *        SecurityLevel3::Credentials interface.
     *
     * This class encapsulates the X.509 certificate associated with a
     * given a principal.
     */
    class TAO_SSLIOP_Export Credentials
      : public virtual SecurityLevel3::Credentials,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor
      Credentials (::X509 * cert, ::EVP_PKEY * evp);

      /**
       * @name SecurityLevel3::Credentials Methods
       *
       * Methods required by the SecurityLevel3::Credentials
       * interface.
       */
      //@{
      virtual char * creds_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual
      SecurityLevel3::CredentialsType creds_type (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

      virtual SecurityLevel3::CredentialsUsage creds_usage (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual TimeBase::UtcT expiry_time (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::CredentialsState creds_state (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual char * add_relinquished_listener (
          SecurityLevel3::RelinquishedCredentialsListener_ptr listener
          ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void remove_relinquished_listener (const char * id
                                                 ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
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

      bool operator== (const Credentials &rhs);

      CORBA::ULong hash (void) const;

      // The static operations.
      static Credentials_ptr _duplicate (Credentials_ptr obj);

      static Credentials_ptr _narrow (CORBA::Object_ptr obj
                                      ACE_ENV_ARG_DECL);

      static Credentials_ptr _nil (void)
      {
        return (Credentials_ptr) 0;
      }

      //@}

    protected:

      /// Destructor.
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting  mechanism.
       */
      ~Credentials (void);

    protected:

      /// Reference to the X.509 certificate associated with this SSLIOP
      /// Credentials object.
      X509_var x509_;

      /// Reference to the private key associated with the X.509
      /// certificate.
      EVP_PKEY_var evp_;

      /// Credentials Identifier.
      CORBA::String_var id_;

      /// The intended usage of the Credentials.
      SecurityLevel3::CredentialsUsage creds_usage_;

      /// The time these Credentials expire.
      TimeBase::UtcT expiry_time_;

      /// The validity of the Credentials.
      SecurityLevel3::CredentialsState creds_state_;

    };

  } // End SSLIOP namespace
}   // End TAO namespace

#if defined (__ACE_INLINE__)
# include "SSLIOP_Credentials.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CREDENTIALS_H */
