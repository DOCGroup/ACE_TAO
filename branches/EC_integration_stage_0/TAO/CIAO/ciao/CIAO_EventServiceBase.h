// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventServiceBase.h
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTSERVICEBASE_H
#define CIAO_EVENTSERVICEBASE_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_EventsS.h"

namespace CIAO
{

  /**
   * @class EventServiceBase
   *
   * An abstract base class; derived classes will provide appropriate
   * implementations of the connect, disconnect, and push methods depending on
   * the event mechanism used. @c EventServiceBase objects are created on a
   * 1-per-supplier basis. Each event source and sink has an associated
   * @c EventServiceBase object, stored in a map in CIAO::Container.
   */
  // @@ George, any particular reason that this cannot be a local
  /// interface in an idl? The above struct  can also be pushed into
  /// an IDL, unless you don't want to have object semantics.
  class CIAO_EVENTS_Export EventServiceBase
  {

  public:

    /**
     * @fn void connect_event_supplier (Supplier_Config_ptr supplier_config)
     * 
     * Connects an event supplier using the options specified by
     * @c supplier_config.
     */
    virtual void connect_event_supplier (
        Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

    /**
     * @fn void connect_event_consumer (Consumer_Config_ptr consumer_config)
     * 
     * Connects an event consumer using the options specified by
     * @c consumer_config.
     */
    virtual void connect_event_consumer (
        Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

    /**
     * @fn void disconnect_event_supplier ()
     * 
     * Disconnects the event supplier associated with this object.
     */
    virtual void disconnect_event_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection)) = 0;

    /**
     * @fn void disconnect_event_consumer (CONNECTION_ID consumer_id)
     * 
     * Disconnects the event consumer with UUID @c consumer_id.
     */
    virtual void disconnect_event_consumer (
        const char * consumer_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection)) = 0;

    /**
     * @fn void push_event (Components::EventBase * ev)
     * 
     * Pushes event @c ev to all consumers.
     */
    virtual void push_event (
        Components::EventBase * ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_EVENTSERVICEBASE_H */
