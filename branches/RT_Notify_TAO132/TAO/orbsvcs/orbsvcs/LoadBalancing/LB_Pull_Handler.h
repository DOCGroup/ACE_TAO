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

#include "LB_MonitorMap.h"

#include "ace/Event_Handler.h"


/// Forward declarations.
class TAO_LB_LoadManager;


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
  TAO_LB_Pull_Handler (void);

  /// Receive the timeout event.
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);

  /// Initialize this event handler.
  void initialize (TAO_LB_MonitorMap * monitor_map,
                   TAO_LB_LoadManager * load_manager);

private:

  /// The table that contains all load monitors registered with the
  /// load balancer.
  TAO_LB_MonitorMap * monitor_map_;

  /// Pointer to the LoadManager servant.
  TAO_LB_LoadManager * load_manager_;

};
#include "ace/post.h"

#endif  /* TAO_LB_PULL_HANDLER_H */
