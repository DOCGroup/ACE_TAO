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
TAO_Buffering_Constraint_Policy::policy_type (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return TAO::BUFFERING_CONSTRAINT_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_Buffering_Constraint_Policy::create (const CORBA::Any& val,
                                         CORBA::Environment &ACE_TRY_ENV)
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
TAO_Buffering_Constraint_Policy::buffering_constraint (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->buffering_constraint_;
}

CORBA::Policy_ptr
TAO_Buffering_Constraint_Policy::copy (CORBA_Environment &ACE_TRY_ENV)
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
TAO_Buffering_Constraint_Policy::destroy (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
