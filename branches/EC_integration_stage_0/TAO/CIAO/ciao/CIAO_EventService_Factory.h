// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventService_Factory.h
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTSERVICE_FACTORY_H
#define CIAO_EVENTSERVICE_FACTORY_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_EventServiceBase.h"

namespace CIAO
{
  /**
   * @class EventService_Factory
   *
   * An abstract factory that creates event configuration and service objects.
   * A concrete implementation is obtained by CIAO::Container via the ACE
   * service configurator.
   */
  class CIAO_EVENTS_Export EventService_Factory :
    public ACE_Service_Object
  {

  public:

    /**
     * @fn ~EventService_Factory (void)
     *
     * Destructor.
     */
    virtual ~EventService_Factory (void);

    /**
     * @fn Consumer_Config_ptr create_consumer_config (EventServiceType type)
     *
     * Creates and returns a new Consumer_Config object corresponding to the
     * type specified by @c type.
     */
    virtual Consumer_Config_ptr create_consumer_config (
      EventServiceType type) = 0;

    /**
     * @fn Supplier_Config_ptr create_supplier_config (EventServiceType type)
     *
     * Creates and returns a new Supplier_Config object corresponding to the
     * type specified by @c type.
     */
    virtual Supplier_Config_ptr create_supplier_config (
      EventServiceType type) = 0;

    /**
     * @fn EventServiceBase * create (EventServiceType type)
     *
     * Creates and returns a new EventServiceBase object corresponding to the
     * type specified by @c type.
     */
    virtual EventServiceBase * create (
      EventServiceType type) = 0;

    virtual void init (
      CORBA::ORB_ptr orb,
      PortableServer::POA_ptr poa) = 0;

  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_EVENTSERVICE_FACTORY_H */
