/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Scheduler.h
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHEDULER_H
#define TAO_RECONFIG_SCHEDULER_H
#include "ace/pre.h"

#include "ace/config-all.h"


#include "ace/Hash_Map_Manager.h"
#include "ace/RB_Tree.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "Reconfig_Sched_Utils.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (__ACE_INLINE__)
#include "Reconfig_Scheduler.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "Reconfig_Scheduler_T.h"


#include "ace/post.h"
#endif /* TAO_RECONFIG_SCHEDULER_H */
