#include "tao/BiDir_GIOP/BiDir_Policy_i.h"

#include "tao/Stub.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_BidirectionalPolicy::TAO_BidirectionalPolicy (
    const BiDirPolicy::BidirectionalPolicyValue  val)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::BiDirPolicy::BidirectionalPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (val)
{
}

TAO_BidirectionalPolicy::TAO_BidirectionalPolicy (const TAO_BidirectionalPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::BiDirPolicy::BidirectionalPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (rhs.value_)
{
}

CORBA::PolicyType
TAO_BidirectionalPolicy::policy_type ()
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE;
}


TAO_BidirectionalPolicy *
TAO_BidirectionalPolicy::clone () const
{
  TAO_BidirectionalPolicy *copy = nullptr;
  ACE_NEW_RETURN (copy,
                  TAO_BidirectionalPolicy (*this),
                  nullptr);
  return copy;
}

CORBA::Policy_ptr
TAO_BidirectionalPolicy::copy (void)
{
  // Future policy implementors: notice how the following code is
  // exception safe!
  TAO_BidirectionalPolicy* tmp = nullptr;
  ACE_NEW_THROW_EX (tmp, TAO_BidirectionalPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_BidirectionalPolicy::destroy ()
{
}

BiDirPolicy::BidirectionalPolicyValue
TAO_BidirectionalPolicy::value ()
{
  return this->value_;
}


TAO_Cached_Policy_Type
TAO_BidirectionalPolicy::_tao_cached_type () const
{
  return TAO_CACHED_POLICY_BIDIRECTIONAL_GIOP;
}

TAO_END_VERSIONED_NAMESPACE_DECL
