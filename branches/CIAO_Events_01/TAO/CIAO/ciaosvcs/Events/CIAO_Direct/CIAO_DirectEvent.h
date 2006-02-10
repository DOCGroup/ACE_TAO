// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_DirectEvent.h
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_DIRECTEVENT_H
#define CIAO_DIRECTEVENT_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciaosvcs/Events/CIAO_EventServiceBase.h"
#include "CIAO_DirectEventS.h"

#include "ace/Array.h"

namespace CIAO
{
  /**
   * @class DirectEventService
   *
   * An implementation of EventServiceBase using direct communication.
   */
  class CIAO_EVENTS_Export DirectEventService :
    public virtual EventServiceBase
  {
  public:

    DirectEventService (CORBA::ORB_ptr orb,
                        PortableServer::POA_ptr poa);

    virtual void
    connect_event_supplier (
        Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (( CORBA::SystemException));

    virtual void
    connect_event_consumer (
        CIAO::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void
    disconnect_event_consumer (
        const char * connection_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void
    disconnect_event_supplier (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void
    push_event (
        Components::EventBase * ev
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    // Reference to the ORB
    CORBA::ORB_var orb_;

    // Reference to the Root POA
    PortableServer::POA_var root_poa_;

    /**
     * @var ACE_Array<Components::EventConsumerBase_ptr> consumer_array_
     *
     * List of consumers.
     */
    /// @@ George, this is cool! Do you want to use a _var or _ptr?
    ACE_Array<Components::EventConsumerBase_var> consumer_array_;

  };


  /**
   * @class Direct_Consumer_Config_impl
   *
   * Implementation of the Direct_Consumer_Config IDL interface that
   * configures TAO's direct event mechanism. An object of this type will be
   * returned from @c CIAO::Container::create_consumer_config () when @c DIRECT
   * is specified as the event service type.
   */
  class Direct_Consumer_Config_impl :
    public virtual POA_CIAO::Direct_Consumer_Config,
    public virtual PortableServer::RefCountServantBase
  {

  public:

    Direct_Consumer_Config_impl (PortableServer::POA_ptr poa);

    virtual ~Direct_Consumer_Config_impl (void);

    virtual void consumer_id (const char * consumer_id
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CONNECTION_ID consumer_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void supplier_id (const char * supplier_id
                              ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CONNECTION_ID supplier_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void consumer (Components::EventConsumerBase_ptr consumer ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual Components::EventConsumerBase_ptr consumer (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // @@ george, probably this should be const'ified?
    virtual EventServiceType service_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    ACE_CString consumer_id_;

    ACE_CString supplier_id_;

    Components::EventConsumerBase_var consumer_;

    EventServiceType service_type_;

    PortableServer::POA_var poa_;

  };

  /**
   * @class Direct_Supplier_Config_impl
   *
   * Implementation of the Direct_Supplier_Config IDL interface that
   * configures TAO's direct event mechanism. An object of this type will be
   * returned from @c CIAO::Container::create_supplier_config () when @c DIRECT
   * is specified as the event service type.
   */
  class Direct_Supplier_Config_impl :
    public virtual POA_CIAO::Direct_Supplier_Config,
    public virtual PortableServer::RefCountServantBase
  {

  public:

    Direct_Supplier_Config_impl (PortableServer::POA_ptr poa);

    virtual ~Direct_Supplier_Config_impl (void);

    void supplier_id (const char * supplier_id ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    char * supplier_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType service_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    ACE_CString supplier_id_;

    EventServiceType service_type_;

    PortableServer::POA_var poa_;

  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_DIRECTEVENT_H */
