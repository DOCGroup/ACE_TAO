// $Id$
#include "Builder.h"

#if ! defined (__ACE_INLINE__)
#include "Builder.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_Builder, "$Id$")

#include "ace/Dynamic_Service.h"
#include "tao/PortableServer/PortableServerC.h"
#include "Factory.h"
#include "Properties.h"
#include "POA_Helper.h"
#include "ID_Factory.h"
#include "EventChannelFactory.h"
#include "EventChannel.h"
#include "SupplierAdmin.h"
#include "ConsumerAdmin.h"
#include "Event_Manager.h"
#include "Worker_Task.h"
#include "Reactive_Task.h"
#include "ThreadPool_Task.h"
#include "FilterFactory.h"
#include "Object.h"
#include "EventType.h"
#include "Event.h"
#include "Any/AnyEvent.h"
#include "Any/ProxyPushConsumer.h"
#include "Any/ProxyPushSupplier.h"
#include "Any/CosEC_ProxyPushConsumer.h"
#include "Any/CosEC_ProxyPushSupplier.h"
#include "Structured/StructuredProxyPushConsumer.h"
#include "Structured/StructuredProxyPushSupplier.h"
#include "Sequence/SequenceProxyPushConsumer.h"
#include "Sequence/SequenceProxyPushSupplier.h"
#include "ETCL_FilterFactory.h"
#include "Container_T.h"

template <class PROXY_IMPL, class PROXY, class PROXY_PTR, class PROXY_VAR, class PARENT>
class TAO_Notify_Proxy_Builder_T
{
public:
  PROXY_PTR
  build (PARENT *parent, CosNotifyChannelAdmin::ProxyID_out proxy_id
         , const CosNotification::QoSProperties & initial_qos ACE_ENV_ARG_DECL)
  {
    PROXY_VAR proxy_ret;

    TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

    PROXY_IMPL* proxy = 0;
    factory->create (proxy ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (proxy_ret._retn ());

    PortableServer::ServantBase_var servant (proxy);

    proxy->init (parent ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (proxy_ret._retn ());

    proxy->set_qos (initial_qos ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (proxy_ret._retn ());

    CORBA::Object_var obj = proxy->activate (proxy ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (proxy_ret._retn ());

    proxy_id = proxy->id ();

    proxy_ret = PROXY::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (proxy_ret._retn ());

    // insert proxy in admin container.
    parent->insert (proxy ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PROXY::_nil ());

    return proxy_ret._retn ();
  }
};

// define the ProxyConsumer Builders.
typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_ProxyPushConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer_ptr
                             , CosNotifyChannelAdmin::ProxyConsumer_var
                             , TAO_Notify_SupplierAdmin>
TAO_Notify_ProxyPushConsumer_Builder;

typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_StructuredProxyPushConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer_ptr
                             , CosNotifyChannelAdmin::ProxyConsumer_var
                             , TAO_Notify_SupplierAdmin>
TAO_Notify_StructuredProxyPushConsumer_Builder;

typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_SequenceProxyPushConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer_ptr
                             , CosNotifyChannelAdmin::ProxyConsumer_var
                             , TAO_Notify_SupplierAdmin>
TAO_Notify_SequenceProxyPushConsumer_Builder;

typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_CosEC_ProxyPushConsumer
                             , CosEventChannelAdmin::ProxyPushConsumer
                             , CosEventChannelAdmin::ProxyPushConsumer_ptr
                             , CosEventChannelAdmin::ProxyPushConsumer_var
                             , TAO_Notify_SupplierAdmin>
TAO_Notify_CosEC_ProxyPushConsumer_Builder;

// define the ProxySupplier Builders.
typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_ProxyPushSupplier
                             , CosNotifyChannelAdmin::ProxySupplier
                             , CosNotifyChannelAdmin::ProxySupplier_ptr
                             , CosNotifyChannelAdmin::ProxySupplier_var
                             , TAO_Notify_ConsumerAdmin>
TAO_Notify_ProxyPushSupplier_Builder;

typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_StructuredProxyPushSupplier
                             , CosNotifyChannelAdmin::ProxySupplier
                             , CosNotifyChannelAdmin::ProxySupplier_ptr
                             , CosNotifyChannelAdmin::ProxySupplier_var
                             , TAO_Notify_ConsumerAdmin>
TAO_Notify_StructuredProxyPushSupplier_Builder;

typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_SequenceProxyPushSupplier
                             , CosNotifyChannelAdmin::ProxySupplier
                             , CosNotifyChannelAdmin::ProxySupplier_ptr
                             , CosNotifyChannelAdmin::ProxySupplier_var
                             , TAO_Notify_ConsumerAdmin>
TAO_Notify_SequenceProxyPushSupplier_Builder;

typedef TAO_Notify_Proxy_Builder_T<TAO_Notify_CosEC_ProxyPushSupplier
                             , CosEventChannelAdmin::ProxyPushSupplier
                             , CosEventChannelAdmin::ProxyPushSupplier_ptr
                             , CosEventChannelAdmin::ProxyPushSupplier_var
                             , TAO_Notify_ConsumerAdmin>
TAO_Notify_CosEC_ProxyPushSupplier_Builder;

TAO_Notify_Builder::TAO_Notify_Builder (void)
{
  // Init the static members.
  TAO_Notify_AnyEvent::event_type_ = TAO_Notify_EventType::special ();
}

TAO_Notify_Builder::~TAO_Notify_Builder ()
{
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_Builder::build_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_FilterFactory* ff = ACE_Dynamic_Service<TAO_Notify_FilterFactory>::instance ("TAO_Notify_FilterFactory");

  if (ff == 0)
    {
      ACE_NEW_THROW_EX (ff,
                        TAO_Notify_ETCL_FilterFactory (),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());
    }

  PortableServer::POA_var default_poa = TAO_Notify_PROPERTIES::instance ()->default_poa ();

  return ff->create (default_poa ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_Builder::build_event_channel_factory (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_ret;

  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  // Create ECF
  TAO_Notify_EventChannelFactory* ecf = 0;
  factory->create (ecf ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ecf_ret._retn ());

  PortableServer::ServantBase_var servant_var (ecf);

  ecf->TAO_Notify_EventChannelFactory::init (poa ACE_ENV_ARG_PARAMETER);

  CORBA::Object_var obj = ecf->activate (ecf ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ecf_ret._retn ());

  ecf_ret = CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ecf_ret._retn ());

  return (ecf_ret._retn ());
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_Builder::build_event_channel (TAO_Notify_EventChannelFactory* ecf, const CosNotification::QoSProperties & initial_qos, const CosNotification::AdminProperties & initial_admin, CosNotifyChannelAdmin::ChannelID_out id ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannel_var ec_ret;

  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  TAO_Notify_EventChannel* ec = 0;
  factory->create (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());

  PortableServer::ServantBase_var servant_var (ec);

  ec->init (ecf, initial_qos, initial_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());

  // insert ec in ec container.
  ecf->ec_container_->insert (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());

  CORBA::Object_var obj = ec->activate (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());

  // Populate the ID to return.
  id = ec->id ();

  ec_ret = CosNotifyChannelAdmin::EventChannel::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());

  return ec_ret._retn ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_Builder::build_consumer_admin (TAO_Notify_EventChannel* ec, CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::ConsumerAdmin_var ca_ret;

  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  TAO_Notify_ConsumerAdmin* ca = 0;
  factory->create (ca ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca_ret._retn ());

  PortableServer::ServantBase_var servant_var (ca);

  ca->init (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca_ret._retn ());

  ca->filter_operator (op);

  CORBA::Object_var obj = ca->activate (ca ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca_ret._retn ());

  id = ca->id ();

  ca_ret = CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca_ret._retn ());

  // insert admin in CA container.
  ec->ca_container_->insert (ca ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca_ret._retn ());

  return ca_ret._retn ();
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_Builder::build_supplier_admin (TAO_Notify_EventChannel* ec, CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::SupplierAdmin_var sa_ret;

  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  TAO_Notify_SupplierAdmin* sa = 0;
  factory->create (sa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa_ret._retn ());

  PortableServer::ServantBase_var servant_var (sa);

  sa->init (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa_ret._retn ());

  sa->filter_operator (op);

  CORBA::Object_var obj = sa->activate (sa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa_ret._retn ());

  id = sa->id ();

  sa_ret = CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa_ret._retn ());

  // insert admin in SA container.
  ec->sa_container_->insert (sa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa_ret._retn ());

  return sa_ret._retn ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_Builder::build_proxy(TAO_Notify_SupplierAdmin* sa
                            , CosNotifyChannelAdmin::ClientType ctype
                            , CosNotifyChannelAdmin::ProxyID_out proxy_id
                            , const CosNotification::QoSProperties & initial_qos
                            ACE_ENV_ARG_DECL)
{
  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        TAO_Notify_ProxyPushConsumer_Builder pb;
        return pb.build (sa, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
      }
    break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_Notify_StructuredProxyPushConsumer_Builder pb;
        return pb.build (sa, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
      }
    break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
     {
       TAO_Notify_SequenceProxyPushConsumer_Builder pb;
       return pb.build (sa, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
     }
      break;

    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxyConsumer::_nil ());
                        }
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_Builder::build_proxy(TAO_Notify_ConsumerAdmin* ca
                            , CosNotifyChannelAdmin::ClientType ctype
                            , CosNotifyChannelAdmin::ProxyID_out proxy_id
                            , const CosNotification::QoSProperties & initial_qos
                            ACE_ENV_ARG_DECL)
{
  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        TAO_Notify_ProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
      }
      break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_Notify_StructuredProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
      }
    break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
        TAO_Notify_SequenceProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
      }
      break;

    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxySupplier::_nil ());
    }
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_Notify_Builder::build_proxy (TAO_Notify_ConsumerAdmin* ca ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::ProxyID proxy_id;

  TAO_Notify_CosEC_ProxyPushSupplier_Builder pb;

  CosNotification::QoSProperties initial_qos;

  return pb.build (ca, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_Notify_Builder::build_proxy (TAO_Notify_SupplierAdmin* sa ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::ProxyID proxy_id;

  TAO_Notify_CosEC_ProxyPushConsumer_Builder pb;

  CosNotification::QoSProperties initial_qos;

  return pb.build (sa, proxy_id, initial_qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Builder::apply_reactive_concurrency (TAO_Notify_Object& object ACE_ENV_ARG_DECL)
{
  TAO_Notify_Reactive_Task* worker_task;

  ACE_NEW_THROW_EX (worker_task,
                    TAO_Notify_Reactive_Task (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  worker_task->init (object.admin_properties_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  object.worker_task_own (worker_task);

  object.worker_task_->_decr_refcnt ();
}

void
TAO_Notify_Builder::apply_thread_pool_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolParams& tp_params ACE_ENV_ARG_DECL)
{
  TAO_Notify_ThreadPool_Task* worker_task;

  ACE_NEW_THROW_EX (worker_task,
                    TAO_Notify_ThreadPool_Task (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  worker_task->init (tp_params, object.admin_properties_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  object.worker_task_own (worker_task);

  object.worker_task_->_decr_refcnt ();
}

void
TAO_Notify_Builder::apply_lane_concurrency (TAO_Notify_Object& /*object*/, const NotifyExt::ThreadPoolLanesParams& /*tpl_params*/ ACE_ENV_ARG_DECL)
{
  // No lane support
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Notify_FilterFactory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Notify_FilterFactory>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
