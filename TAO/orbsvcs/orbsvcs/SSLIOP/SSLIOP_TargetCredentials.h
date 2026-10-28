// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_TargetCredentials.h
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_TARGET_CREDENTIALS_H
#define TAO_SSLIOP_TARGET_CREDENTIALS_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Credentials.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @class TargetCredentials
     *
     * @brief SSLIOP-specific implementation of the
     *        SecurityLevel3::TargetCredentials interface.
     *
     * This class implements SSLIOP-specific
     * SecurityLevel3::TargetCredentials.
     */
    class TAO_SSLIOP_Export TargetCredentials
      : public virtual SecurityLevel3::TargetCredentials,
        public virtual SSLIOP_Credentials
    {
    public:
      TargetCredentials (::X509 *cert, ::EVP_PKEY *evp);

      /**
       * @name SecurityLevel3::Credentials Methods
       *
       * Methods required by the SecurityLevel3::Credentials
       * interface.
       */
      //@{
      SecurityLevel3::CredentialsType creds_type () override;
      //@}

      /**
       * @name SecurityLevel3::TargetCredentials Methods
       *
       * Methods required by the SecurityLevel3::TargetCredentials
       * interface.
       */
      //@{
      char * context_id () override;

      SecurityLevel3::Principal * client_principal () override;

      SecurityLevel3::StatementList * client_supporting_statements () override;

      SecurityLevel3::ResourceNameList * client_restricted_resources () override;

      SecurityLevel3::Principal * target_principal () override;

      SecurityLevel3::StatementList * target_supporting_statements () override;

      SecurityLevel3::ResourceNameList * target_restricted_resources () override;

      SecurityLevel3::OwnCredentials_ptr parent_credentials () override;

      CORBA::Boolean client_authentication () override;

      CORBA::Boolean target_authentication () override;

      CORBA::Boolean confidentiality () override;

      CORBA::Boolean integrity () override;

      CORBA::Boolean target_embodied () override;

      CORBA::Boolean target_endorsed () override;

      void release () override;
      //@}
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_TARGET_CREDENTIALS_H */
