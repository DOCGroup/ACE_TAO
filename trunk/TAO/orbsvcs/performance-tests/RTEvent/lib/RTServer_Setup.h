/**
 * @file RTServer_Setup.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_RTSERVER_SETUP_H
#define TAO_PERF_RTEC_RTSERVER_SETUP_H

#include "RTCORBA_Setup.h"
#include "RTPOA_Setup.h"
#include "PriorityBand_Setup.h"
#include "SyncScope_Setup.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RT_Class;

/**
 * @class RTServer_Setup
 *
 * @brief Simplify the initialization of performance tests.
 *
 */
class TAO_RTEC_Perf_Export RTServer_Setup
{
public:
  /// Constructor
  RTServer_Setup (int use_rt_corba,
                  CORBA::ORB_ptr orb,
                  const RT_Class &rt_class
                  ACE_ENV_ARG_DECL);

  /// Return the POA configured by the RTPOA_Setup
  PortableServer::POA_ptr poa (void);

  /// Return the underlying RTCORBA_Setup pointer
  RTCORBA_Setup *rtcorba_setup (void);

private:
  auto_ptr<RTCORBA_Setup> rtcorba_setup_;
  auto_ptr<RTPOA_Setup> rtpoa_setup_;
  auto_ptr<PriorityBand_Setup> priorityband_setup_;
  SyncScope_Setup syncscope_setup_;
};

#if defined(__ACE_INLINE__)
#include "RTServer_Setup.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_RTSERVER_SETUP_H */
