/**
 * @file SyncScope_Setup.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_SYNCSCOPE_SETUP_H
#define TAO_PERF_RTEC_SYNCSCOPE_SETUP_H

#include "rtec_perf_export.h"
#include "tao/Messaging/Messaging.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class SyncScope_Setup
 *
 * @brief Simplify the initialization of performance tests.
 *
 */
class TAO_RTEC_Perf_Export SyncScope_Setup
{
public:
  /// Constructor
  SyncScope_Setup (CORBA::ORB_ptr orb
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  SyncScope_Setup (CORBA::ORB_ptr orb,
                   Messaging::SyncScope value
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Destructor
  /**
   * It is a no-op, but it shuts up g++
   */
  ~SyncScope_Setup (void);

private:
  /// Implement the shared code between both constructors
  void init (CORBA::ORB_ptr orb,
             Messaging::SyncScope value
             ACE_ENV_ARG_DECL);
};

#if defined(__ACE_INLINE__)
#include "SyncScope_Setup.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_SYNCSCOPE_SETUP_H */
