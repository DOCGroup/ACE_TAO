// -*- C++ -*-

//=============================================================================
/**
 *  @file    CIAO_Events.h
 *
 *  $Id$
 *
 *  Header file for CIAO's event classes
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTS_H
#define CIAO_EVENTS_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "CCM_ContainerC.h"

namespace CIAO
{

  class CIAO_EventServiceBase
  {

  public:

    virtual ::Components::Cookie * specify_event_service (
        const char * event_name,
        const char * publisher_name,
        const char * service_name
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

    virtual ::Components::Cookie * connect_event_supplier (
        ::CIAO::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

    virtual ::Components::Cookie * connect_event_consumer (
        ::Components::EventConsumerBase_ptr c,
        ::CIAO::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

    virtual void disconnect_event_consumer (
        ::Components::Cookie *ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::InvalidName,
        ::Components::InvalidConnection)) = 0;

    virtual void disconnect_event_supplier (
        ::Components::Cookie *ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::InvalidName,
        ::Components::InvalidConnection)) = 0;

    virtual void push_event (
        ::Components::EventBase *ev,
        ::Components::Cookie *ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

  };

  class CIAO_RTEventService :
    public virtual CIAO_EventServiceBase
  {

  public:

    virtual ::Components::Cookie * specify_event_service (
        const char * event_name,
        const char * publisher_name,
        const char * service_name
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual ::Components::Cookie * connect_event_supplier (
        CIAO_EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual ::Components::Cookie * connect_event_consumer (
        ::Components::EventConsumerBase_ptr c,
        CIAO_EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void disconnect_event_consumer (
        ::Components::Cookie *ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::InvalidName,
        ::Components::InvalidConnection));

    virtual void disconnect_event_supplier (
        ::Components::Cookie *ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::InvalidName,
        ::Components::InvalidConnection));

    virtual void push_event (
        ::Components::EventBase *ev,
        ::Components::Cookie *ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    /// Map of event types
    ACE_Hash_Map_Manager<const char *, RtecEventComm::EventType, ACE_Null_Mutex>
      ciao_event_types_map_;

    /// Map of suppliers
    ACE_Hash_Map_Manager<const char *, RtecEventComm::EventSourceID, ACE_Null_Mutex>
      ciao_publishers_map_;

    /// Reference to the RT event channel
    RtecEventChannelAdmin::EventChannel_var ciao_rt_event_channel_;

  };
      

  enum CIAO_EventServiceType
  {
    DIRECT,
    EC,
    RTEC,
    NS,
    RTNS
  };

  struct CIAO_EventServiceInfo
  {
    EventServiceType service_type;
    RtecEventComm::EventType event_type_id;
    RtecEventComm::EventSourceID event_source_id;
  };

  class CIAO_RTEventServiceSupplier_impl :
    public virtual POA_RtecEventComm::PushSupplier,
    public virtual PortableServer::RefCountServantBase
  {

  public:
    CIAO_RTEventServiceSupplier_impl (void);

    CIAO_RTEventServiceSupplier_impl (CORBA::ORB_ptr orb);

    virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    CORBA::ORB_var orb_;
  };

  class CIAO_RTEventServiceConsumer_impl :
    public virtual POA_RtecEventComm::PushConsumer,
    public virtual PortableServer::RefCountServantBase
  {

  public:
    CIAO_RTEventServiceConsumer_impl (void);

    CIAO_RTEventServiceConsumer_impl (CORBA::ORB_ptr orb,
      Components::EventConsumerBase_ptr consumer);

    virtual void push (const RtecEventComm::EventSet& events);

    virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    CORBA::ORB_var orb_;
    Components::EventConsumerBase_var event_consumer_;
  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_EVENTS_H */
