#include "IdAssignmentPolicy.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"
#include "PortableServer.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentPolicy::IdAssignmentPolicy ()
    {
    }

    void
    IdAssignmentPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::IdAssignmentPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    IdAssignmentPolicy::init (
      ::PortableServer::IdAssignmentPolicyValue value)
    {
      value_ = value;
    }

    CORBA::Policy_ptr
    IdAssignmentPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdAssignmentPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdAssignmentPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_);

      return copy;
    }

    void
    IdAssignmentPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::IdAssignmentPolicyValue
    IdAssignmentPolicy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_;
    }

    CORBA::PolicyType
    IdAssignmentPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::ID_ASSIGNMENT_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    IdAssignmentPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_ID_ASSIGNMENT;
    }

    TAO_Policy_Scope
    IdAssignmentPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

