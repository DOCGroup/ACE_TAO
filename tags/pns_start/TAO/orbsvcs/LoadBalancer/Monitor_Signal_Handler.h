// -*- C++ -*-

//=============================================================================
/**
 * @file   Monitor_Signal_Handler.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_MONITOR_SIGNAL_HANDLER_H
#define TAO_LB_MONITOR_SIGNAL_HANDLER_H

#include "Signal_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingC.h"


/**
 * @class TAO_LB_Monitor_Signal_Handler
 *
 * @brief LoadMonitor-specific signal handler.
 *
 * This class simply builds on the TAO_LB_Signal_Handler class to add
 * some LoadMonitor-specific cleanup.
 */
class TAO_LB_Monitor_Signal_Handler : public TAO_LB_Signal_Handler
{
public:

  /// Constructor.
  TAO_LB_Monitor_Signal_Handler (
    CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa,
    CosLoadBalancing::LoadManager_ptr load_manager,
    const PortableGroup::Location & location);

protected:

  /// Template method that initiates the cleanup process.
  virtual int perform_cleanup (int signum);

private:

  /// Reference to the LoadManager with which the LoadMonitor is
  /// registered.
  CosLoadBalancing::LoadManager_var load_manager_;

  /// Reference to the location the LoadMonitor resides at.
  const PortableGroup::Location & location_;

};


#endif  /* TAO_LB_MONITOR_SIGNAL_HANDLER_H */
