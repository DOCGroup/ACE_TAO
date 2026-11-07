#include "PolicyFactory.h"
#include "Policy.h"

CORBA::Policy_ptr
PolicyFactory::create_policy (CORBA::PolicyType type,
                              CORBA::Any const &value)
{
  // Sanity check since this condition should be caught earlier by the
  // TAO_PolicyFactory_Registry.
  if (type != Test::POLICY_TYPE)
    throw CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);

  CORBA::ULong val {};
  if (!(value >>= val))
    throw CORBA::INTERNAL ();

  Policy *p {};
  ACE_NEW_THROW_EX (p,
                    Policy (val),
                    CORBA::NO_MEMORY ());

  return p;
}
