// $Id$
#include "Builder.h"

#if ! defined (__ACE_INLINE__)
#include "Builder.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Builder, "$Id$")

#include "ace/Auto_Ptr.h"
#include "tao/PortableServer/PortableServerC.h"
#include "Factory.h"
#include "Properties.h"
#include "POA_Helper.h"
#include "ID_Factory.h"
#include "EventChannelFactory.h"
#include "EventChannel.h"
#include "SupplierAdmin.h"
#include "ConsumerAdmin.h"
#include "Structured/StructuredProxyPushConsumer.h"
#include "Structured/StructuredProxyPushSupplier.h"
#include "Event_Manager.h"
#include "Worker_Task.h"
#include "Reactive_Task.h"
#include "ThreadPool_Task.h"

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_NS_Builder::build_event_channel_factory (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_ret;

  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  TAO_NS_POA_Helper* object_poa = 0;
  // Bootstrap initial Object POA
  ACE_NEW_THROW_EX (object_poa,
		    TAO_NS_POA_Helper (),
		    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (ecf_ret._retn ());
 
  auto_ptr<TAO_NS_POA_Helper> auto_object_poa (object_poa);

  object_poa->init (poa ACE_ENV_ARG_PARAMETER);   
  ACE_CHECK_RETURN (ecf_ret._retn ());
 
  // Create ECF
  TAO_NS_EventChannelFactory* ecf = 0;
  factory->create (ecf ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ecf_ret._retn ());
  
  ecf->destroy_callback (ecf);

  PortableServer::ServantBase_var servant_var (ecf);

  ecf->init (object_poa, 0, object_poa, object_poa ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ecf_ret._retn ());

  ecf->init_collection (ACE_ENV_SINGLE_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ecf_ret._retn ());

  // Give ownership of object_poa 
  ecf->object_poa (object_poa);
  
  CORBA::Object_var obj = ecf->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ecf_ret._retn ());

  // release auto_ref.
  auto_object_poa.release ();

  ecf_ret = CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ecf_ret._retn ());
  
  return (ecf_ret._retn ());
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_NS_Builder::build_event_channel (TAO_NS_EventChannelFactory* ecf, const CosNotification::QoSProperties & initial_qos, const CosNotification::AdminProperties & initial_admin, CosNotifyChannelAdmin::ChannelID_out id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     , CosNotification::UnsupportedAdmin
                     ))
{
  CosNotifyChannelAdmin::EventChannel_var ec_ret;

  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  TAO_NS_EventChannel* ec = 0;
  factory->create (ec ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ec_ret._retn ());
  
  ec->destroy_callback (ec);

  PortableServer::ServantBase_var servant_var (ec);

  //  set the parent -
  ec->parent_ = ecf;

  ec->init (ecf->object_poa(), 0, ecf->object_poa(), ecf->object_poa() ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ec_ret._retn ());

  // Create the default worker task.
  TAO_NS_Reactive_Task* worker_task = new TAO_NS_Reactive_Task ();
  ec->worker_task_own (worker_task);

  // proxy poa is set next.
  ec->init_collection (ACE_ENV_SINGLE_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ec_ret._retn ());

  // insert ec in ecf container.
  ecf->insert (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());
  
  // create the event manager. @@ use factory
  ACE_NEW_THROW_EX (ec->event_manager_,
		    TAO_NS_Event_Manager (),
		    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (ec_ret._retn ());

  ec->event_manager_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());
  
  ec->set_qos (initial_qos ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ec_ret._retn ());
  
  ec->set_admin (initial_admin ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ec_ret._retn ());

  CORBA::Object_var obj = ec->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());

  id = ec->id ();

  ec_ret = CosNotifyChannelAdmin::EventChannel::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec_ret._retn ());

  return ec_ret._retn ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_Builder::build_consumer_admin (TAO_NS_EventChannel* ec, CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ))
{
  CosNotifyChannelAdmin::ConsumerAdmin_var ca_ret;

  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  TAO_NS_ConsumerAdmin* ca = 0;
  factory->create (ca ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ca_ret._retn ()); 

  ca->destroy_callback (ca);

  ca->parent_ = ec;
  ca->event_manager_ = ec->event_manager_;

  PortableServer::ServantBase_var servant_var (ca);

  ca->init (ec->object_poa (), ec->worker_task (), ec->object_poa (), ec->proxy_poa () ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ca_ret._retn ());
  
  ca->init_collection (ACE_ENV_SINGLE_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ca_ret._retn ());

  // insert admin in ec container.
  ec->insert (ca ACE_ENV_ARG_PARAMETER);

  CORBA::Object_var obj = ca->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca_ret._retn ());

  id = ca->id ();

  ca_ret = CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in() ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (ca_ret._retn ());
  
  return ca_ret._retn ();
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NS_Builder::build_supplier_admin (TAO_NS_EventChannel* ec, CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ))
{
  CosNotifyChannelAdmin::SupplierAdmin_var sa_ret;

  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  TAO_NS_SupplierAdmin* sa = 0;
  factory->create (sa ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (sa_ret._retn ()); 

  sa->destroy_callback (sa);

  sa->parent_ = ec;
  sa->event_manager_ = ec->event_manager_;
  
  PortableServer::ServantBase_var servant_var (sa);

  sa->init (ec->object_poa (), ec->worker_task (), ec->object_poa (), ec->proxy_poa () ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (sa_ret._retn ());
  
  sa->init_collection (ACE_ENV_SINGLE_ARG_PARAMETER); 
  ACE_CHECK_RETURN (sa_ret._retn ());

  // insert admin in ec container.
  ec->insert (sa ACE_ENV_ARG_PARAMETER);

  CORBA::Object_var obj = sa->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa_ret._retn ());

  id = sa->id ();

  sa_ret = CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in() ACE_ENV_ARG_PARAMETER); 
  ACE_CHECK_RETURN (sa_ret._retn ());
  
  return sa_ret._retn ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_NS_Builder::build_notification_push_consumer (TAO_NS_SupplierAdmin* sa, CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
  
{
  CosNotifyChannelAdmin::ProxyConsumer_var proxy_ret;

  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {

      }
      break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_NS_StructuredProxyPushConsumer* pc = 0;
        factory->create (pc ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (proxy_ret._retn ());

	pc->destroy_callback (pc);

        PortableServer::ServantBase_var servantbase_var (pc);

	pc->event_manager_ = sa->event_manager_;
	pc->parent_ = sa;
	
        pc->init (sa->proxy_poa (), sa->worker_task () ACE_ENV_ARG_PARAMETER); 
        ACE_CHECK_RETURN (proxy_ret._retn ());
		
	// insert proxy in admin container.
	sa->insert (pc ACE_ENV_ARG_PARAMETER);
  
        CORBA::Object_var obj = pc->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (proxy_ret._retn ());

        proxy_id = pc->id ();

        proxy_ret = CosNotifyChannelAdmin::ProxyConsumer::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
	ACE_CHECK_RETURN (proxy_ret._retn ());
      }

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
      }
      break;

    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxyConsumer::_nil ());
    }
  
  return proxy_ret._retn ();
}


CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NS_Builder::build_notification_push_supplier (TAO_NS_ConsumerAdmin* ca, CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminLimitExceeded
                     ))
{
  CosNotifyChannelAdmin::ProxySupplier_var proxy_ret;

  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {

      }
      break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        TAO_NS_StructuredProxyPushSupplier* ps = 0;
        factory->create (ps ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (proxy_ret._retn ());

        PortableServer::ServantBase_var servant (ps->servant ());

	ps->event_manager_ = ca->event_manager_;
	ps->parent_ = ca;
		
        ps->init (ca->proxy_poa (), ca->worker_task () ACE_ENV_ARG_PARAMETER); 
        ACE_CHECK_RETURN (proxy_ret._retn ());
  	
	// insert proxy in admin container.
	ca->insert (ps ACE_ENV_ARG_PARAMETER);

        CORBA::Object_var obj = ps->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (proxy_ret._retn ());

        proxy_id = ps->id ();

	ps->init_ps (ACE_ENV_SINGLE_ARG_PARAMETER);

        proxy_ret = CosNotifyChannelAdmin::ProxySupplier::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
	ACE_CHECK_RETURN (proxy_ret._retn ());
      }

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
      }
      break;

    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxySupplier::_nil ());
    }
  
  return proxy_ret._retn ();
}
