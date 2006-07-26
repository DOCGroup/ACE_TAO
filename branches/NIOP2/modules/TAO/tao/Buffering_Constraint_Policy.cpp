// $Id$

#include "tao/Buffering_Constraint_Policy.h"

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

#include "tao/TAOC.h"
#include "tao/SystemException.h"

ACE_RCSID (tao,
           Buffering_Constraint_Policy,
           "$Id$")

#if ! defined (__ACE_INLINE__)
#include "tao/Buffering_Constraint_Policy.i"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Buffering_Constraint_Policy::TAO_Buffering_Constraint_Policy (const TAO::BufferingConstraint &buffering_constraint)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::CORBA::LocalObject ()
  , TAO::BufferingConstraintPolicy ()
  , TAO_Local_RefCounted_Object ()
  , buffering_constraint_ (buffering_constraint)
{
}

TAO_Buffering_Constraint_Policy::TAO_Buffering_Constraint_Policy (const TAO_Buffering_Constraint_Policy &rhs)

  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::CORBA::LocalObject ()
  , TAO::BufferingConstraintPolicy ()
  , TAO_Local_RefCounted_Object ()
  , buffering_constraint_ (rhs.buffering_constraint_)
{
}

CORBA::PolicyType
TAO_Buffering_Constraint_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return TAO::BUFFERING_CONSTRAINT_POLICY_TYPE;
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
TAO_Buffering_Constraint_Policy::buffering_constraint (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->buffering_constraint_;
}

CORBA::Policy_ptr
TAO_Buffering_Constraint_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_Buffering_Constraint_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_Cached_Policy_Type
TAO_Buffering_Constraint_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_BUFFERING_CONSTRAINT;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
