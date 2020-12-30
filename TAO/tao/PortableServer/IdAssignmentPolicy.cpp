// -*- C++ -*-
#include "tao/PortableServer/IdAssignmentPolicy.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/CORBA_macros.h"

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
    IdAssignmentPolicy::copy ()
    {
      IdAssignmentPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdAssignmentPolicy (this->value_),
                        CORBA::NO_MEMORY ());

      return copy;
    }

    void
    IdAssignmentPolicy::destroy ()
    {
    }

    ::PortableServer::IdAssignmentPolicyValue
    IdAssignmentPolicy::value ()
    {
      return value_;
    }

    CORBA::PolicyType
    IdAssignmentPolicy::policy_type ()
    {
      return ::PortableServer::ID_ASSIGNMENT_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    IdAssignmentPolicy::_tao_cached_type () const
    {
      return TAO_CACHED_POLICY_ID_ASSIGNMENT;
    }

    TAO_Policy_Scope
    IdAssignmentPolicy::_tao_scope () const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
