/**
 * @file EC_Destroyer.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_EC_DESTROYER_H
#define TAO_PERF_RTEC_EC_DESTROYER_H

#include "rtec_perf_export.h"
#include "orbsvcs/orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC_Destroyer
 *
 * @brief Implement a helper class to destroy a Real-time Event
 * Channel.
 *
 */
class TAO_RTEC_Perf_Export EC_Destroyer
{
public:
  /// Constructor
  EC_Destroyer (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Destructor
  /**
   * @todo This method could benefit from the error logging described
   * in Servant_var.cpp
   */
  ~EC_Destroyer (void);

private:
  /// The event channel
  RtecEventChannelAdmin::EventChannel_var ec_;
};

#if defined(__ACE_INLINE__)
#include "EC_Destroyer.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_EC_DESTROYER_H */
