// $Id$


#include "tao/Buffering_Constraint_Policy.h"

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ACE_RCSID(TAO, Buffering_Constraint_Policy, "$Id$")

#if ! defined (__ACE_INLINE__)
#include "tao/Buffering_Constraint_Policy.i"
#endif /* __ACE_INLINE__ */

TAO_Buffering_Constraint_Policy::TAO_Buffering_Constraint_Policy (const TAO::BufferingConstraint &buffering_constraint)
  : buffering_constraint_ (buffering_constraint)
{
}

TAO_Buffering_Constraint_Policy::TAO_Buffering_Constraint_Policy (const TAO_Buffering_Constraint_Policy &rhs)
  : TAO::BufferingConstraintPolicy (),
    TAO_Local_RefCounted_Object (),
    buffering_constraint_ (rhs.buffering_constraint_)
{
}

CORBA::PolicyType
TAO_Buffering_Constraint_Policy::policy_type (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return TAO::BUFFERING_CONSTRAINT_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_Buffering_Constraint_Policy::create (const CORBA::Any& val
                                         TAO_ENV_ARG_DECL)
{
  TAO::BufferingConstraint *buffering_constraint;
  if ((val >>= buffering_constraint) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Buffering_Constraint_Policy *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_Buffering_Constraint_Policy (*buffering_constraint),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return servant;
}

TAO_Buffering_Constraint_Policy *
TAO_Buffering_Constraint_Policy::clone (void) const
{
  TAO_Buffering_Constraint_Policy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_Buffering_Constraint_Policy (*this),
                  0);
  return copy;
}

TAO::BufferingConstraint
TAO_Buffering_Constraint_Policy::buffering_constraint (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->buffering_constraint_;
}

CORBA::Policy_ptr
TAO_Buffering_Constraint_Policy::copy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Buffering_Constraint_Policy* servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_Buffering_Constraint_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return servant;
}

void
TAO_Buffering_Constraint_Policy::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_Cached_Policy_Type
TAO_Buffering_Constraint_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_BUFFERING_CONSTRAINT;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
