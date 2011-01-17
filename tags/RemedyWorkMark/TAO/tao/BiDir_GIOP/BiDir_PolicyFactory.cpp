// -*- C++ -*-
// $Id$

#include "tao/BiDir_GIOP/BiDir_PolicyFactory.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/BiDir_GIOP/BiDir_Policy_i.h"
#include "tao/ORB_Constants.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_BiDir_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
  CORBA::Policy_ptr policy = CORBA::Policy_ptr ();

  if (type == BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE)
    {
      BiDirPolicy::BidirectionalPolicyValue val;

      // Extract the value from the any.
      if ((value >>= val) == 0)
        {
          throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
        }

      ACE_NEW_THROW_EX (policy,
                        TAO_BidirectionalPolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
