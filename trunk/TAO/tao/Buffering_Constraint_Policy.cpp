// $Id$

#include "tao/Buffering_Constraint_Policy.h"

#if ! defined (__ACE_INLINE__)
#include "tao/Buffering_Constraint_Policy.i"
#endif /* __ACE_INLINE__ */

#if defined (TAO_HAS_CORBA_MESSAGING)

ACE_RCSID(TAO, Buffering_Constraint_Policy, "$Id$")

TAO_Buffering_Constraint_Policy::TAO_Buffering_Constraint_Policy (const TAO::BufferingConstraint &buffering_constraint,
                                                                  PortableServer::POA_ptr poa)
  : buffering_constraint_ (buffering_constraint),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Buffering_Constraint_Policy::TAO_Buffering_Constraint_Policy (const TAO_Buffering_Constraint_Policy &rhs)
  : TAO_RefCountServantBase (rhs),
    POA_TAO::BufferingConstraintPolicy (rhs),
    buffering_constraint_ (rhs.buffering_constraint_),
    poa_ (rhs.poa_)
{
}

TAO::BufferingConstraint
TAO_Buffering_Constraint_Policy::buffering_constraint (CORBA::Environment &)
{
  return this->buffering_constraint_;
}

TAO::BufferingConstraint
TAO_Buffering_Constraint_Policy::buffering_constraint (void)
{
  return this->buffering_constraint_;
}

CORBA::PolicyType
TAO_Buffering_Constraint_Policy::policy_type (CORBA_Environment &)
{
  return TAO::BUFFERING_CONSTRAINT_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_Buffering_Constraint_Policy::create (PortableServer::POA_ptr poa,
                                         const CORBA::Any& val,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  TAO::BufferingConstraint *buffering_constraint;
  if ((val >>= buffering_constraint) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Buffering_Constraint_Policy *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_Buffering_Constraint_Policy (*buffering_constraint,
                                                     poa),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  PortableServer::ServantBase_var smart_servant (servant);

  CORBA::Policy_var result = servant->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return result._retn ();
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

CORBA::Policy_ptr
TAO_Buffering_Constraint_Policy::copy (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Buffering_Constraint_Policy* servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_Buffering_Constraint_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  PortableServer::ServantBase_var smart_servant (servant);

  CORBA::Policy_var result = servant->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return result._retn ();
}

void
TAO_Buffering_Constraint_Policy::destroy (CORBA_Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this,
                               ACE_TRY_ENV);
  ACE_CHECK;

  this->poa_->deactivate_object (id.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;
}

PortableServer::POA_ptr
TAO_Buffering_Constraint_Policy::_default_POA (CORBA_Environment &)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_CORBA_MESSAGING */
