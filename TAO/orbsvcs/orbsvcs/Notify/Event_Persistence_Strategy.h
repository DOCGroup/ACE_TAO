/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Persistence_Strategy.h
 *
 *  $Id$
 *
 *  A factory class that creates a Routing_Slip_Persistence_Manager.
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef EVENT_PERSISTENCE_STRATEGY_H
#define EVENT_PERSISTENCE_STRATEGY_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "notify_serv_export.h"

#include "ace/Service_Object.h"

namespace TAO_Notify
{
  class Event_Persistence_Factory;

  /// Interface to be implemented by specific strategies
  class TAO_Notify_Serv_Export Event_Persistence_Strategy: public ACE_Service_Object
  {
  public:

    // get the current factory, creating it if necessary
    virtual Event_Persistence_Factory * get_factory () = 0;

    // release the current factory so a new one can be created
    virtual void reset () = 0;
  };

} // namespace TAO_Notify

#include /**/ "ace/post.h"
#endif /* EVENT_PERSISTENCE_STRATEGY_H */
