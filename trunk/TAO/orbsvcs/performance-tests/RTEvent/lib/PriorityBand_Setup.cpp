/**
 * @file PriorityBand_Setup.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "PriorityBand_Setup.h"
#include "RTCORBA_Setup.h"
#include "RIR_Narrow.h"

#if !defined(__ACE_INLINE__)
#include "PriorityBand_Setup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, PriorityBand_Setup, "$Id$")

PriorityBand_Setup::PriorityBand_Setup (CORBA::ORB_ptr orb,
                                        const RTCORBA_Setup &rtcorba_setup
                                        ACE_ENV_ARG_DECL)
{
  CORBA::PolicyManager_var policy_manager =
    RIR_Narrow<CORBA::PolicyManager>::resolve (orb,
                                               "ORBPolicyManager"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::RTORB_var rtorb =
    RIR_Narrow<RTCORBA::RTORB>::resolve (orb,
                                         "RTORB"
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const RTCORBA::ThreadpoolLanes &lanes = rtcorba_setup.lanes ();

  RTCORBA::PriorityBands priority_bands (3); priority_bands.length (3);
  for (CORBA::ULong i = 0; i != lanes.length (); ++i)
    {
      priority_bands[i].low  = lanes[i].lane_priority;
      priority_bands[i].high = lanes[i].lane_priority;
    }

  CORBA::PolicyList policy_list (1); policy_list.length (1);

  policy_list[0] =
    rtorb->create_priority_banded_connection_policy (priority_bands
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  policy_manager->set_policy_overrides (policy_list,
                                        CORBA::ADD_OVERRIDE
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policy_list[0]->destroy (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PriorityBand_Setup::~PriorityBand_Setup (void)
{
}

