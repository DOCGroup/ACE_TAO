// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_TargetCredentials.h
 *
 *  $Id$
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
      virtual SecurityLevel3::CredentialsType creds_type ();
      //@}

      /**
       * @name SecurityLevel3::TargetCredentials Methods
       *
       * Methods required by the SecurityLevel3::TargetCredentials
       * interface.
       */
      //@{
      virtual char * context_id (void);

      virtual SecurityLevel3::Principal * client_principal ();

      virtual SecurityLevel3::StatementList * client_supporting_statements ();

      virtual SecurityLevel3::ResourceNameList * client_restricted_resources ();

      virtual SecurityLevel3::Principal * target_principal ();

      virtual SecurityLevel3::StatementList * target_supporting_statements ();

      virtual SecurityLevel3::ResourceNameList * target_restricted_resources ();

      virtual SecurityLevel3::OwnCredentials_ptr parent_credentials ();

      virtual CORBA::Boolean client_authentication (void);

      virtual CORBA::Boolean target_authentication (void);

      virtual CORBA::Boolean confidentiality (void);

      virtual CORBA::Boolean integrity (void);

      virtual CORBA::Boolean target_embodied (void);

      virtual CORBA::Boolean target_endorsed (void);

      virtual void release (void);
      //@}

    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_TARGET_CREDENTIALS_H */
