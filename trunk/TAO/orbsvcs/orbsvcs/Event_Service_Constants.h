// -*- C++ -*-

//==========================================================================
/**
 *  @file    Event_Service_Constants.h
 *
 *  $Id$
 *
 *  Keep constants required by both Event Service implementation and
 *  the Scheduling Service implementation.
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//==========================================================================

#ifndef ACE_ES_CONSTANTS_H
#define ACE_ES_CONSTANTS_H

#include /**/ "ace/pre.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Time_Utilities.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// = Predefined event types.  All application defined types should be
// relative to ACE_ES_EVENT_UNDEFINED.
const long ACE_ES_EVENT_ANY = 0;
const long ACE_ES_EVENT_SHUTDOWN = 1;
const long ACE_ES_EVENT_ACT = 2;
const long ACE_ES_EVENT_NOTIFICATION = 3;
const long ACE_ES_EVENT_TIMEOUT = 4;
const long ACE_ES_EVENT_INTERVAL_TIMEOUT = 5;
const long ACE_ES_EVENT_DEADLINE_TIMEOUT = 6;
const long ACE_ES_GLOBAL_DESIGNATOR = 7;
const long ACE_ES_CONJUNCTION_DESIGNATOR = 8;
const long ACE_ES_DISJUNCTION_DESIGNATOR = 9;
const long ACE_ES_NEGATION_DESIGNATOR = 10;
const long ACE_ES_LOGICAL_AND_DESIGNATOR = 11;
const long ACE_ES_BITMASK_DESIGNATOR = 12;
const long ACE_ES_MASKED_TYPE_DESIGNATOR = 13;
const long ACE_ES_NULL_DESIGNATOR = 14;
const long ACE_ES_EVENT_UNDEFINED = 16;

///  Predefined event sources.
const long ACE_ES_EVENT_SOURCE_ANY = 0;

/// The max number of priorities provided by the target platform.
/// @todo This should be defined in ACE (somehow) and only mapped here
/// to some variables (and even that is doubtful).
const long ACE_Scheduler_MAX_PRIORITIES = 5;

const TimeBase::TimeT ACE_Scheduler_Rates[ACE_Scheduler_MAX_PRIORITIES] = {
     250000, // 40 Hz
     500000, // 20 Hz
    1000000, // 10 Hz
    2000000, // 5 Hz
   10000000  // 1 Hz
};

/// The minimum preemption priority.
const long ACE_Scheduler_MIN_PREEMPTION_PRIORITY =
      ACE_Scheduler_MAX_PRIORITIES - 1;

/// Max preemption priority.
const long ACE_Scheduler_MAX_PREEMPTION_PRIORITY = 0;

/// This is the maximum sub priority.
const u_long ACE_Scheduler_MAX_SUB_PRIORITY =
      ACE_Message_Block::MB_USER - 1;

/// This is the minimum sub priority.
const u_long ACE_Scheduler_MIN_SUB_PRIORITY =
      ACE_Message_Block::MB_NORMAL;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_ES_CONSTANTS_H */
