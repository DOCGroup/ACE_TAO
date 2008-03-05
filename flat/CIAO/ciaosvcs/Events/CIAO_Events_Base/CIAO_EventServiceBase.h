// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventServiceBase.h
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTSERVICEBASE_H
#define CIAO_EVENTSERVICEBASE_H

#include /**/ "ace/pre.h"

#include "CIAO_Events_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_EventsS.h"

namespace CIAO
{
  /**
   * @class EventServiceBase
   *
   * An abstract base servant class to implement the CIAO_Event_Service
   * interface. The derived classes will provide appropriate
   * implementations of the connect, disconnect, and push methods depending on
   * the event mechanism used.
   */
  class CIAO_EVENTS_Export EventServiceBase
    : public virtual POA_CIAO::CIAO_Event_Service
  {
  public:
    EventServiceBase (void);

    virtual ~EventServiceBase (void);

    /// A factory method for Supplier_Config objects
    virtual Supplier_Config_ptr create_supplier_config (void) = 0;

    /// A factory method for Consumer_Config objects
    virtual Consumer_Config_ptr create_consumer_config (void) = 0;

    /**
     * @fn void connect_event_supplier (Supplier_Config_ptr supplier_config)
     *
     * Connects an event supplier using the options specified by
     * @c supplier_config.
     */
    virtual void connect_event_supplier (Supplier_Config_ptr supplier_config) = 0;

    /**
     * @fn void connect_event_consumer (Consumer_Config_ptr consumer_config)
     *
     * Connects an event consumer using the options specified by
     * @c consumer_config.
     */
    virtual void connect_event_consumer (Consumer_Config_ptr consumer_config) = 0;

    /**
     * @fn void disconnect_event_supplier ()
     *
     * Disconnects the event supplier associated with this object.
     */
    virtual void disconnect_event_supplier (const char * consumer_id) = 0;

    /**
     * @fn void disconnect_event_consumer (CONNECTION_ID consumer_id)
     *
     * Disconnects the event consumer with UUID @c consumer_id.
     */
    virtual void disconnect_event_consumer (const char * consumer_id) = 0;

    /**
     * @fn void push_event (Components::EventBase * ev)
     *
     * Pushes event @c ev to all consumers.
     */
    virtual void push_event (Components::EventBase * ev) = 0;

    /**
     * Pushes event @c ev to all consumers. The source id of the
     * supplier is specified through @c source_id.
     */
    virtual void ciao_push_event (::Components::EventBase * evt,
                                  const char * source_id,
                                  ::CORBA::TypeCode_ptr tc);
  };

  class Event_Consumer_Config_Base
    : public virtual POA_CIAO::Consumer_Config
  {
  public:
    virtual void start_conjunction_group (::CORBA::Long size) = 0;

    virtual void start_disjunction_group (::CORBA::Long size) = 0;

    virtual void insert_source (const char * source_id) = 0;

    virtual void insert_type (::CORBA::Long event_type) = 0;
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_EVENTSERVICEBASE_H */
