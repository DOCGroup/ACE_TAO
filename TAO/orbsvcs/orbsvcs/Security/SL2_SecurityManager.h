// -*- C++ -*-

//=============================================================================
/**
 * @file SL2_SecurityManager.h
 *
 * $Id$
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

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<>
class ACE_Export ACE_Hash<CORBA::Object_var>
{
public:
  unsigned long operator() (const CORBA::Object_var&) const;
};

template<>
class ACE_Export ACE_Equal_To<CORBA::Object_var>
{
public:
  int operator () (const CORBA::Object_var& lhs,
		   const CORBA::Object_var& rhs) const;
};

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
	public virtual TAO_Local_RefCounted_Object
    {
    public:
      /*! Constructor */
      AccessDecision (/* not yet known */);
      ~AccessDecision (void);
      
      virtual ::CORBA::Boolean access_allowed (
        const ::SecurityLevel2::CredentialsList & cred_list,
        ::CORBA::Object_ptr target,
        const char * operation_name,
        const char * target_interface_name
      )
	ACE_THROW_SPEC ((::CORBA::SystemException));

      virtual ::CORBA::Boolean default_decision (void)
	ACE_THROW_SPEC ((::CORBA::SystemException));
      virtual void default_decision (::CORBA::Boolean d)
	ACE_THROW_SPEC ((::CORBA::SystemException));

      virtual void add_object (::CORBA::Object_ptr obj,
			       ::CORBA::Boolean allow_insecure_access)
	ACE_THROW_SPEC ((::CORBA::SystemException));
      virtual void remove_object (::CORBA::Object_ptr obj)
	ACE_THROW_SPEC ((::CORBA::SystemException));

    private:
      /*!
       * This is the default value that's returned from access_allowed()
       * when the access table doesn't contain an entry for the reference.
       */
      ::CORBA::Boolean default_allowance_decision_;

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
      typedef CORBA::Object_var OBJECT_KEY;
      // This is typedef'd because we might try to do something fancier
      // where, rather than having just a string as the key, we have a
      // structure and the structure precomputes some of the information
      // for the actual key.  Thus, we could then customize the hash and
      // comparison functors so that they use the precomputed information
      // rather than computing it each time.  For now, though, I want to
      // make this easy to get things working.
#if 0
      typedef ACE_Hash_Map_Manager_Ex<OBJECT_KEY,  // stringified IOR
				      CORBA::Boolean, // access_allowed?
				      ACE_Hash<const char*>,
				      ACE_Equal_To<const char*>,
				      ACE_Null_Mutex> // not sure this is right
#else
      typedef ACE_Hash_Map_Manager_Ex<OBJECT_KEY, // Object_var
				      CORBA::Boolean, // access_allowed?
				      ACE_Hash<CORBA::Object_var>,
				      ACE_Equal_To<CORBA::Object_var>,
				      ACE_Null_Mutex>
#endif
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
    };

    /**
     * @class SecurityManager
     *
     * @brief
     *
     */
    class SecurityManager
      : public virtual SecurityLevel2::SecurityManager,
        public virtual TAO_Local_RefCounted_Object
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
      virtual ::Security::MechandOptionsList* supported_mechanisms ()
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual SecurityLevel2::CredentialsList* own_credentials ()
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual SecurityLevel2::RequiredRights_ptr required_rights_object ()
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual SecurityLevel2::PrincipalAuthenticator_ptr principal_authenticator ()
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual SecurityLevel2::AccessDecision_ptr access_decision ()
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual SecurityLevel2::AuditDecision_ptr audit_decision ()
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual SecurityLevel2::TargetCredentials_ptr get_target_credentials (CORBA::Object_ptr o)
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual void remove_own_credentials (SecurityLevel2::Credentials_ptr creds)
        ACE_THROW_SPEC ((CORBA::SystemException));
      virtual CORBA::Policy_ptr get_security_policy (CORBA::PolicyType policy_type)
        ACE_THROW_SPEC ((CORBA::SystemException));
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
