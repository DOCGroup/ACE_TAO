// $Id$

#include "orbsvcs/Security/SL2_SecurityManager.h"

#include "tao/ORB_Constants.h"
#include "ace/Functor.h"
#include "tao/Object_KeyC.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/Creation_Time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Security::SecurityManager::SecurityManager (/* unknown */)
  : principal_authenticator_ (SecurityLevel2::PrincipalAuthenticator::_nil ())
{
  // this needs to change to access decision
  SecurityLevel2::AccessDecision_ptr ad;
  ACE_NEW_THROW_EX (ad,
                    TAO::Security::AccessDecision,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  this->access_decision_ = ad;
}

TAO::Security::SecurityManager::~SecurityManager (void)
{
}

Security::MechandOptionsList*
TAO::Security::SecurityManager::supported_mechanisms ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::CredentialsList*
TAO::Security::SecurityManager::own_credentials ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::RequiredRights_ptr
TAO::Security::SecurityManager::required_rights_object ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::PrincipalAuthenticator_ptr
TAO::Security::SecurityManager::principal_authenticator ()
{
  return SecurityLevel2::PrincipalAuthenticator::_duplicate
    (this->principal_authenticator_.in () );
}

SecurityLevel2::AccessDecision_ptr
TAO::Security::SecurityManager::access_decision ()
{
  return SecurityLevel2::AccessDecision::_duplicate (this->access_decision_.in () );
}

SecurityLevel2::AuditDecision_ptr
TAO::Security::SecurityManager::audit_decision ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::TargetCredentials_ptr
TAO::Security::SecurityManager::get_target_credentials (CORBA::Object_ptr /*o*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO::Security::SecurityManager::remove_own_credentials
  (SecurityLevel2::Credentials_ptr /*creds*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Policy_ptr
TAO::Security::SecurityManager::get_security_policy
(CORBA::PolicyType /*policy_type */)
{
  throw CORBA::NO_IMPLEMENT ();
}

/*
 * AccessDecision stuff below here
 */

bool
TAO::Security::AccessDecision::ReferenceKeyType::operator==
  (const ReferenceKeyType& other) const
{
  ::CORBA::ULong olen = this->oid_->length();
  ::CORBA::ULong alen = this->adapter_id_->length();

  if (olen == other.oid_->length() &&
      alen == other.adapter_id_->length())
    return (ACE_OS::memcmp (this->oid_->get_buffer(),
                            other.oid_->get_buffer(),olen) == 0 &&
            ACE_OS::memcmp (this->adapter_id_->get_buffer(),
                            other.adapter_id_->get_buffer(),alen) == 0 &&
            ACE_OS_String::strcmp (this->orbid_.in(), other.orbid_.in()) == 0);
  return false;
}

CORBA::ULong
TAO::Security::AccessDecision::ReferenceKeyType::hash () const
{
  return 0;
}

TAO::Security::AccessDecision::ReferenceKeyType::operator const char* () const
{
  return "<hardcoded refkey>";
}

TAO::Security::AccessDecision::AccessDecision ()
  : default_allowance_decision_ (false)
{
}

TAO::Security::AccessDecision::~AccessDecision ()
{
}

TAO::Security::AccessDecision::OBJECT_KEY
TAO::Security::AccessDecision::map_key_from_objref (CORBA::Object_ptr /*obj */)
{
  ACE_ERROR ((LM_ERROR, "map_key_from_objref is currently not implemented\n"));

  throw CORBA::NO_IMPLEMENT();

#if defined (__HP_aCC)
  OBJECT_KEY key;
  return key;
#endif /* __HP_aCC */
}

CORBA::Boolean
TAO::Security::AccessDecision::access_allowed_i (OBJECT_KEY &key,
                                                 const char *operation_name)
{
  // LOCK THE MAP!
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->map_lock_,
                    this->default_allowance_decision_);

  ACE_Hash<OBJECT_KEY> hash;

  // Look up the target in access_map_; if there, return the value,
  // otherwise return the default value.
  CORBA::Boolean access_decision = false;
  if (this->access_map_.find (key, access_decision) == -1)
    {
      // Couldn't find the IOR in the map, so we use the default
      access_decision = this->default_allowance_decision_;
      if (TAO_debug_level >= 3)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) SL2_AccessDecision::access_decision(%x,%s)"
                    " NOT FOUND using default %d\n",
                    hash.operator()(key),
                    operation_name, access_decision));
    }
  else if (TAO_debug_level >= 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) SL2_AccessDecision::access_decision(%x,%s)"
                  " found with decision %d\n",
                  hash.operator()(key),
                  operation_name, access_decision));
    }

  // For now we just return the default.
  return access_decision;

}

CORBA::Boolean
TAO::Security::AccessDecision::access_allowed_ex (
          const char * orb_id,
          const ::CORBA::OctetSeq & adapter_id,
          const ::CORBA::OctetSeq & object_id,
          const ::SecurityLevel2::CredentialsList & /*cred_list */,
          const char * operation_name)
{
  OBJECT_KEY key;
  key.orbid_ = orb_id;
  key.adapter_id_ = adapter_id;
  key.oid_ = object_id;

  return this->access_allowed_i (key, operation_name);
}

CORBA::Boolean
TAO::Security::AccessDecision::access_allowed
  (const ::SecurityLevel2::CredentialsList & /*cred_list */,
   ::CORBA::Object_ptr target,
   const char * operation_name,
   const char * /*target_interface_name */)
{
  // @@ I still don't know what we do with the cred_list in here...
  // Do we inspect it?

  // Turn the target into what we'll use as a key into the map.
  OBJECT_KEY key = this->map_key_from_objref (target);
  return this->access_allowed_i (key, operation_name);
}

void
TAO::Security::AccessDecision::add_object
  (const char * orb_id,
   const ::CORBA::OctetSeq & adapter_id,
   const ::CORBA::OctetSeq & object_id,
   CORBA::Boolean allow_insecure_access)
{
  // make a key from 'obj'
  OBJECT_KEY key;
  key.orbid_ = orb_id;
  key.adapter_id_ = adapter_id;
  key.oid_ = object_id;

  // bind it into the access_map_, replacing anything that's there.
  // LOCK THE MAP!
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->map_lock_);

  ACE_Hash<OBJECT_KEY> hash;

  // Since we want to replace any existing entry in the map, we just
  // use rebind.
  errno = 0; // Not sure if this gets set if rebind fails...it only
             // appears to fail when an allocation thru the allocator's
             // malloc() fails.  Depending on the malloc() implementation,
             // errno could get set OR an exception thrown.
  int ret = this->access_map_.rebind (key, allow_insecure_access);
  if (ret == -1)
    {
      // rebind shouldn't fail under normal circumstances
      if (TAO_debug_level > 1)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t): SL2_AccessDecision::add_object(%x,%d) "
                    "unexpectedly failed (errno=%d)\n",
                    hash.operator()(key),
                    allow_insecure_access,
                    ACE_ERRNO_GET));
      throw
        CORBA::NO_MEMORY(CORBA::SystemException::_tao_minor_code (TAO::VMCID,
                                                                  errno),
                                                                  CORBA::COMPLETED_NO);
    }
  else
    {
      if (TAO_debug_level >= 3)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t): SL2_AccessDecision::add_object(%x,%d) okay\n",
                    hash.operator()(key),
                    allow_insecure_access));
    }
}

void
TAO::Security::AccessDecision::remove_object
  (const char * orb_id,
   const ::CORBA::OctetSeq & adapter_id,
   const ::CORBA::OctetSeq & object_id)
{
  OBJECT_KEY key;
  key.orbid_ = orb_id;
  key.adapter_id_ = adapter_id;
  key.oid_ = object_id;

  ACE_Hash<OBJECT_KEY> hash;

  // unbind it from access_map_, no matter if it's not in there...
  // LOCK THE MAP!
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->map_lock_);

  errno = 0;
  int ret = this->access_map_.unbind (key);
  if (ret == -1)
    {
      if (errno == ENOENT)
        {
          // ignore b/c we don't care...maybe log a debug message for info
          if (TAO_debug_level >= 3)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t): SL2_AccessDecision::remove_object(%x) "
                        "object not found in access map\n",
                        hash.operator()(key)));
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t): SL2_AccessDecision::remove_object(%x) "
                        " unexpected error during unbind from map (errno=%d\n)",
                        hash.operator()(key),
                        ACE_ERRNO_GET));

          throw CORBA::UNKNOWN (CORBA::SystemException::_tao_minor_code (TAO::VMCID,
                                                                         errno),
                                                                         CORBA::COMPLETED_NO);
        }
    }
}

CORBA::Boolean
TAO::Security::AccessDecision::default_decision (void)
{
  return this->default_allowance_decision_;
}

void
TAO::Security::AccessDecision::default_decision (CORBA::Boolean d)
{
  this->default_allowance_decision_ = d;
}

TAO_END_VERSIONED_NAMESPACE_DECL
