// $Id$

#include "BiDir_Policy_i.h"
#include "tao/Stub.h"
#include "tao/debug.h"

ACE_RCSID(TAO, BiDir_Policy_i, "$Id$")



#if ! defined (__ACE_INLINE__)
#include "BiDir_Policy_i.inl"
#endif /* __ACE_INLINE__ */

TAO_BidirectionalPolicy::TAO_BidirectionalPolicy (
    const BiDirPolicy::BidirectionalPolicyValue  val)
  : value_ (val)
{
}

TAO_BidirectionalPolicy::TAO_BidirectionalPolicy (const TAO_BidirectionalPolicy &rhs)
  : BiDirPolicy::BidirectionalPolicy (),
    TAO_Local_RefCounted_Object (),
    value_ (rhs.value_)
{
}



CORBA::PolicyType
TAO_BidirectionalPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE;
}


TAO_BidirectionalPolicy *
TAO_BidirectionalPolicy::clone (void) const
{
  TAO_BidirectionalPolicy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_BidirectionalPolicy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
TAO_BidirectionalPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TAO_BidirectionalPolicy* tmp;
  ACE_NEW_THROW_EX (tmp, TAO_BidirectionalPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_BidirectionalPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


BiDirPolicy::BidirectionalPolicyValue
TAO_BidirectionalPolicy::value (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
      CORBA::SystemException))
{
  return this->value_;
}


TAO_Cached_Policy_Type
TAO_BidirectionalPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_BIDIRECTIONAL_GIOP;
}
