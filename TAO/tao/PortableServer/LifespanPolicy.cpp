// -*- C++ -*-
// $Id$

#include "tao/PortableServer/LifespanPolicy.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/CORBA_macros.h"

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicy::LifespanPolicy (
      ::PortableServer::LifespanPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    LifespanPolicy::copy (void)
    {
      LifespanPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        LifespanPolicy (this->value_),
                        CORBA::NO_MEMORY ());

      return copy;
    }

    void
    LifespanPolicy::destroy (void)
    {
    }

    ::PortableServer::LifespanPolicyValue
    LifespanPolicy::value (void)
    {
      return value_;
    }

    CORBA::PolicyType
    LifespanPolicy::policy_type (void)
    {
      return ::PortableServer::LIFESPAN_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    LifespanPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_LIFESPAN;
    }

    TAO_Policy_Scope
    LifespanPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
