// $Id$

#include "Messaging_Policy_i.h"

#if defined (TAO_HAS_CORBA_MESSAGING)

#if ! defined (__ACE_INLINE__)
#include "Messaging_Policy_i.i"
#endif /* __ACE_INLINE__ */

#include "ace/Auto_Ptr.h"

ACE_RCSID(TAO, Messaging_Policy_i, "$Id$")

TimeBase::TimeT
TAO_RelativeRoundtripTimeoutPolicy_i::relative_expiry (
      CORBA::Environment &ACE_TRY_ENV
    )
{
  return this->relative_expiry_;
}

CORBA::PolicyType
TAO_RelativeRoundtripTimeoutPolicy_i::policy_type (
      CORBA_Environment &ACE_TRY_ENV
    )
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_RelativeRoundtripTimeoutPolicy_i::create (
      PortableServer::POA_ptr poa,
      const CORBA::Any& val,
      CORBA::Environment &ACE_TRY_ENV
    )
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TimeBase::TimeT value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_RelativeRoundtripTimeoutPolicy_i *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_RelativeRoundtripTimeoutPolicy_i (poa,
                                                        value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  auto_ptr<TAO_RelativeRoundtripTimeoutPolicy_i> clone (tmp);

  CORBA::Policy_var result = clone->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  clone->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  clone.release ();

  return result._retn ();
}

CORBA::Policy_ptr
TAO_RelativeRoundtripTimeoutPolicy_i::copy (
      CORBA_Environment &ACE_TRY_ENV
    )
{
  // Future policy implementors: notice how the following code is
  // exception safe!

  TAO_RelativeRoundtripTimeoutPolicy_i* tmp;
  ACE_NEW_THROW_EX (tmp, TAO_RelativeRoundtripTimeoutPolicy_i (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  auto_ptr<TAO_RelativeRoundtripTimeoutPolicy_i> clone (tmp);

  CORBA::Policy_var result = clone->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  clone->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  clone.release ();

  return result._retn ();
}

void
TAO_RelativeRoundtripTimeoutPolicy_i::destroy (
      CORBA_Environment &ACE_TRY_ENV
    )
{
  PortableServer::ObjectId_var id =
    this->poa_->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  this->poa_->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

PortableServer::POA_ptr
TAO_RelativeRoundtripTimeoutPolicy_i::_default_POA (
      CORBA_Environment &ACE_TRY_ENV
    )
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<TAO_RelativeRoundtripTimeoutPolicy_i>;
template class ACE_Auto_Basic_Ptr<TAO_RelativeRoundtripTimeoutPolicy_i>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<TAO_RelativeRoundtripTimeoutPolicy_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_RelativeRoundtripTimeoutPolicy_i>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_CORBA_MESSAGING */
