// -*- C++ -*-

//=============================================================================
/**
 * @file SL3_SecurityManager.h
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SL3_SECURITY_MANAGER_H
#define TAO_SL3_SECURITY_MANAGER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel3C.h"

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SL3
  {
    /**
     * @class SecurityManager
     *
     * @brief
     *
     *
     */
    class SecurityManager
      : public virtual SecurityLevel3::SecurityManager,
        public virtual ::CORBA::LocalObject
    {
    public:

      /// Constructor
      SecurityManager (SecurityLevel3::CredentialsCurator_ptr cc);

      /**
       * @name SecurityLevel3::SecurityManager Methods
       *
       * Methods required by the SecurityLevel3::SecurityManager
       * interface.
       */
      //@{
      virtual SecurityLevel3::CredentialsCurator_ptr credentials_curator ();

      virtual SecurityLevel3::TargetCredentials_ptr get_target_credentials (
          CORBA::Object_ptr the_object);

      virtual SecurityLevel3::ContextEstablishmentPolicy_ptr
      create_context_estab_policy (
          SecurityLevel3::CredsDirective creds_directive,
          const SecurityLevel3::OwnCredentialsList & creds_list,
          SecurityLevel3::FeatureDirective use_client_auth,
          SecurityLevel3::FeatureDirective use_target_auth,
          SecurityLevel3::FeatureDirective use_confidentiality,
          SecurityLevel3::FeatureDirective use_integrity);

      virtual SecurityLevel3::ObjectCredentialsPolicy_ptr
      create_object_creds_policy (
          const SecurityLevel3::OwnCredentialsList & cred_list);
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      virtual ~SecurityManager (void);

    private:

      /// The ORB-specific SecurityLevel3::CredentialsCurator
      /// reference.
      SecurityLevel3::CredentialsCurator_var credentials_curator_;

    };

  } // End SL3 namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SL3_SECURITY_MANAGER_H */
