/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Persistence_Factory.h
 *
 *  $Id$
 *
 *  A factory class that creates a Routing_Slip_Persistence_Manager.
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef EVENT_PERSISTENCE_FACTORY_H
#define EVENT_PERSISTENCE_FACTORY_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "notify_serv_export.h"

namespace TAO_Notify
{
  class Routing_Slip_Persistence_Manager;
  class Persistent_Callback;

  /// interface to be implemented by specific Event_Persistence_Factories
  class TAO_Notify_Serv_Export Event_Persistence_Factory
  {
  public:
    /// Create a Persistence Manager
    virtual Routing_Slip_Persistence_Manager *
      create_routing_slip_persistence_manager (Persistent_Callback * callback) = 0;

    /// Begin the reload process by returning the first Routing_Slip_Persistence_Manager
    /// to continue call Routing_Slip_Persistence_Manager::load_next ()
    virtual Routing_Slip_Persistence_Manager * first_reload_manager () = 0;

  };
} // namespace TAO_Notify

#include /**/ "ace/post.h"
#endif /* EVENT_PERSISTENCE_FACTORY_H */
