// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_CosNotify.h
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_COSNOTIFY_H
#define CIAO_COSNOTIFY_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_EventServiceBase.h"
#include "CIAO_CosNotifyS.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"

namespace CIAO
{
  /**
   * @class CosNotifyService
   *
   * @brief An implementation of EventServiceBase to use the
   * CosNotification service.
   */
  class CosNotifyService :
    public virtual EventServiceBase
  {

  public:

    CosNotifyService (CORBA::ORB_ptr orb,
                      PortableServer::POA_ptr poa,
                      CosNotifyChannelAdmin::EventChannel_ptr ec);

    // @@ George, just a place holder for you to add documentation.
    virtual void connect_event_supplier (
        CIAO::Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void connect_event_consumer (
        CIAO::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void disconnect_event_consumer (
        const char * connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection));

    virtual void disconnect_event_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection));

    virtual void push_event (
        Components::EventBase * ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    // Reference to the ORB
    CORBA::ORB_var orb_;

    // Reference to the Root POA
    PortableServer::POA_var root_poa_;

    /**
     * @var CosNotifyChannelAdmin::EventChannel_var notify_channel_
     *
     * Reference to the notification channel.
     */
    CosNotifyChannelAdmin::EventChannel_var notify_channel_;

    /**
     * @var ACE_CString type_id_
     *
     * The type of event.
     */
    ACE_CString type_id_;

    /**
     * @var ACE_CString source_id_
     *
     * The supplier id.
     */
    ACE_CString source_id_;

    /**
     * @var CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_
     *
     * The proxy consumer to which events are pushed.
     */
    CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;

    /**
     * @var ACE_Hash_Map_Manager<> proxy_supplier_map_
     *
     * Mapping of each event sink to a proxy supplier for disconnect purposes.
     */
    ACE_Hash_Map_Manager<ACE_CString,
                         CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr,
                         ACE_Null_Mutex> proxy_supplier_map_;

  };

  /**
   * @class CosNotifyServiceSupplier_impl
   *
   * An implementation of the StructuredPushSupplier interface.
   */
  class CosNotifyServiceSupplier_impl :
    public virtual POA_CosNotifyComm::StructuredPushSupplier,
    public virtual PortableServer::RefCountServantBase
  {

  public:

    // @@ George, is it okay to have a default constructor?
    CosNotifyServiceSupplier_impl (void);

    CosNotifyServiceSupplier_impl (
      CORBA::ORB_ptr orb);

    virtual void disconnect_structured_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void subscription_change (
        const CosNotification::EventTypeSeq& events_added,
        const CosNotification::EventTypeSeq& events_removed
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType));

  private:

    CORBA::ORB_var orb_;
  };


  /**
   * @class CosNotifyServiceConsumer_impl
   *
   * An implementation of the StructuredPushConsumer interface.
   */
  class CosNotifyServiceConsumer_impl :
    public virtual POA_CosNotifyComm::StructuredPushConsumer,
    public virtual PortableServer::RefCountServantBase
  {

  public:

    CosNotifyServiceConsumer_impl (void);

    CosNotifyServiceConsumer_impl (
      CORBA::ORB_ptr orb,
      Components::EventConsumerBase_ptr consumer);

    virtual void push_structured_event (
      const CosNotification::StructuredEvent& event);

    virtual void disconnect_structured_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void offer_change (
        const CosNotification::EventTypeSeq& events_added,
        const CosNotification::EventTypeSeq& events_removed
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType));

  private:

    CORBA::ORB_var orb_;

    Components::EventConsumerBase_var event_consumer_;

  };

  /**
   * @class CosNotify_Consumer_Config_impl
   *
   * Implementation of the CosNotify_Consumer_Config IDL interface that
   * configures the CosNotification service. An object of this type will be
   * returned from @c CIAO::Container::create_consumer_config () when @c NOTIFY
   * is specified as the event service type.
   */
  class CosNotify_Consumer_Config_impl :
    public virtual POA_CIAO::CosNotify_Consumer_Config
  {

  public:

    CosNotify_Consumer_Config_impl ();

    virtual void consumer_id (const char * consumer_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CONNECTION_ID consumer_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void supplier_id (const char * supplier_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CONNECTION_ID supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void consumer (Components::EventConsumerBase_ptr consumer ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual Components::EventConsumerBase_ptr consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual EventServiceType service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotifyFilter::Filter_ptr notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    ACE_CString consumer_id_;

    ACE_CString supplier_id_;

    Components::EventConsumerBase_var consumer_;

    EventServiceType service_type_;

    CosNotifyFilter::Filter_var filter_;

    CosNotification::QoSProperties_var qos_;

  };

  /**
   * @class CosNotify_Supplier_Config_impl
   *
   * Implementation of the CosNotify_Supplier_Config IDL interface that
   * configures the CosNotification service. An object of this type will be
   * returned from @c CIAO::Container::create_supplier_config () when @c NOTIFY
   * is specified as the event service type.
   */
  class CosNotify_Supplier_Config_impl :
    public virtual POA_CIAO::CosNotify_Supplier_Config
  {

  public:

    CosNotify_Supplier_Config_impl ();

    void supplier_id (const char * supplier_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    char * supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotifyFilter::Filter_ptr notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    ACE_CString supplier_id_;

    EventServiceType service_type_;

    CosNotifyFilter::Filter_var filter_;

    CosNotification::QoSProperties_var qos_;

  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_COSNOTIFY_H */
