// @(#) $Id$

#include "POA_Policy_Set.h"
#include "POA_Cached_Policies.h"
#include "Policy_Validator.h"

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
TAO_POA_Policy_Set::add_client_exposed_fixed_policies (CORBA::PolicyList *client_exposed_policies,
                                                       CORBA::Environment &ACE_TRY_ENV)
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
                                  policy->copy (ACE_TRY_ENV);
          ACE_CHECK;

          cep_index++;
        }
    }
}

void
TAO_POA_Policy_Set::validate_policies (TAO_POA_Policy_Validator &validator,
                                       TAO_ORB_Core &orb_core,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  // Validate that all of the specified policies make sense.
  validator.validate (this->impl_, ACE_TRY_ENV);
  ACE_CHECK;

  // Verify that all policies are legal for the currently loaded
  // POA extensions.
  for (CORBA::ULong i = 0;
       i < this->impl_.num_policies ();
       i++)
    {
      CORBA::Policy_var policy = this->impl_.get_policy_by_index (i);

      CORBA::PolicyType type = policy->policy_type (ACE_TRY_ENV);
      ACE_CHECK;

      if (validator.legal_policy (type) == 0)
        {
          // @@ Bala, please move this into a policy validator and then
          //    register it when you load the BiDir library.

          // Check whether we have a BiDirectional policy set. Call the
          // ORB_Core to do the checking for us
          int retval = orb_core.parse_bidir_policy (policy.in (),
                                                    ACE_TRY_ENV);

          ACE_CHECK;

          // The policy has been successfully parsed, so return
          if (retval)
            continue;

          // An invalid policy was specified.  Let the user know about
          // it.
          ACE_THROW (PortableServer::POA::InvalidPolicy ());
        }
    }
}


