// @(#) $Id$

#include "POA_Policy_Set.h"
#include "POA_Cached_Policies.h"
#include "tao/Policy_Validator.h"

#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "POA_Policy_Set.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, POA, "$Id$")

TAO_POA_Policy_Set::TAO_POA_Policy_Set ()
  :
    impl_ (TAO_POLICY_POA_SCOPE)
{
}

TAO_POA_Policy_Set::TAO_POA_Policy_Set (const TAO_POA_Policy_Set &rhs)
  :
    impl_ (rhs.impl_)
{
}

TAO_POA_Policy_Set::~TAO_POA_Policy_Set (void)
{
}

void
TAO_POA_Policy_Set::add_client_exposed_fixed_policies (CORBA::PolicyList *client_exposed_policies
                                                       ACE_ENV_ARG_DECL)
{
  CORBA::ULong cep_index = client_exposed_policies->length ();

  for (CORBA::ULong i = 0;
       i < this->num_policies ();
       ++i)
    {
      CORBA::Policy_var policy = this->get_policy_by_index (i);

      // If this policy is client exposed, add it to the list.
      if (policy->_tao_scope () & TAO_POLICY_CLIENT_EXPOSED)
        {
          client_exposed_policies->length (cep_index + 1);
          (*client_exposed_policies)[cep_index] =
                                  policy->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          cep_index++;
        }
    }
}

void
TAO_POA_Policy_Set::validate_policies (TAO_Policy_Validator &validator,
                                       TAO_ORB_Core &orb_core
                                       ACE_ENV_ARG_DECL)
{
  // Just give a last chance for all the unloaded validators in other
  // libraries to be registered
  orb_core.load_policy_validators (validator);

  // Validate that all of the specified policies make sense.
  validator.validate (this->impl_  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Verify that all policies are legal for the currently loaded
  // POA extensions.
  for (CORBA::ULong i = 0;
       i < this->impl_.num_policies ();
       i++)
    {
      CORBA::Policy_var policy = this->impl_.get_policy_by_index (i);

      CORBA::PolicyType type = policy->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (validator.legal_policy (type) == 0)
        {
          // An invalid policy was specified.  Let the user know about
          // it.
          ACE_THROW (PortableServer::POA::InvalidPolicy ());
        }
    }
}
