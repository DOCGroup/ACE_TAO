// -*- C++ -*-

//=============================================================================
/**
 * @file SSLIOP_OwnCredentials.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_OWN_CREDENTIALS_H
#define TAO_SSLIOP_OWN_CREDENTIALS_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Credentials.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    class OwnCredentials;
    typedef OwnCredentials* OwnCredentials_ptr;
    typedef TAO_Pseudo_Var_T<OwnCredentials> OwnCredentials_var;
    typedef TAO_Pseudo_Out_T<OwnCredentials> OwnCredentials_out;

    /**
     * @class OwnCredentials
     *
     * @brief Credentials representing our identity, not our peer's
     *        identity.
     *
     * @c OwnCredentials are a representation of our identity, not our
     * peer's identity.
     */
    class OwnCredentials
      : public virtual SecurityLevel3::OwnCredentials,
        public virtual SSLIOP_Credentials
    {
    public:
      typedef OwnCredentials_ptr _ptr_type;
      typedef OwnCredentials_var _var_type;
      typedef OwnCredentials_out _out_type;

      /// Constructor
      OwnCredentials (::X509 *cert, ::EVP_PKEY *evp);

      static OwnCredentials_ptr _duplicate (OwnCredentials_ptr obj);
      static OwnCredentials_ptr _nil (void);
      static OwnCredentials_ptr _narrow (CORBA::Object_ptr obj);

      /**
       * @name SecurityLevel3::TargetCredentials Methods
       *
       * Methods required by the SecurityLevel3::Credentials
       * interface.
       */
      //@{
      SecurityLevel3::CredentialsType creds_type (void);
      //@}

      /**
       * @name SecurityLevel3::OwnCredentials Methods
       *
       * Methods required by the SecurityLevel3::OwnCredentials
       * interface.
       */
      //@{
      virtual SecurityLevel3::CredsInitiator_ptr creds_initiator ();

      virtual SecurityLevel3::CredsAcceptor_ptr creds_acceptor ();

      virtual void release_credentials (void);
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~OwnCredentials (void);
    };

  } // End SSLIOP namespace
}  // End TAO namespace


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_OWN_CREDENTIALS_H */
