// $Id$
//
// = DESCRIPTION
//    Keep constants required by both Event Service implementation and
//    the Scheduling Service implementation.
//

#ifndef ACE_ES_CONSTANTS_H
#define ACE_ES_CONSTANTS_H

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "orbsvcs/Time_Utilities.h"

const int ACE_ES_MAX_SUBSCRIPTIONS = 32;
// This is the number of events a consumer can subscribe to.

//const int ACE_ES_MAX_SUPPLIERS = 100;
// Max number of suppliers that a channel can handle.

const int ACE_ES_MAX_CONSUMERS_PER_SUPPLIER = 100;
// Max number of consumers that one supplier can have subscribed to
// one type of event (including source-only).  This restriction is
// very lightweight since subscription lists are dynamic.  This
// constant is only used for ACE_ES_Simple_Set in Event_Channel.* for
// disconnecting consumers.  Since these objects are allocated off the
// stack, we can increase this number without requiring more heap
// memory.

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
const long ACE_ES_EVENT_UNDEFINED = 16;

// = Predefined event sources.
const long ACE_ES_EVENT_SOURCE_ANY = 0;

// The max number of priorities provided by the target platform.
// TODO: This should be defined in ACE (somehow) and only mapped here
// to some variables (and even that is doubtful).
const long ACE_Scheduler_MAX_PRIORITIES = 5;

const TimeBase::TimeT ACE_Scheduler_Rates[ACE_Scheduler_MAX_PRIORITIES] = {
     250000, // 40 Hz
     500000, // 20 Hz
    1000000, // 10 Hz
    2000000, // 5 Hz
   10000000  // 1 Hz
};

// The minimum preemption priority.
const long ACE_Scheduler_MIN_PREEMPTION_PRIORITY =
      ACE_Scheduler_MAX_PRIORITIES - 1;
// Max preemption priority.
const long ACE_Scheduler_MAX_PREEMPTION_PRIORITY = 0;

// This is the maximum sub priority.
const u_long ACE_Scheduler_MAX_SUB_PRIORITY =
      ACE_Message_Block::MB_USER - 1;

// This is the minimum sub priority.
const u_long ACE_Scheduler_MIN_SUB_PRIORITY =
      ACE_Message_Block::MB_NORMAL;

typedef ACE_SYNCH_MUTEX ACE_ES_MUTEX;
typedef ACE_Guard<ACE_ES_MUTEX> ACE_ES_GUARD;

typedef ACE_SYNCH_RW_MUTEX ACE_ES_RW_LOCK;
typedef ACE_Write_Guard<ACE_ES_RW_LOCK> ACE_ES_WGUARD;
typedef ACE_Read_Guard<ACE_ES_RW_LOCK> ACE_ES_RGUARD;

// ************************************************************
// Channel configuration parameters.

// ACE_ES_DISPATCHING_MODULE determines what Dispatching Module is
// used by the channel.  ACE_ES_Dispatching_Base is a null dispatcher
// that can be used to build an EFD.
#define ACE_ES_DISPATCHING_MODULE ACE_ES_Priority_Dispatching
//#define ACE_ES_DISPATCHING_MODULE ACE_ES_Dispatching_Base

// This is the number of threads spawned for *each* priority queue in
// the channel's Dispatching Module.  This is used by the channel when
// creating the dispatch module.
const int THREADS_PER_DISPATCH_QUEUE = 1;

// This is the chunk size of the cached allocator for
// ACE_ES_Event_Container.
const int ACE_ES_EVENT_CONTAINER_MEMORY_POOL = 1024;
const int ACE_ES_EVENT_MEMORY_POOL = 1024;
const int ACE_ES_DISPATCH_REQUEST_MEMORY_POOL = 1024;

#endif /* ACE_ES_CONSTANTS_H */
