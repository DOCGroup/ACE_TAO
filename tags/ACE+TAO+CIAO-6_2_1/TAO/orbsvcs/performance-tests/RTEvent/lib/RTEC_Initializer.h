/**
 * @file RTEC_Initializer.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_RTEC_INITIALIZER_H
#define TAO_PERF_RTEC_RTEC_INITIALIZER_H

#include "rtec_perf_export.h"
#include "tao/ORB.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/PortableServer/PortableServer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_EC_Event_Channel;
TAO_END_VERSIONED_NAMESPACE_DECL

class RTCORBA_Setup;

/**
 * @class RTEC_Initializer
 *
 * @brief Implement a helper method to initialize and instance of
 * TAO's Real-time Event Service
 *
 */
class TAO_RTEC_Perf_Export RTEC_Initializer
{
public:
  /// Return a new event channel
  static TAO_EC_Event_Channel *create (PortableServer::POA_ptr consumer_poa,
                                       PortableServer::POA_ptr supplier_poa,
                                       RTCORBA_Setup *rtcorba_setup);
};

#endif /* TAO_PERF_RTEC_RTEC_INITIALIZER_H */
