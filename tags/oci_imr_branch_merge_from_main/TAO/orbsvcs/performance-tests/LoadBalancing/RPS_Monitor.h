// -*- C++ -*-

//=============================================================================
/**
 *  @file RPS_Monitor.h
 *
 *  $Id$
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
  : public virtual POA_CosLoadBalancing::LoadMonitor,
    public virtual PortableServer::RefCountServantBase
{
public:

  /// Constructor
  RPS_Monitor (ServerRequestInterceptor * interceptor);

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
  virtual CosLoadBalancing::Location * the_location (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the average CPU load at the location which this
  /// LoadMonitor resides.
  /**
   * @return A "Load" sequence of length 1 that contains a LoadId
   *         equal to CosLoadBalancing::CPU, and the average CPU
   *         load.
   */
  virtual CosLoadBalancing::LoadList * loads (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * reference counting.
   */
  ~RPS_Monitor (void);

private:

  /// The name of the location at which this LoadMonitor resides.
  CosLoadBalancing::Location location_;

  ServerRequestInterceptor * interceptor_;

  ACE_Time_Value last_time_;

  ACE_SYNCH_MUTEX lock_;
};

#include /**/ "ace/post.h"

#endif  /* TAO_RPS_MONITOR_H */
