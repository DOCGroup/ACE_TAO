// -*- C++ -*-

//=============================================================================
/**
 * @file SL2_SecurityManager.h
 *
 * @author Chris Cleeland <cleeland@ociweb.com>
 */
//=============================================================================

#ifndef TAO_SL2_SECURITY_MANAGER_H
#define TAO_SL2_SECURITY_MANAGER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityC.h"
#include "orbsvcs/SecurityLevel2C.h"

#include "tao/LocalObject.h"
#include "tao/PortableServer/PS_ForwardC.h"

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  // would prefer SL2, but all the other SL2 stuff is in the Security namespace
  namespace Security
  {
    class AccessDecision
      : public virtual TAO::SL2::AccessDecision,
        public virtual ::CORBA::LocalObject
    {
    public:
      AccessDecision () = default;
      ~AccessDecision () override = default;

      AccessDecision (AccessDecision const &) = delete;
      AccessDecision (AccessDecision &&) = delete;
      AccessDecision &operator= (AccessDecision const &) = delete;
      AccessDecision &operator= (AccessDecision &&) = delete;

      ::CORBA::Boolean access_allowed (
        ::SecurityLevel2::CredentialsList const & cred_list,
        ::CORBA::Object_ptr target,
        char const * operation_name,
        char const * target_interface_name) override;

      ::CORBA::Boolean access_allowed_ex (
          char const * orb_id,
          ::CORBA::OctetSeq const & adapter_id,
          ::CORBA::OctetSeq const & object_id,
          ::SecurityLevel2::CredentialsList const & cred_list,
          char const * operation_name,
          ::CORBA::Boolean collocated_invocation) override;

      ::CORBA::Boolean default_decision () override;
      void default_decision (::CORBA::Boolean d) override;

      ::CORBA::Boolean default_collocated_decision () override;
      void default_collocated_decision (::CORBA::Boolean d) override;

      void add_object (char const * orbid,
                       ::CORBA::OctetSeq const & adapter_id,
                       ::CORBA::OctetSeq const & object_id,
                       ::CORBA::Boolean allow_insecure_access) override;

      void remove_object (char const * orbid,
                          ::CORBA::OctetSeq const & adapter_id,
                          ::CORBA::OctetSeq const & object_id) override;

    private:
      /*!
       * This is the default value that's returned from access_allowed()
       * when the access table doesn't contain an entry for the reference.
       */
      ::CORBA::Boolean default_allowance_decision_ {};

      /*!
       * This is the default value that's returned from access_allowed()
       * when the access table doesn't contain an entry for the reference and
       * we are handling a collocated call
       */
      ::CORBA::Boolean default_collocated_decision_ {};
      /*!
       * Map containing references and their designated insecure access.
       */
      struct ReferenceKeyType
      {
        PortableServer::ObjectId_var oid_;
        CORBA::OctetSeq_var adapter_id_;
        CORBA::String_var orbid_;

        bool operator== (ReferenceKeyType const & other) const;
        CORBA::ULong hash () const;

        operator char const * () const;
      };
      using OBJECT_KEY = ReferenceKeyType;
      using ACCESS_MAP_TYPE = ACE_Hash_Map_Manager_Ex<OBJECT_KEY,
                                                       CORBA::Boolean, // access_allowed?
                                                       ACE_Hash<OBJECT_KEY>,
                                                       ACE_Equal_To<OBJECT_KEY>,
                                                       ACE_Null_Mutex>;

      ACCESS_MAP_TYPE access_map_;

      // Protects access_map_.
      TAO_SYNCH_MUTEX map_lock_;

    private:
      /*!
       * @brief Encapsulates a TAO-specific way to do object_to_string() without having an ORB reference handy.
       *
       * @note If OBJECT_KEY changes as described above, this should change
       * so that it generates an OBJECT_KEY.
       */
      OBJECT_KEY map_key_from_objref (CORBA::Object_ptr obj);

      /// Shared implementation for access_allowed and access_allowed_ex.
      ::CORBA::Boolean access_allowed_i (OBJECT_KEY & key,
                                         char const * operation_name,
                                         CORBA::Boolean collocated = false);
    };

    class SecurityManager
      : public virtual SecurityLevel2::SecurityManager,
        public virtual ::CORBA::LocalObject
    {
    public:
      SecurityManager ();

      SecurityManager (SecurityManager const &) = delete;
      SecurityManager (SecurityManager &&) = delete;
      SecurityManager &operator= (SecurityManager const &) = delete;
      SecurityManager &operator= (SecurityManager &&) = delete;

      /**
       * @name SecurityLevel2::SecurityManager Methods
       *
       * Methods required by the SecurityLevel2::SecurityManager
       * interface.
       */
      //@{
      ::Security::MechandOptionsList * supported_mechanisms () override;
      SecurityLevel2::CredentialsList * own_credentials () override;
      SecurityLevel2::RequiredRights_ptr required_rights_object () override;
      SecurityLevel2::PrincipalAuthenticator_ptr principal_authenticator () override;
      SecurityLevel2::AccessDecision_ptr access_decision () override;
      SecurityLevel2::AuditDecision_ptr audit_decision () override;
      SecurityLevel2::TargetCredentials_ptr get_target_credentials (CORBA::Object_ptr o) override;
      void remove_own_credentials (SecurityLevel2::Credentials_ptr creds) override;
      CORBA::Policy_ptr get_security_policy (CORBA::PolicyType policy_type) override;
      //@}

    protected:
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~SecurityManager () override = default;

    private:
      /// The ORB-specific SecurityLevel2::PrincipalAuthenticator
      /// reference.
      // Except we're not going to have one of these right now
      SecurityLevel2::PrincipalAuthenticator_var principal_authenticator_;

      // AccessDecision instance
      SecurityLevel2::AccessDecision_var access_decision_;
    };
  } // End SL3 namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SL2_SECURITY_MANAGER_H */
