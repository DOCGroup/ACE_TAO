// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_CPU_Load_Average_Monitor.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_CPU_LOAD_AVERAGE_MONITOR_H
#define TAO_LB_CPU_LOAD_AVERAGE_MONITOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/LoadBalancing/LoadBalancing_export.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LB_CPU_Load_Average_Monitor
 *
 * @brief LoadMonitor implementation that monitors the overall CPU
 *        load on a given host.
 *
 * Loads returned from this load monitor are the average number of
 * processes in the "run" queue over a ***FIXME*** seconds averaged
 * over the number of processors.  For example, a load of 2 on a dual
 * CPU host is returned as an average load of 1 by this CPU load
 * monitor, a load of 5 on a quad CPU host will be reported as a load
 * of 1.25 (i.e. 5/4), and so on and so forth.
 */
class TAO_LoadBalancing_Export TAO_LB_CPU_Load_Average_Monitor
  : public virtual POA_CosLoadBalancing::LoadMonitor
{
public:

  /// Constructor
  /**
   * If no location is supplied the hostname or IP address is used by
   * default.
   */
  TAO_LB_CPU_Load_Average_Monitor (const ACE_TCHAR * location_id = 0,
                                   const ACE_TCHAR * location_kind = 0);

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
  virtual CosLoadBalancing::Location * the_location ();

  /// Return the average CPU load at the location which this
  /// LoadMonitor resides.
  /**
   * @return A "Load" sequence of length 1 that contains a LoadId
   *         equal to CosLoadBalancing::LoadAverage, and the average CPU
   *         load.
   */
  virtual CosLoadBalancing::LoadList * loads ();

  //@}

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management through
   * reference counting.
   */
  ~TAO_LB_CPU_Load_Average_Monitor (void);

private:

  /// The name of the location at which this LoadMonitor resides.
  CosLoadBalancing::Location location_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LB_CPU_LOAD_AVERAGE_MONITOR_H */
