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

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_Credentials.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{
  namespace SSLIOP
  {
    class OwnCredentials;
    typedef OwnCredentials* OwnCredentials_ptr;
    typedef TAO_Pseudo_Var_T<OwnCredentials> OwnCredentials_var;
    typedef TAO_Pseudo_Out_T<OwnCredentials, OwnCredentials_var> OwnCredentials_out;

    /**
     * @class OwnCredentials
     *
     * @brief Credentials representing the our identity, not our
     *        peer's identity.
     *
     * @c OwnCredentials are a representation of our identity, not our
     * peer's identity.
     */
    class TAO_SSLIOP_Export OwnCredentials
      : public virtual SecurityLevel3::OwnCredentials,
        public virtual Credentials
    {
    public:

      /// Constructor
      OwnCredentials (::X509 *cert, ::EVP_PKEY *evp);

      static OwnCredentials_ptr _duplicate (OwnCredentials_ptr obj);
      static OwnCredentials_ptr _nil (void);
      static OwnCredentials_ptr _narrow (CORBA::Object_ptr obj
                                         ACE_ENV_ARG_DECL);

      /**
       * @name SecurityLevel3::TargetCredentials Methods
       *
       * Methods required by the SecurityLevel3::Credentials
       * interface.
       */
      //@{
      SecurityLevel3::CredentialsType creds_type (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
      //@}

      /**
       * @name SecurityLevel3::OwnCredentials Methods
       *
       * Methods required by the SecurityLevel3::OwnCredentials
       * interface.
       */
      //@{
      virtual SecurityLevel3::CredsInitiator_ptr creds_initiator (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::CredsAcceptor_ptr creds_acceptor (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void release_credentials (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
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


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_OWN_CREDENTIALS_H */
