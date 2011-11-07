// -*- C++ -*-
// $Id$

#include "tao/PI_Server/ServerRequestDetails.h"

#if TAO_HAS_INTERCEPTORS == 1

#if !defined (__ACE_INLINE__)
#include "tao/PI_Server/ServerRequestDetails.inl"
#endif /* defined INLINE */

#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  void
  ServerRequestDetails::apply_policies (const CORBA::PolicyList &policies)
  {
    // Flag to check for duplicate ProcessingModePolicy objects in the list.
    bool processing_mode_applied = false;

    CORBA::ULong const plen = policies.length ();

    for (CORBA::ULong i = 0; i < plen; ++i)
      {
        CORBA::Policy_var policy = CORBA::Policy::_duplicate (policies[i]);

        if (CORBA::is_nil (policy.in ()))
          {
            // Just ignore nil policies...
            continue;
          }

        // Obtain the PolicyType from the current Policy object.
        CORBA::PolicyType const policy_type = policy->policy_type ();

        if (policy_type == PortableInterceptor::PROCESSING_MODE_POLICY_TYPE)
          {
            if (processing_mode_applied)
              {
                // This is the second time we have run into this policy type,
                // and that is not allowed.
                throw ::CORBA::INV_POLICY ();
              }

            // Flip the flag to true in order to trap any dupes.
            processing_mode_applied = true;

            // Narrow the Policy to the ProcessingModePolicy interface.
            PortableInterceptor::ProcessingModePolicy_var pm_policy =
                    PortableInterceptor::ProcessingModePolicy::_narrow (
                                                 policy.in ());

            // Save the value of the ProcessingModePolicy in our data member.
            this->processing_mode_ = pm_policy->processing_mode ();
          }
        else
          {
            // We don't support the current policy type.
            throw ::CORBA::INV_POLICY ();
          }
      }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
