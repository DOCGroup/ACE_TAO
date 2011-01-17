// -*- C++ -*-
// $Id$

#include "tao/PortableServer/IdUniquenessPolicy.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/CORBA_macros.h"

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicy::IdUniquenessPolicy (
      ::PortableServer::IdUniquenessPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    IdUniquenessPolicy::copy (void)
    {
      IdUniquenessPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdUniquenessPolicy (this->value_),
                        CORBA::NO_MEMORY ());

      return copy;
    }

    void
    IdUniquenessPolicy::destroy (void)
    {
    }

    ::PortableServer::IdUniquenessPolicyValue
    IdUniquenessPolicy::value (void)
    {
      return value_;
    }

    CORBA::PolicyType
    IdUniquenessPolicy::policy_type (void)
    {
      return ::PortableServer::ID_UNIQUENESS_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    IdUniquenessPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_ID_UNIQUENESS;
    }

    TAO_Policy_Scope
    IdUniquenessPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
