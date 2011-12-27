// $Id$
#include "orbsvcs/Notify/Builder.h"

#include "ace/Dynamic_Service.h"

#include "tao/PortableServer/PortableServerC.h"

#include "orbsvcs/Notify/Factory.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/POA_Helper.h"
#include "orbsvcs/Notify/ID_Factory.h"
#include "orbsvcs/Notify/EventChannelFactory.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "orbsvcs/Notify/SupplierAdmin.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Reactive_Task.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"
#include "orbsvcs/Notify/FilterFactory.h"
#include "orbsvcs/Notify/Object.h"
#include "orbsvcs/Notify/EventType.h"
#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/Any/AnyEvent.h"
#include "orbsvcs/Notify/Any/ProxyPushConsumer.h"
#include "orbsvcs/Notify/Any/ProxyPushSupplier.h"
#include "orbsvcs/Notify/Any/CosEC_ProxyPushConsumer.h"
#include "orbsvcs/Notify/Any/CosEC_ProxyPushSupplier.h"
#include "orbsvcs/Notify/Structured/StructuredProxyPushConsumer.h"
#include "orbsvcs/Notify/Structured/StructuredProxyPushSupplier.h"
#include "orbsvcs/Notify/Sequence/SequenceProxyPushConsumer.h"
#include "orbsvcs/Notify/Sequence/SequenceProxyPushSupplier.h"
#include "orbsvcs/Notify/ETCL_FilterFactory.h"
#include "orbsvcs/Notify/Container_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class PROXY_IMPL,
          class PROXY,
          class PROXY_PTR,
          class PROXY_VAR,
          class PARENT>
class TAO_Notify_Proxy_Builder_T
{
public:
  PROXY_PTR
  build (PARENT *parent, CosNotifyChannelAdmin::ProxyID_out proxy_id
         , const CosNotification::QoSProperties & initial_qos)
  {
    PROXY_VAR proxy_ret;

    TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

    PROXY_IMPL* proxy = 0;
    factory->create (proxy);

    PortableServer::ServantBase_var servant (proxy);

    proxy->init (parent);

    proxy->set_qos (initial_qos);

    CORBA::Object_var obj = proxy->activate (proxy);

    proxy_id = proxy->id ();

    // insert proxy in admin container.
    parent->insert (proxy);

    proxy->configure (*parent, proxy_id);

    proxy_ret = PROXY::_narrow (obj.in());
    return proxy_ret._retn ();
  }

  PROXY_IMPL*
  build (PARENT *parent, const CosNotifyChannelAdmin::ProxyID proxy_id)
  {
    TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

    PROXY_IMPL* proxy = 0;
    factory->create (proxy);

    PortableServer::ServantBase_var servant (proxy);

    proxy->init (parent);

    proxy->activate (proxy, proxy_id);

    // insert proxy in admin container.
    parent->insert (proxy);

    return proxy;
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
TAO_Notify_Builder::build_filter_factory (PortableServer::POA_ptr poa, TAO_Notify_FilterFactory*& ff)
{
  ff = ACE_Dynamic_Service<TAO_Notify_FilterFactory>::instance ("TAO_Notify_FilterFactory");

  if (ff == 0)
    {
      ACE_NEW_THROW_EX (ff,
                        TAO_Notify_ETCL_FilterFactory (),
                        CORBA::NO_MEMORY ());
    }

  return ff->create (poa);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_Builder::build_event_channel_factory (PortableServer::POA_ptr poa,
                                                 const char* name)
{
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  // Create ECF
  TAO_Notify_EventChannelFactory* ecf = 0;
  factory->create (ecf, name);

  ecf->init (poa);
  return ecf->activate_self ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_Builder::build_event_channel (
  TAO_Notify_EventChannelFactory* ecf,
  const CosNotification::QoSProperties & initial_qos,
  const CosNotification::AdminProperties & initial_admin,
  CosNotifyChannelAdmin::ChannelID_out id,
  const char* ec_name)
{
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  TAO_Notify_EventChannel* ec = 0;
  factory->create (ec, ec_name);

  ec->init (ecf, initial_qos, initial_admin);

  // insert ec in ec container.
  ecf->ec_container().insert (ec);

  CORBA::Object_var obj = ec->activate (ec);

  // Populate the ID to return.
  id = ec->id ();

  return CosNotifyChannelAdmin::EventChannel::_narrow (obj.in());
}

TAO_Notify_EventChannel *
TAO_Notify_Builder::build_event_channel (
  TAO_Notify_EventChannelFactory* ecf,
  const CosNotifyChannelAdmin::ChannelID id,
  const char* ec_name)
{
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  TAO_Notify_EventChannel* ec = 0;
  factory->create (ec, ec_name);

  ec->init (ecf); //, initial_qos, initial_admin

  // insert ec in ec container.
  ecf->ec_container().insert (ec);

  ec->activate (ec, id);

  return ec;
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_Builder::build_consumer_admin (
    TAO_Notify_EventChannel* ec,
    CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID_out id)
{
  CosNotifyChannelAdmin::ConsumerAdmin_var ca_ret;

  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  TAO_Notify_ConsumerAdmin* ca = 0;
  factory->create (ca);

  ca->init (ec);

  ca->filter_operator (op);

  CORBA::Object_var obj = ca->activate (ca);

  id = ca->id ();

  ca_ret = CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in());

  // insert admin in CA container.
  ec->ca_container_->insert (ca);

  return ca_ret._retn ();
}

TAO_Notify_ConsumerAdmin *
TAO_Notify_Builder::build_consumer_admin (
  TAO_Notify_EventChannel* ec,
  const CosNotifyChannelAdmin::ChannelID id)
{
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();
  TAO_Notify_ConsumerAdmin * ca = 0;
  factory->create (ca);

  ca->init (ec);

  CORBA::Object_var obj = ca->activate (ca, id);

  // insert admin in CA container.
  ec->ca_container_->insert (ca);
  return ca;
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_Builder::build_supplier_admin (TAO_Notify_EventChannel* ec,
                                          CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                          CosNotifyChannelAdmin::AdminID_out id)
{
  CosNotifyChannelAdmin::SupplierAdmin_var sa_ret;

  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  TAO_Notify_SupplierAdmin* sa = 0;
  factory->create (sa);

  sa->init (ec);

  sa->filter_operator (op);

  CORBA::Object_var obj = sa->activate (sa);

  id = sa->id ();

  sa_ret = CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in());

  // insert admin in SA container.
  ec->sa_container().insert (sa);

  return sa_ret._retn ();
}

TAO_Notify_SupplierAdmin *
TAO_Notify_Builder::build_supplier_admin (
  TAO_Notify_EventChannel* ec,
  const CosNotifyChannelAdmin::ChannelID id)
{
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();
  TAO_Notify_SupplierAdmin * sa = 0;
  factory->create (sa);

  sa->init (ec);

  CORBA::Object_var obj = sa->activate (sa, id);

  // insert admin in CA container.
  ec->sa_container().insert (sa);

  return sa;
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_Builder::build_proxy(TAO_Notify_SupplierAdmin* sa
                            , CosNotifyChannelAdmin::ClientType ctype
                            , CosNotifyChannelAdmin::ProxyID_out proxy_id
                            , const CosNotification::QoSProperties & initial_qos)
{
  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        TAO_Notify_ProxyPushConsumer_Builder pb;
        return pb.build (sa, proxy_id, initial_qos);
      }
    break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_Notify_StructuredProxyPushConsumer_Builder pb;
        return pb.build (sa, proxy_id, initial_qos);
      }
    break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
     {
       TAO_Notify_SequenceProxyPushConsumer_Builder pb;
       return pb.build (sa, proxy_id, initial_qos);
     }
      break;

    default:
      throw CORBA::BAD_PARAM ();
                        }
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_Builder::build_proxy(TAO_Notify_ConsumerAdmin* ca
                            , CosNotifyChannelAdmin::ClientType ctype
                            , CosNotifyChannelAdmin::ProxyID_out proxy_id
                            , const CosNotification::QoSProperties & initial_qos)
{
  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        TAO_Notify_ProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id, initial_qos);
      }
      break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_Notify_StructuredProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id, initial_qos);
      }
    break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
        TAO_Notify_SequenceProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id, initial_qos);
      }
      break;

    default:
      throw CORBA::BAD_PARAM ();
    }
}

TAO_Notify_ProxyConsumer *
TAO_Notify_Builder::build_proxy(TAO_Notify_SupplierAdmin* sa
                            , CosNotifyChannelAdmin::ClientType ctype
                            , const CosNotifyChannelAdmin::ProxyID proxy_id)
{
  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        TAO_Notify_ProxyPushConsumer_Builder pb;
        return pb.build (sa, proxy_id);
      }
    break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_Notify_StructuredProxyPushConsumer_Builder pb;
        return pb.build (sa, proxy_id);
      }
    break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
     {
       TAO_Notify_SequenceProxyPushConsumer_Builder pb;
       return pb.build (sa, proxy_id);
     }
      break;

    default:
      throw CORBA::BAD_PARAM ();
                        }
}

TAO_Notify_ProxySupplier *
TAO_Notify_Builder::build_proxy(TAO_Notify_ConsumerAdmin* ca
                            , CosNotifyChannelAdmin::ClientType ctype
                            , const CosNotifyChannelAdmin::ProxyID proxy_id)
{
  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        TAO_Notify_ProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id);
      }
      break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_Notify_StructuredProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id);
      }
    break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
        TAO_Notify_SequenceProxyPushSupplier_Builder pb;
        return pb.build (ca, proxy_id);
      }
      break;

    default:
      throw CORBA::BAD_PARAM ();
    }
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_Notify_Builder::build_proxy (TAO_Notify_ConsumerAdmin* ca)
{
  CosNotifyChannelAdmin::ProxyID proxy_id;

  TAO_Notify_CosEC_ProxyPushSupplier_Builder pb;

  CosNotification::QoSProperties initial_qos;

  return pb.build (ca, proxy_id, initial_qos);
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_Notify_Builder::build_proxy (TAO_Notify_SupplierAdmin* sa)
{
  CosNotifyChannelAdmin::ProxyID proxy_id;

  TAO_Notify_CosEC_ProxyPushConsumer_Builder pb;

  CosNotification::QoSProperties initial_qos;

  return pb.build (sa, proxy_id, initial_qos);
}

void
TAO_Notify_Builder::apply_reactive_concurrency (TAO_Notify_Object& object)
{
  TAO_Notify_Reactive_Task* worker_task;

  ACE_NEW_THROW_EX (worker_task,
                    TAO_Notify_Reactive_Task (),
                    CORBA::NO_MEMORY ());

  object.set_worker_task (worker_task);

  worker_task->init ();
}

void
TAO_Notify_Builder::apply_thread_pool_concurrency (TAO_Notify_Object& object, const NotifyExt::ThreadPoolParams& tp_params)
{
  TAO_Notify_ThreadPool_Task* worker_task = 0;

  ACE_NEW_THROW_EX (worker_task,
                    TAO_Notify_ThreadPool_Task (),
                    CORBA::NO_MEMORY ());

  object.set_worker_task (worker_task);

  worker_task->init (tp_params, object.admin_properties_);
}

void
TAO_Notify_Builder::apply_lane_concurrency (TAO_Notify_Object& /*object*/, const NotifyExt::ThreadPoolLanesParams& /*tpl_params*/)
{
  // No lane support
  throw CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
