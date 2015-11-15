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
    // This should move out of here probably, but it's easier to stick it
    // here for the moment...(CJC)
    /**
     * @class AccessDecision
     *
     * @brief
     */
    class AccessDecision
      : public virtual TAO::SL2::AccessDecision,
  public virtual ::CORBA::LocalObject
    {
    public:
      /*! Constructor */
      AccessDecision (void);
      ~AccessDecision (void);

      virtual ::CORBA::Boolean access_allowed (
        const ::SecurityLevel2::CredentialsList & cred_list,
        ::CORBA::Object_ptr target,
        const char * operation_name,
        const char * target_interface_name);

      virtual ::CORBA::Boolean access_allowed_ex (
          const char * orb_id,
          const ::CORBA::OctetSeq & adapter_id,
          const ::CORBA::OctetSeq & object_id,
          const ::SecurityLevel2::CredentialsList & cred_list,
          const char * operation_name,
          ::CORBA::Boolean collocated_invocation);

      virtual ::CORBA::Boolean default_decision (void);
      virtual void default_decision (::CORBA::Boolean d);

      virtual ::CORBA::Boolean default_collocated_decision (void);
      virtual void default_collocated_decision (::CORBA::Boolean d);

      virtual void add_object (const char * orbid,
                               const ::CORBA::OctetSeq & adapter_id,
                               const ::CORBA::OctetSeq & object_id,
                               ::CORBA::Boolean allow_insecure_access);

      virtual void remove_object (const char * orbid,
                                  const ::CORBA::OctetSeq & adapter_id,
                                  const ::CORBA::OctetSeq & object_id);

    private:
      /*!
       * This is the default value that's returned from access_allowed()
       * when the access table doesn't contain an entry for the reference.
       */
      ::CORBA::Boolean default_allowance_decision_;

      /*!
       * This is the default value that's returned from access_allowed()
       * when the access table doesn't contain an entry for the reference and
       * we are handling a collocated call
       */
      ::CORBA::Boolean default_collocated_decision_;
      /*!
       * Map containing references and their designated insecure access.
       */
      // What sorts of maps are available in ACE?  We'll be mapping
      // an object reference to a boolean, basically.  Looks like for
      // now we'll map a stringified IOR to the boolean, and provide some
      // (for now) simple keys and functions for comparing them.
      //
      // Locking on this needs to be exclusive to add_object,
      // remove_object, and access_allowed.  I think that the lock on the
      // map itself will be sufficient, but we'll model this after the
      // Active Object map in the POA...so whatever way that goes, so, too,
      // will this.
      struct ReferenceKeyType
      {
        PortableServer::ObjectId_var oid_;
        CORBA::OctetSeq_var adapter_id_;
        CORBA::String_var orbid_;

        // operations/methods necessary for functors in HashMap; might
        // need to add operator< if we decide to use an RB_Tree
        bool operator== (const ReferenceKeyType& other) const;
        CORBA::ULong hash() const;

        // operator kind of like a "toString()" for debug statements
        operator const char * () const;
      };
      typedef ReferenceKeyType OBJECT_KEY;
      // This is typedef'd because we might try to do something fancier
      // where, rather than having just a string as the key, we have a
      // structure and the structure precomputes some of the information
      // for the actual key.  Thus, we could then customize the hash and
      // comparison functors so that they use the precomputed information
      // rather than computing it each time.  For now, though, I want to
      // make this easy to get things working.
      typedef ACE_Hash_Map_Manager_Ex<OBJECT_KEY,
                                      CORBA::Boolean, // access_allowed?
                                      ACE_Hash<OBJECT_KEY>,
                                      ACE_Equal_To<OBJECT_KEY>,
                                      ACE_Null_Mutex> // not sure this is right
      ACCESS_MAP_TYPE;

      ACCESS_MAP_TYPE access_map_;

      // Lock for accessing the map.  It may be possible to get away with
      // just using a lock directly in the map, but I'm not sure, so I'll err
      // conservatively.
      TAO_SYNCH_MUTEX map_lock_;

    private:
      /*!
       * @brief Encapsulates a TAO-specific way to do object_to_string() without having an ORB reference handy.
       *
       * @note If OBJECT_KEY changes as described above, this should change
       * so that it generates an OBJECT_KEY.
       */
      OBJECT_KEY map_key_from_objref (CORBA::Object_ptr obj);

      //
      // This is the private implementation that is common to both
      // access_allowed and access_allowed_ex.
      ::CORBA::Boolean access_allowed_i (OBJECT_KEY& key,
                                         const char *operation_name,
                                         CORBA::Boolean collocated = false);

    };

    /**
     * @class SecurityManager
     *
     * @brief
     *
     */
    class SecurityManager
      : public virtual SecurityLevel2::SecurityManager,
        public virtual ::CORBA::LocalObject
    {
    public:

      /// Constructor
      SecurityManager (/* not sure what's needed yet */);

      /**
       * @name SecurityLevel2::SecurityManager Methods
       *
       * Methods required by the SecurityLevel2::SecurityManager
       * interface.
       */
      //@{
      virtual ::Security::MechandOptionsList* supported_mechanisms ();
      virtual SecurityLevel2::CredentialsList* own_credentials ();
      virtual SecurityLevel2::RequiredRights_ptr required_rights_object ();
      virtual SecurityLevel2::PrincipalAuthenticator_ptr principal_authenticator ();
      virtual SecurityLevel2::AccessDecision_ptr access_decision ();
      virtual SecurityLevel2::AuditDecision_ptr audit_decision ();
      virtual SecurityLevel2::TargetCredentials_ptr get_target_credentials (CORBA::Object_ptr o);
      virtual void remove_own_credentials (SecurityLevel2::Credentials_ptr creds);
      virtual CORBA::Policy_ptr get_security_policy (CORBA::PolicyType policy_type);
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      virtual ~SecurityManager (void);

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
