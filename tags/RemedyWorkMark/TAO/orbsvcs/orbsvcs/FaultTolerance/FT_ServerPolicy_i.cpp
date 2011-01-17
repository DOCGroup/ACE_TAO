//$Id$
#include "orbsvcs/FaultTolerance/FT_ServerPolicy_i.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/Any.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/FaultTolerance/FT_ServerPolicy_i.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Boolean
TAO_FT_Heart_Beat_Enabled_Policy::heartbeat_enabled_policy_value ()
{
  return this->heartbeat_enabled_value_;
}


CORBA::Policy_ptr
TAO_FT_Heart_Beat_Enabled_Policy::create (const CORBA::Any& val)
{
  CORBA::Boolean value;

  if ((val >>= CORBA::Any::to_boolean (value)) == 0)
    throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_FT_Heart_Beat_Enabled_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_FT_Heart_Beat_Enabled_Policy (value),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

CORBA::PolicyType
TAO_FT_Heart_Beat_Enabled_Policy::policy_type (void)
{
  return FT::HEARTBEAT_ENABLED_POLICY;
}


CORBA::Policy_ptr
TAO_FT_Heart_Beat_Enabled_Policy::copy (void)
{
  TAO_FT_Heart_Beat_Enabled_Policy * tmp = 0;
  ACE_NEW_THROW_EX (tmp, TAO_FT_Heart_Beat_Enabled_Policy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

TAO_FT_Heart_Beat_Enabled_Policy *
TAO_FT_Heart_Beat_Enabled_Policy::clone (void) const
{
  TAO_FT_Heart_Beat_Enabled_Policy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_FT_Heart_Beat_Enabled_Policy (*this),
                  0);
  return copy;
}

void
TAO_FT_Heart_Beat_Enabled_Policy::destroy (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
