// -*- C++ -*-

//=======================================================================
/**
 *  @file    LB_Pull_Handler.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_PULL_HANDLER_H
#define TAO_LB_PULL_HANDLER_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingC.h"

#include "LB_ReplicaInfo.h"
#include "LB_Location_Map.h"


/**
 * @class TAO_LB_Pull_Handler
 *
 * @brief Event handler used when the "pull" monitoring style is
 *        configured.
 *
 * An event handler designed to pull loads from all load monitors
 * registered with the load balancer.
 */
class TAO_LB_Pull_Handler : public ACE_Event_Handler
{
public:

  /// Constructor
  TAO_LB_Pull_Handler (TAO_LB_Location_Map &location_map);

  /// Receive the timeout event.
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);

private:

  /// The table that contains all object groups registered with the
  /// load balancer.
  TAO_LB_Location_Map &location_map_;

};
#include "ace/post.h"

#endif  /* TAO_LB_PULL_HANDLER_H */
