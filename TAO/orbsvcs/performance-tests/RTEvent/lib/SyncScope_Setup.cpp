/**
 * @file SyncScope_Setup.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "SyncScope_Setup.h"
#include "RIR_Narrow.h"

#if !defined(__ACE_INLINE__)
#include "SyncScope_Setup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, SyncScope_Setup, "$Id$")

SyncScope_Setup::SyncScope_Setup (CORBA::ORB_ptr orb,
                                  Messaging::SyncScope value
                                  ACE_ENV_ARG_DECL)
{
  CORBA::PolicyManager_var policy_manager =
    RIR_Narrow<CORBA::PolicyManager>::resolve (orb,
                                               "ORBPolicyManager"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);

  CORBA::Any sync_scope;
  sync_scope <<= Messaging::SYNC_WITH_TARGET;

  // @@ We need a helper class that automatically calls the
  //    destroy() method on each policy...
  policy_list[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        sync_scope
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  policy_manager->set_policy_overrides (policy_list,
                                        CORBA::ADD_OVERRIDE
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policy_list[0]->destroy (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

SyncScope_Setup::~SyncScope_Setup (void)
{
}

