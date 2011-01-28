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
#include "tao/Policy_ManagerC.h"

#if !defined(__ACE_INLINE__)
#include "PriorityBand_Setup.inl"
#endif /* __ACE_INLINE__ */

PriorityBand_Setup::PriorityBand_Setup (CORBA::ORB_ptr orb,
                                        const RTCORBA_Setup &rtcorba_setup)
{
  CORBA::PolicyManager_var policy_manager =
    RIR_Narrow<CORBA::PolicyManager>::resolve (orb, "ORBPolicyManager");

  RTCORBA::RTORB_var rtorb =
    RIR_Narrow<RTCORBA::RTORB>::resolve (orb, "RTORB");

  const RTCORBA::ThreadpoolLanes &lanes = rtcorba_setup.lanes ();

  RTCORBA::PriorityBands priority_bands (3); priority_bands.length (3);
  for (CORBA::ULong i = 0; i != lanes.length (); ++i)
    {
      priority_bands[i].low  = lanes[i].lane_priority;
      priority_bands[i].high = lanes[i].lane_priority;
    }

  CORBA::PolicyList policy_list (1); policy_list.length (1);

  policy_list[0] =
    rtorb->create_priority_banded_connection_policy (priority_bands);
  policy_manager->set_policy_overrides (policy_list,
                                        CORBA::ADD_OVERRIDE);

  policy_list[0]->destroy ();
}

PriorityBand_Setup::~PriorityBand_Setup (void)
{
}
