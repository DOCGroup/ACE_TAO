// ============================================================================
//
// $Id$
//
// ============================================================================

#ifndef ACE_SCHEDULER_UTILITIES_H
#define ACE_SCHEDULER_UTILITIES_H

#include "ace/OS.h"

#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ORBSVCS_Export ACE_RT_Info : public RtecScheduler::RT_Info
  // = TITLE
  //   Offers a convenient C++ interface to the IDL RT_Info structure.
  //
  // = DESCRIPTION
  //   For performance reasons the RT_Info data is represented as an
  //   IDL structure, this permits sending complete RT_Info's from the
  //   client to the server.
  //   Unfortunately this precludes the usage of member functions and
  //   constructors, this class serves as a helper to implement those
  //   without loosing the performance on IDL.
{
public:
  ACE_RT_Info (const char* entry_point,
               RtecScheduler::Time worst_time,
               RtecScheduler::Time typical_time,
               RtecScheduler::Time cached_time,
               RtecScheduler::Period period,
               RtecScheduler::Importance importance,
               RtecScheduler::Quantum quantum,
               CORBA::Long threads);

  ACE_RT_Info (const RtecScheduler::RT_Info& rt_info);

  int add_dependency(RtecScheduler::handle_t dep,
                     int number_of_calls = 1);
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Scheduler_Utilities.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SCHEDULER_UTILITIES_H */
