//$Id$
#include "FT_ServerPolicy_i.h"

#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
#include "FT_ServerPolicy_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(FaultTolerance, FT_ServerPolicy_i, "$Id$")

CORBA::Boolean
TAO_FT_Heart_Beat_Enabled_Policy::heartbeat_enabled_policy_value (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->heartbeat_enabled_value_;
}


CORBA::Policy_ptr
TAO_FT_Heart_Beat_Enabled_Policy::create (const CORBA::Any& val
                                          ACE_ENV_ARG_DECL)
{
  CORBA::Boolean value;

  if ((val >>= CORBA::Any::to_boolean (value)) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE),
                      CORBA::Policy::_nil ());

  TAO_FT_Heart_Beat_Enabled_Policy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_FT_Heart_Beat_Enabled_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

CORBA::PolicyType
TAO_FT_Heart_Beat_Enabled_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return FT::HEARTBEAT_ENABLED_POLICY;
}


CORBA::Policy_ptr
TAO_FT_Heart_Beat_Enabled_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FT_Heart_Beat_Enabled_Policy * tmp;
  ACE_NEW_THROW_EX (tmp, TAO_FT_Heart_Beat_Enabled_Policy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

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
TAO_FT_Heart_Beat_Enabled_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
