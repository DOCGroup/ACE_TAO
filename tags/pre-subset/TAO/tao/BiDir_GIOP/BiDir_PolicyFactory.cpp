#include "BiDir_PolicyFactory.h"
#include "BiDirGIOP.h"
#include "BiDir_Policy_i.h"


ACE_RCSID (BiDir_GIOP,
           BiDir_PolicyFactory,
           "$Id$")


CORBA::Policy_ptr
TAO_BiDir_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  if (type == BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE)
    {
      BiDirPolicy::BidirectionalPolicyValue val;

      // Extract the value from the any
      value >>= val;

      ACE_NEW_THROW_EX (policy,
                        TAO_BidirectionalPolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
