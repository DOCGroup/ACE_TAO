/**
 * @file RTPOA_Setup.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_RTPOA_SETUP_H
#define TAO_PERF_RTEC_RTPOA_SETUP_H

#include "rtec_perf_export.h"
#include "tao/RTPortableServer/RTPortableServer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RTCORBA_Setup;

/**
 * @class RTPOA_Setup
 *
 * @brief Simplify the initialization of performance tests.
 *
 */
class TAO_RTEC_Perf_Export RTPOA_Setup
{
public:
  /// Constructor
  RTPOA_Setup (CORBA::ORB_ptr orb,
               const RTCORBA_Setup &rtcorba_setup
               ACE_ENV_ARG_DECL);

  PortableServer::POA_ptr poa (void);

private:
  PortableServer::POA_var poa_;
};

#if defined(__ACE_INLINE__)
#include "RTPOA_Setup.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_RTPOA_SETUP_H */
