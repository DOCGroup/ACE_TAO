// -*- C++ -*-

//=============================================================================
/**
 * @file SSLIOP_ClientCredentials.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_CLIENT_CREDENTIALS_H
#define TAO_SSLIOP_CLIENT_CREDENTIALS_H

#include /**/ "ace/pre.h"

#include "orbsvcs/orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/SSLIOP/SSLIOP_Credentials.h"

#include "orbsvcs/orbsvcs/SecurityLevel3C.h"

#include "tao/LocalObject.h"

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
    /**
     * @class ClientCredentials
     *
     * @brief
     *
     *
     */
    class TAO_SSLIOP_Export ClientCredentials
      : public virtual SecurityLevel3::ClientCredentials,
        public virtual Credentials
    {
    public:

      /// Constructor
      ClientCredentials (::X509 *cert, ::EVP_PKEY *evp);

      /**
       * @name SecurityLevel3::Credentials Methods
       *
       * Methods required by the SecurityLevel3::Credentials
       * interface.
       */
      //@{
      virtual SecurityLevel3::CredentialsType creds_type (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
      //@}

      /**
       * @name SecurityLevel3::ClientCredentials Methods
       *
       * Methods required by the SecurityLevel3::ClientCredentials
       * interface.
       */
      //@{
      virtual char * context_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::Principal * client_principal (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::StatementList * client_supporting_statements (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::ResourceNameList * client_restricted_resources (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::Principal * target_principal (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::StatementList * target_supporting_statements (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::ResourceNameList * target_restricted_resources (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::OwnCredentials_ptr parent_credentials (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::Boolean client_authentication (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::Boolean target_authentication (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::Boolean confidentiality (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::Boolean integrity (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~ClientCredentials (void);

    };

  } // End SSLIOP namespace
}  // End TAO namespace


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CLIENT_CREDENTIALS_H */
