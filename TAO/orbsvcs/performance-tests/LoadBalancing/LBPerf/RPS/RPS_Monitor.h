// -*- C++ -*-

//=============================================================================
/**
 *  @file RPS_Monitor.h
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_RPS_MONITOR_H
#define TAO_RPS_MONITOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosLoadBalancingS.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

class ServerRequestInterceptor;

/**
 * @class RPS_Monitor
 *
 * @brief LoadMonitor implementation that calculates the number of
 *        request arriving per second.
 */
class RPS_Monitor
  : public virtual POA_CosLoadBalancing::LoadMonitor
{
public:
  /// Constructor
  explicit RPS_Monitor (ServerRequestInterceptor *interceptor);
  RPS_Monitor (RPS_Monitor const &) = delete;
  RPS_Monitor (RPS_Monitor &&) = delete;
  RPS_Monitor &operator= (RPS_Monitor const &) = delete;
  RPS_Monitor &operator= (RPS_Monitor &&) = delete;

  /**
   * @name CosLoadBalancing::LoadMonitor Methods
   *
   * Methods required by the CosLoadBalancing::LoadMonitor interface.
   */
  //@{
  /// Return the location at which the LoadMonitor resides.
  /**
   * The returned "Location" is a sequence of length 1.
   */
  CosLoadBalancing::Location *the_location () override;

  /// Return the average CPU load at the location which this
  /// LoadMonitor resides.
  /**
   * @return A "Load" sequence of length 1 that contains a LoadId
   *         equal to CosLoadBalancing::LoadAverage, and the average CPU
   *         load.
   */
  CosLoadBalancing::LoadList *loads () override;

  //@}

protected:
  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * reference counting.
   */
  ~RPS_Monitor () override = default;

private:
  /// The name of the location at which this LoadMonitor resides.
  CosLoadBalancing::Location location_;

  ServerRequestInterceptor * interceptor_;

  ACE_Time_Value last_time_;

  TAO_SYNCH_MUTEX lock_;
};

#include /**/ "ace/post.h"

#endif  /* TAO_RPS_MONITOR_H */
