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

#include "ace/inc_user_config.h"

// The templatized method parameters needed by this file are
// hopelessly broken on pre-2.8 versions of g++
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)

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

#endif /* __GNUC__ */

#include "ace/post.h"
#endif /* TAO_RECONFIG_SCHEDULER_H */
