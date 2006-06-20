#include "tao/PortableServer/IdAssignmentPolicy.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicy,
           "$Id$")

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentPolicy::IdAssignmentPolicy (
      ::PortableServer::IdAssignmentPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    IdAssignmentPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdAssignmentPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdAssignmentPolicy (this->value_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
