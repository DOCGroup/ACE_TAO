// -*- C++ -*-

//=======================================================================
/**
 *  @file    Push_Handler.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_PUSH_HANDLER_H
#define TAO_LB_PUSH_HANDLER_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingC.h"

#include "ace/Event_Handler.h"


/**
 * @class TAO_LB_Push_Handler
 *
 * @brief Event handler used when the "push" monitoring style is
 *        configured.
 *
 * An event handler designed to push loads to the LoadManager.
 */
class TAO_LB_Push_Handler : public ACE_Event_Handler
{
public:

  /// Constructor
  TAO_LB_Push_Handler (CosLoadBalancing::LoadMonitor_ptr monitor,
                       const PortableGroup::Location & location,
                       CosLoadBalancing::LoadManager_ptr manager);

  /// Receive the timeout event.
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);

private:

  /// Reference to the LoadMonitor.
  CosLoadBalancing::LoadMonitor_var monitor_;

  /// Location where the LoadMonitor resides.
  const PortableGroup::Location location_;

  /// Reference to the LoadManager.
  CosLoadBalancing::LoadManager_var manager_;

};

#include /**/ "ace/post.h"

#endif  /* TAO_LB_PUSH_HANDLER_H */
