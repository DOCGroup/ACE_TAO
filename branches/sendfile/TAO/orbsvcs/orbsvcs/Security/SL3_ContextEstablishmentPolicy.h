// -*- C++ -*-

//=============================================================================
/**
 * @file SL3_ContextEstablishmentPolicy.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SL3_CONTEXT_ESTABLISHMENT_POLICY_H
#define TAO_SL3_CONTEXT_ESTABLISHMENT_POLICY_H

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
     * @class ContextEstablishmentPolicy
     *
     * @brief
     *
     *
     */
    class ContextEstablishmentPolicy
      : public virtual SecurityLevel3::ContextEstablishmentPolicy,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor
      ContextEstablishmentPolicy (
        SecurityLevel3::CredsDirective             creds_directive,
        const SecurityLevel3::OwnCredentialsList & creds_list,
        SecurityLevel3::FeatureDirective           use_client_auth,
        SecurityLevel3::FeatureDirective           use_target_auth,
        SecurityLevel3::FeatureDirective           use_confidentiality,
        SecurityLevel3::FeatureDirective           use_integrity);

      /**
       * @name SecurityLevel3::ContextEstablishmentPolicy Methods
       *
       * Methods required by the
       * SecurityLevel3::ContextEstablishmentPolicy interface.
       */
      //@{
      virtual SecurityLevel3::CredsDirective creds_directive (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::OwnCredentialsList * creds_list (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::FeatureDirective use_client_auth (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::FeatureDirective use_target_auth (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::FeatureDirective use_confidentiality (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual SecurityLevel3::FeatureDirective use_integrity (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~ContextEstablishmentPolicy (void);

    private:

      SecurityLevel3::CredsDirective     creds_directive_;
      SecurityLevel3::OwnCredentialsList creds_list_;
      SecurityLevel3::FeatureDirective   use_client_auth_;
      SecurityLevel3::FeatureDirective   use_target_auth_;
      SecurityLevel3::FeatureDirective   use_confidentiality_;
      SecurityLevel3::FeatureDirective   use_integrity_;

    };

  } // End SL3 namespace

}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SL3_CONTEXT_ESTABLISHMENT_POLICY_H */
