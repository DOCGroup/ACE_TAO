// $Id$

#include "PolicyFactory.h"
#include "Policy.h"

ACE_RCSID (PolicyFactory,
           PolicyFactory,
           "$Id$")


CORBA::Policy_ptr
PolicyFactory::create_policy (CORBA::PolicyType type,
                              const CORBA::Any & value)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  // Sanity check since this condition should be caught earlier by the
  // TAO_PolicyFactory_Registry.
  if (type != Test::POLICY_TYPE)
      ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                        CORBA::Policy::_nil ());

  CORBA::ULong val;
  if (!(value >>= val))
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      CORBA::Policy::_nil ());

  Policy * p = 0;
  ACE_NEW_THROW_EX (p,
                    Policy (val),
                    CORBA::NO_MEMORY ());

  return p;
}
