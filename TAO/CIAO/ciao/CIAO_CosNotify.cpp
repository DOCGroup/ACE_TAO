// -*- C++ -*-
//$Id$
#include "CIAO_CosNotify.h"

// @@ George, ACE_RCSID please..

namespace CIAO
{

  CosNotifyService::CosNotifyService (
      CORBA::ORB_ptr orb,
      PortableServer::POA_ptr poa,
      CosNotifyChannelAdmin::EventChannel_ptr ec)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , root_poa_ (PortableServer::POA::_duplicate (poa))
    , notify_channel_ (CosNotifyChannelAdmin::EventChannel::_duplicate (ec))
    , type_id_ ("Any")
    , source_id_ ("Any")
  {
  }

  void
  CosNotifyService::connect_event_supplier (
      Supplier_Config_ptr supplier_config
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {

    ACE_DEBUG ((LM_DEBUG,
                "CIAO::CosNotifyService::connect_event_supplier\n"));

    CosNotifyChannelAdmin::AdminID admin_id;

    CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin =
      this->notify_channel_->new_for_suppliers (
        CosNotifyChannelAdmin::OR_OP,
        admin_id
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    CosNotifyChannelAdmin::ProxyID proxy_id;

    // @@ George, what happens if my_proxy is NULL?
    CosNotifyChannelAdmin::ProxyConsumer_var my_proxy =
      supplier_admin->obtain_notification_push_consumer (
        CosNotifyChannelAdmin::STRUCTURED_EVENT,
        proxy_id
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->proxy_consumer_ =
      CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (my_proxy.in ());

    // @@ George, This check is good, but you should probably return.
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      {
        ACE_ERROR ((LM_ERROR,
                  " (%P|%t) CosNotifyService::connect_event_supplier\n"));
      }

    // Create and register supplier servant
    CosNotifyServiceSupplier_impl * supplier_servant = 0;

    // @@ George, THROW_EX please..
    ACE_NEW (supplier_servant,
             CosNotifyServiceSupplier_impl (orb_.in ()));

    CosNotifyComm::StructuredPushSupplier_var push_supplier =
      supplier_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    CosNotify_Supplier_Config_ptr notify_config =
      CosNotify_Supplier_Config::_narrow (supplier_config
                                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (notify_config))
      {
        ACE_THROW ((CORBA::BAD_PARAM ()));
      }

    CosNotifyFilter::Filter_var filter =
      notify_config->notify_filter (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    CosNotification::QoSProperties_var qos =
      notify_config->notify_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    // @@ George, isin't this a good place to get the QoS values from
    // the meta level for the conenction?? Do  we have any hooks that
    // can achieve this?

    // @@ George, what happens if these calls fail? Yes, the exception
    // would be raised. Do we want to propogate the exception,
    // probably to the deployer? If so, the exception specification
    // for this method wouldn't help you at all. You have declared
    // SystemException. This would mean none of the user exceptions
    // would get propogated. The deployer would get a UNKNOWN
    // exception. That isn't very useful, isn't it?
    this->proxy_consumer_->add_filter (filter.in ()
                                       ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->proxy_consumer_->set_qos (qos.in ()
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->proxy_consumer_->connect_structured_push_supplier (
      push_supplier.in ()
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  CosNotifyService::connect_event_consumer (
          Consumer_Config_ptr consumer_config
          ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException))
  {
    Components::EventConsumerBase_var consumer =
      consumer_config->consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    // @@ George, the same comments about exceptions, return values
    // etc etc. apply.
    CosNotifyChannelAdmin::AdminID admin_id;
    CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
      this->notify_channel_->new_for_consumers (CosNotifyChannelAdmin::OR_OP,
                                                admin_id
                                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    CosNotifyChannelAdmin::ProxyID proxy_id;
    CosNotifyChannelAdmin::ProxySupplier_var my_proxy =
      consumer_admin->obtain_notification_push_supplier (
        CosNotifyChannelAdmin::STRUCTURED_EVENT,
        proxy_id
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier =
      CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (my_proxy.in ());
    if (CORBA::is_nil (proxy_supplier.in ()))
      {
        ACE_ERROR ((LM_ERROR,
                  " (%P|%t) CosNotifyService::connect_event_consumer\n"));
      }

    // Create and register consumer servant
    CosNotifyServiceConsumer_impl * consumer_servant = 0;

    ACE_NEW (consumer_servant,
             CosNotifyServiceConsumer_impl (orb_.in (),
                                            consumer.in ()));

    CosNotifyComm::StructuredPushConsumer_var push_consumer =
      consumer_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    CosNotify_Consumer_Config_ptr notify_config =
      CosNotify_Consumer_Config::_narrow (consumer_config
                                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (notify_config))
      {
        ACE_THROW ((CORBA::BAD_PARAM ()));
      }

    CosNotifyFilter::Filter_var filter =
      notify_config->notify_filter (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    CosNotification::QoSProperties_var qos =
      notify_config->notify_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    proxy_supplier->add_filter (filter.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    proxy_supplier->set_qos (qos.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    proxy_supplier->connect_structured_push_consumer (push_consumer.in ()
                                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    ACE_CString consumer_id =
      consumer_config->consumer_id (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    // @@ George, What happens if the bind fails? Don't we want to
    // roll all this back ie. close the connection etc. ?
    this->proxy_supplier_map_.bind (consumer_id.c_str (),
                                    proxy_supplier._retn ());
  }

  void
  CosNotifyService::disconnect_event_supplier (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException,
          Components::InvalidName,
          Components::InvalidConnection))
  {
    // @@ Comments about exception applies...
    this->proxy_consumer_->disconnect_structured_push_consumer (
      ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

  }

  void
  CosNotifyService::disconnect_event_consumer (
      const char * connection_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
          CORBA::SystemException,
          Components::InvalidName,
          Components::InvalidConnection))
  {

    CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier;

    this->proxy_supplier_map_.unbind (connection_id, proxy_supplier);

    proxy_supplier->disconnect_structured_push_supplier (
      ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

  }

  void
  CosNotifyService::push_event (
      Components::EventBase * ev
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {

    ACE_DEBUG ((LM_DEBUG, "CIAO::CosNotifyService::push_event\n"));

    CosNotification::StructuredEvent event;

    // @@ George, not now, but later we may want to cache these
    // details that are in the critical path that way we don't need to
    // pay for this every call. Just leave the comment around so that
    // we can get to it once we are done with other things.
    event.header.fixed_header.event_type.domain_name =
      CORBA::string_dup ("CIAO_Events");
    event.header.fixed_header.event_type.type_name =
      this->type_id_.c_str ();
    event.header.fixed_header.event_name =
      this->source_id_.c_str ();

    event.remainder_of_body <<= ev;

    this->proxy_consumer_->push_structured_event (
      event
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  CosNotifyServiceSupplier_impl::CosNotifyServiceSupplier_impl (void)
  {
  }

  CosNotifyServiceSupplier_impl::CosNotifyServiceSupplier_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  void
  CosNotifyServiceSupplier_impl::disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void
  CosNotifyServiceSupplier_impl::subscription_change (
          const CosNotification::EventTypeSeq& events_added,
          const CosNotification::EventTypeSeq& events_removed
          ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException,
          CosNotifyComm::InvalidEventType))
  {
  }

  CosNotifyServiceConsumer_impl::CosNotifyServiceConsumer_impl (void)
  {
  }

  CosNotifyServiceConsumer_impl::CosNotifyServiceConsumer_impl (
      CORBA::ORB_ptr orb,
      Components::EventConsumerBase_ptr consumer)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      event_consumer_ (Components::EventConsumerBase::_duplicate (consumer))
  {
  }

  void
  CosNotifyServiceConsumer_impl::push_structured_event (
      const CosNotification::StructuredEvent& event)
  {
    ACE_DEBUG ((LM_DEBUG,
                "CosNotifyServiceConsumer_impl::push_structured_event\n"));

    Components::EventBase *ev = 0;

    if (event.remainder_of_body >>= ev)
      {
        // @@ George, is this add_ref () required?
        ev->_add_ref ();

        this->event_consumer_->push_event (ev
                                           ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
  }

  void
  CosNotifyServiceConsumer_impl::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void
  CosNotifyServiceConsumer_impl::offer_change (
          const CosNotification::EventTypeSeq& events_added,
          const CosNotification::EventTypeSeq& events_removed
          ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((
          CORBA::SystemException,
          CosNotifyComm::InvalidEventType))
  {
  }

  CosNotify_Consumer_Config_impl::CosNotify_Consumer_Config_impl () :
    service_type_ (NOTIFY)
  {
  }

  void
  CosNotify_Consumer_Config_impl::consumer_id (const char * consumer_id
                                               ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->consumer_id_ = consumer_id;
  }

  void
  CosNotify_Consumer_Config_impl::supplier_id (const char * supplier_id
                                               ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->supplier_id_ = supplier_id;
  }

  void
  CosNotify_Consumer_Config_impl::consumer (Components::EventConsumerBase_ptr consumer
                                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->consumer_ = consumer;
  }

  CONNECTION_ID
  CosNotify_Consumer_Config_impl::consumer_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup (this->consumer_id_.c_str ());
  }

  CONNECTION_ID
  CosNotify_Consumer_Config_impl::supplier_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup (this->supplier_id_.c_str ());
  }

  EventServiceType
  CosNotify_Consumer_Config_impl::service_type (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->service_type_;
  }

  Components::EventConsumerBase_ptr
  CosNotify_Consumer_Config_impl::consumer (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return Components::EventConsumerBase::_duplicate (this->consumer_.in ());
  }

  CosNotifyFilter::Filter *
  CosNotify_Consumer_Config_impl::notify_filter (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->filter_._retn ();
  }

  CosNotification::QoSProperties *
  CosNotify_Consumer_Config_impl::notify_qos (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->qos_._retn ();
  }

  CosNotify_Supplier_Config_impl::CosNotify_Supplier_Config_impl () :
    service_type_ (NOTIFY)
  {
  }

  void
  CosNotify_Supplier_Config_impl::supplier_id (const char * supplier_id ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->supplier_id_ = supplier_id;
  }

  CONNECTION_ID
  CosNotify_Supplier_Config_impl::supplier_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup (this->supplier_id_.c_str ());
  }

  EventServiceType
  CosNotify_Supplier_Config_impl::service_type (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->service_type_;
  }

  CosNotifyFilter::Filter_ptr
  CosNotify_Supplier_Config_impl::notify_filter (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->filter_._retn ();
  }

  CosNotification::QoSProperties *
  CosNotify_Supplier_Config_impl::notify_qos (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->qos_._retn ();
  }

}
