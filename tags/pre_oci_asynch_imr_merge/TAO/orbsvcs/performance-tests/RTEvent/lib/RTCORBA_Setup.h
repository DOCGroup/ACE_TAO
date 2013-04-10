/**
 * @file RTCORBA_Setup.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_RTCORBA_SETUP_H
#define TAO_PERF_RTEC_RTCORBA_SETUP_H

#include "rtec_perf_export.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RT_Class;

/**
 * @class RTCORBA_Setup
 *
 * @brief Simplify the initialization of performance tests.
 *
 */
class TAO_RTEC_Perf_Export RTCORBA_Setup
{
public:
  /// Constructor
  RTCORBA_Setup (CORBA::ORB_ptr orb,
                 const RT_Class &rtclass,
                 int nthreads);

  /// Return the thread pool lanes appropriate for the benchamrks
  const RTCORBA::ThreadpoolLanes & lanes (void) const;

  /// Return the priority of the process
  RTCORBA::Priority process_priority (void) const;

private:
  void setup_lane (int priority, RTCORBA::ThreadpoolLane &lane
 );

private:
  RTCORBA::PriorityMappingManager_var priority_mapping_manager_;

  RTCORBA::ThreadpoolLanes lanes_;

  RTCORBA::Priority process_priority_;

  /// Just a plain pointer, the ORB owns the memory...
  RTCORBA::PriorityMapping *priority_mapping_;
};

#if defined(__ACE_INLINE__)
#include "RTCORBA_Setup.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_RTCORBA_SETUP_H */
