// ============================================================================
//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Scheduler.cpp
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHEDULER_C
#define TAO_RECONFIG_SCHEDULER_C

#include "Reconfig_Scheduler_T.h"
#include "Reconfig_Scheduler.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "Reconfig_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Sched, Reconfig_Scheduler, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template void maintain_scheduling_array <> (TAO_Reconfig_Scheduler_Entry **&,
                                            long&, RtecScheduler::handle_t ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException));

template void maintain_scheduling_array <> (TAO_RT_Info_Tuple **&, long&,
                                   RtecScheduler::handle_t ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException));
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
template void maintain_scheduling_array <> (TAO_Reconfig_Scheduler_Entry **&,
                                            long&, RtecScheduler::handle_t ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException));

template void maintain_scheduling_array <> (TAO_RT_Info_Tuple **&, long&,
                                   RtecScheduler::handle_t ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException));
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_RECONFIG_SCHEDULER_C */
