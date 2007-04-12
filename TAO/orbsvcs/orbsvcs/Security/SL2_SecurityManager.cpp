// $Id$

#include "orbsvcs/Security/SL2_SecurityManager.h"

#include "tao/ORB_Constants.h"

ACE_RCSID (Security,
           SL2_SecurityManager,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

unsigned long
ACE_Hash<CORBA::Object_var>::operator() (const CORBA::Object_var& o) const
{
  return o->_hash ((CORBA::ULong)-1);
}

int
ACE_Equal_To<CORBA::Object_var>::operator () (const CORBA::Object_var& lhs,
					      const CORBA::Object_var& rhs) const
{
  return lhs->_is_equivalent (rhs.in ());
}

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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::CredentialsList*
TAO::Security::SecurityManager::own_credentials ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::RequiredRights_ptr
TAO::Security::SecurityManager::required_rights_object ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::PrincipalAuthenticator_ptr
TAO::Security::SecurityManager::principal_authenticator ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return SecurityLevel2::PrincipalAuthenticator::_duplicate
    (this->principal_authenticator_.in () );
}

SecurityLevel2::AccessDecision_ptr
TAO::Security::SecurityManager::access_decision ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return SecurityLevel2::AccessDecision::_duplicate (this->access_decision_.in () );
}

SecurityLevel2::AuditDecision_ptr
TAO::Security::SecurityManager::audit_decision ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel2::TargetCredentials_ptr
TAO::Security::SecurityManager::get_target_credentials (CORBA::Object_ptr /*o*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO::Security::SecurityManager::remove_own_credentials (
  SecurityLevel2::Credentials_ptr creds)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  throw CORBA::NO_IMPLEMENT ();
}
                     
CORBA::Policy_ptr
TAO::Security::SecurityManager::get_security_policy (CORBA::PolicyType policy_type)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  throw CORBA::NO_IMPLEMENT ();
}

/*
 * AccessDecision stuff below here
 */

TAO::Security::AccessDecision::AccessDecision ()
  : default_allowance_decision_ (false)
{
}

TAO::Security::AccessDecision::~AccessDecision ()
{
}

TAO::Security::AccessDecision::OBJECT_KEY
TAO::Security::AccessDecision::map_key_from_objref (CORBA::Object_ptr obj)
{
#if 0
  // Originally this lived in access_allowed, but it was needed
  // in add_object and remove_object, too, so it's been factored out.
  //
  // We need an ORB reference here.  Where do we get it?
  //
  // The primary place we need this facility is in access_allowed.
  // Unfortunately, the interface for access_allowed is cast in
  // dormant OMG spec stone, so that can't change.  We could pass in a
  // reference as an argument to the constructor and store it, but
  // what do we do, then, if the same interceptor is registered with
  // multiple ORBs (is that possible?!?!)?  Then we could end up using
  // a different ORB to stringify, which could end up giving us a
  // different string, which means they won't compare propertly.
  //
  // As a hack, we could realize that TAO's CORBA::Object implementation
  // has a reference to its associated ORB, and just dip in there
  // to get access to it.  Ugly, but at least it should probably work.
  CORBA::ORB_var orb = obj->_get_orb ();
  CORBA::String_var ior = orb->object_to_string (obj);
  return ior;
#else
  return CORBA::Object::_duplicate(obj);
#endif
}

CORBA::Boolean
TAO::Security::AccessDecision::access_allowed (
  const ::SecurityLevel2::CredentialsList & cred_list,
  ::CORBA::Object_ptr target,
  const char * operation_name,
  const char * target_interface_name
					       )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ I still don't know what we do with the cred_list in here...
  // Do we inspect it?

  // Turn the target into what we'll use as a key into the map.
  OBJECT_KEY key = this->map_key_from_objref (target);

  // LOCK THE MAP!
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->map_lock_,
		    this->default_allowance_decision_);

  ACE_Hash<CORBA::Object_var> hash;

  // Look up the target in access_map_; if there, return the value,
  // otherwise return the default value.
  CORBA::Boolean access_decision;
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

void
TAO::Security::AccessDecision::add_object (CORBA::Object_ptr obj,
					   CORBA::Boolean allow_insecure_access)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // make a key from 'obj'
  OBJECT_KEY key = this->map_key_from_objref (obj);

  // bind it into the access_map_, replacing anything that's there.
  // LOCK THE MAP!
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->map_lock_);

  ACE_Hash<CORBA::Object_var> hash;

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
		    errno));
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
TAO::Security::AccessDecision::remove_object (CORBA::Object_ptr obj)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // make a key from 'obj'
  OBJECT_KEY key = this->map_key_from_objref (obj);

  ACE_Hash<CORBA::Object_var> hash;

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
			errno));
	  throw
	    CORBA::UNKNOWN (CORBA::SystemException::_tao_minor_code (TAO::VMCID,
								     errno),
			    CORBA::COMPLETED_NO);
	}
    }
}

CORBA::Boolean
TAO::Security::AccessDecision::default_decision (void)
  ACE_THROW_SPEC ((::CORBA::SystemException))
{
  return this->default_allowance_decision_;
}

void
TAO::Security::AccessDecision::default_decision (CORBA::Boolean d)
  ACE_THROW_SPEC ((::CORBA::SystemException))
{
  this->default_allowance_decision_ = d;
}

TAO_END_VERSIONED_NAMESPACE_DECL
