/**
 * @file RTCORBA_Setup.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "RTCORBA_Setup.h"
#include "RIR_Narrow.h"
#include "RT_Class.h"

#include "ace/Log_Msg.h"

#if !defined(__ACE_INLINE__)
#include "RTCORBA_Setup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO_PERF_RTEC, 
           RTCORBA_Setup, 
           "$Id$")

RTCORBA_Setup::RTCORBA_Setup (CORBA::ORB_ptr orb,
                              const RT_Class &rtclass,
                              int nthreads
                              ACE_ENV_ARG_DECL)
  :  lanes_ (3)
{
  this->priority_mapping_manager_ =
    RIR_Narrow<RTCORBA::PriorityMappingManager>::resolve (orb,
                                                          "PriorityMappingManager"
                                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->priority_mapping_ =
    this->priority_mapping_manager_->mapping ();

  RTCORBA::Current_var current =
    RIR_Narrow<RTCORBA::Current>::resolve (orb,
                                           "RTCurrent"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::Priority corba_prc_priority;
  this->priority_mapping_->to_CORBA (rtclass.priority_process (),
                                     corba_prc_priority);

  current->the_priority (corba_prc_priority
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->lanes_.length (3);

  this->setup_lane (rtclass.priority_high (),
                    this->lanes_[0]
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->setup_lane (rtclass.priority_process (),
                    this->lanes_[1]
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->setup_lane (rtclass.priority_low (),
                    this->lanes_[2]
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->lanes_[2].static_threads = nthreads;

  this->process_priority_ = this->lanes_[1].lane_priority;
}

void
RTCORBA_Setup::setup_lane (int priority,
                           RTCORBA::ThreadpoolLane &lane
			   ACE_ENV_ARG_DECL)
{
  if (!this->priority_mapping_->to_CORBA (priority,
                                          lane.lane_priority))
    ACE_THROW (CORBA::BAD_PARAM ());

  lane.static_threads  = 1;
  lane.dynamic_threads = 0;
}
