// $Id$


#include "tao/Client_Priority_Policy.h"

#if ! defined (__ACE_INLINE__)
#include "tao/Client_Priority_Policy.i"
#endif /* __ACE_INLINE__ */

#if (TAO_HAS_CORBA_MESSAGING == 1)


ACE_RCSID(TAO, Client_Priority_Policy, "$Id$")


TAO_Client_Priority_Policy::TAO_Client_Priority_Policy (
    PortableServer::POA_ptr poa,
    const TAO::PrioritySpecification &priority_spec)
  :  poa_ (PortableServer::POA::_duplicate (poa)),
     priority_spec_ (priority_spec)
{
}

TAO_Client_Priority_Policy::TAO_Client_Priority_Policy (const TAO_Client_Priority_Policy &rhs)
  : TAO_RefCountServantBase (rhs),
    POA_TAO::ClientPriorityPolicy (rhs),
    poa_ (rhs.poa_),
    priority_spec_ (rhs.priority_spec_)
{
}

TAO::PrioritySpecification
TAO_Client_Priority_Policy::priority_specification (CORBA::Environment &)
{
  return this->priority_spec_;
}

CORBA::PolicyType
TAO_Client_Priority_Policy::policy_type (CORBA_Environment &)
{
  return TAO::CLIENT_PRIORITY_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_Client_Priority_Policy::create (PortableServer::POA_ptr poa,
                                    const CORBA::Any& val,
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
                    TAO_Client_Priority_Policy (poa, *priority_spec),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  PortableServer::ServantBase_var clone (tmp);

  CORBA::Policy_var result = tmp->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return result._retn ();
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
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TAO_Client_Priority_Policy* tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Client_Priority_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  PortableServer::ServantBase_var clone (tmp);

  CORBA::Policy_var result = tmp->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return result._retn ();
}

void
TAO_Client_Priority_Policy::destroy (CORBA_Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  this->poa_->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

PortableServer::POA_ptr
TAO_Client_Priority_Policy::_default_POA (CORBA_Environment &)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
