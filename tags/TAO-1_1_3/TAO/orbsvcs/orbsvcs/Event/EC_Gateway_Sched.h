/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Gateway_Sched
//
// = AUTHOR
//   Carlos O'Ryan
//
// = DESCRIPTION
//   Extend the EC_Gateway_IIOP class to support the scheduling service.
//
// ============================================================================

#ifndef TAO_EC_GATEWAY_SCHED_H
#define TAO_EC_GATEWAY_SCHED_H
#include "ace/pre.h"

#include "EC_Gateway.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "sched_event_export.h"

// ****************************************************************

class TAO_RTSchedEvent_Export TAO_EC_Gateway_Sched : public TAO_EC_Gateway_IIOP
//
// = TITLE
//   Extend the EC_Gateway_IIOP interface to support scheduling.
//
{
public:
  TAO_EC_Gateway_Sched (void);
  ~TAO_EC_Gateway_Sched (void);

  void init (RtecEventChannelAdmin::EventChannel_ptr rmt_ec,
             RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             RtecScheduler::Scheduler_ptr rmt_sched,
             RtecScheduler::Scheduler_ptr lcl_sched,
             const char* lcl_name,
             const char* rmt_name,
             CORBA::Environment &env = TAO_default_environment ());
  // To do its job this class requires to know the local and remote
  // ECs it will connect to; furthermore it also requires to build
  // RT_Infos for the local and remote schedulers.
  // @@ TODO part of the RT_Info is hardcoded, we need to make it
  // parametric.
};

#include "ace/post.h"
#endif /* ACE_EC_GATEWAY_SCHED_H */
