// $Id$


#include "tao/Client_Priority_Policy.h"

#if ! defined (__ACE_INLINE__)
#include "tao/Client_Priority_Policy.i"
#endif /* __ACE_INLINE__ */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

ACE_RCSID(TAO, Client_Priority_Policy, "$Id$")

TAO_Client_Priority_Policy::TAO_Client_Priority_Policy (
    const TAO::PrioritySpecification &priority_spec)
  :  priority_spec_ (priority_spec)
{
}

TAO_Client_Priority_Policy::TAO_Client_Priority_Policy (const TAO_Client_Priority_Policy &rhs)
  : TAO::ClientPriorityPolicy (),
    TAO_Local_RefCounted_Object (),
    priority_spec_ (rhs.priority_spec_)
{
}

TAO::PrioritySpecification
TAO_Client_Priority_Policy::priority_specification (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->priority_spec_;
}

CORBA::PolicyType
TAO_Client_Priority_Policy::policy_type (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return TAO::CLIENT_PRIORITY_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_Client_Priority_Policy::create (const CORBA::Any& val,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TAO::PrioritySpecification *priority_spec;
  if ((val >>= priority_spec) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Client_Priority_Policy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Client_Priority_Policy (*priority_spec),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

TAO_Client_Priority_Policy *
TAO_Client_Priority_Policy::clone (void) const
{
  TAO_Client_Priority_Policy *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_Client_Priority_Policy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
TAO_Client_Priority_Policy::copy (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TAO_Client_Priority_Policy* tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Client_Priority_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_Client_Priority_Policy::destroy (CORBA_Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */
