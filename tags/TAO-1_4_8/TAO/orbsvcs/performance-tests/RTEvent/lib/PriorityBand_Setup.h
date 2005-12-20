/**
 * @file PriorityBand_Setup.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_PRIORITYBAND_SETUP_H
#define TAO_PERF_RTEC_PRIORITYBAND_SETUP_H

#include "rtec_perf_export.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RTCORBA_Setup;

/**
 * @class PriorityBand_Setup
 *
 * @brief Simplify the initialization of performance tests.
 *
 */
class TAO_RTEC_Perf_Export PriorityBand_Setup
{
public:
  /// Constructor
  PriorityBand_Setup (CORBA::ORB_ptr orb,
                      const RTCORBA_Setup &rtcorba_setup
                      ACE_ENV_ARG_DECL);

  /// Destructor
  /**
   * It is a no-op, but it shuts up g++
   */
  ~PriorityBand_Setup (void);
};

#if defined(__ACE_INLINE__)
#include "PriorityBand_Setup.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_PRIORITYBAND_SETUP_H */
