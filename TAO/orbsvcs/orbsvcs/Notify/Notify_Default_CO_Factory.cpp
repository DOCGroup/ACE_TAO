// $Id$

#include "Notify_Default_CO_Factory.h"
#include "Notify_Factory.h"
#include "Notify_EventChannelFactory_i.h"
#include "Notify_EventChannel_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_FilterFactory_i.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_StructuredProxyPushSupplier_i.h"
#include "Notify_SequenceProxyPushSupplier_i.h"
#include "Notify_ProxyPushSupplier_i.h"
#include "Notify_StructuredProxyPushConsumer_i.h"
#include "Notify_SequenceProxyPushConsumer_i.h"
#include "Notify_ProxyPushConsumer_i.h"

ACE_RCSID(Notify, Notify_Default_CO_Factory, "$Id$")

int
TAO_Notify_Default_CO_Factory::init_svc (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_Notify_Default_CO_Factory);
}

TAO_Notify_Default_CO_Factory::TAO_Notify_Default_CO_Factory (void)
{
}

TAO_Notify_Default_CO_Factory::~TAO_Notify_Default_CO_Factory ()
{
}

int
TAO_Notify_Default_CO_Factory::init (int /*argc*/, char* /*argv*/[])
{
  return 0;
}

int
TAO_Notify_Default_CO_Factory::fini (void)
{
  return 0;
}


TAO_Notify_EventChannel_i*
TAO_Notify_Default_CO_Factory::create_event_channel (TAO_Notify_EventChannelFactory_i* parent TAO_ENV_ARG_DECL)
{
  TAO_Notify_EventChannel_i* channel;
  ACE_NEW_THROW_EX (channel,
                    TAO_Notify_EventChannel_i (parent),
                    CORBA::NO_MEMORY ());
  return channel;
}

TAO_Notify_ConsumerAdmin_i*
TAO_Notify_Default_CO_Factory::create_consumer_admin (TAO_Notify_EventChannel_i* event_channel
                                               TAO_ENV_ARG_DECL)
{
  TAO_Notify_ConsumerAdmin_i* consumer_admin_i;
  ACE_NEW_THROW_EX (consumer_admin_i,
                    TAO_Notify_ConsumerAdmin_i (event_channel),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the event channels qos to the consumer
      CosNotification::QoSProperties_var qos =
                           event_channel->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      consumer_admin_i->set_qos (qos.in ()
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate event channel qos\n");
    }
  ACE_ENDTRY;

  return consumer_admin_i;
}

TAO_Notify_SupplierAdmin_i*
TAO_Notify_Default_CO_Factory::create_supplier_admin (TAO_Notify_EventChannel_i* event_channel
                                                   TAO_ENV_ARG_DECL)
{
  TAO_Notify_SupplierAdmin_i* supplier_admin_i;

  ACE_NEW_THROW_EX (supplier_admin_i,
                    TAO_Notify_SupplierAdmin_i (event_channel),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the event channels qos to the supplier
      CosNotification::QoSProperties_var qos =
                           event_channel->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier_admin_i->set_qos (qos.in ()
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate event channel qos\n");
    }
  ACE_ENDTRY;

  return supplier_admin_i;
}


TAO_Notify_FilterFactory_i*
TAO_Notify_Default_CO_Factory::create_filter_factory (TAO_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_FilterFactory_i* filterfactory;
  ACE_NEW_THROW_EX (filterfactory,
                    TAO_Notify_FilterFactory_i (),
                    CORBA::NO_MEMORY ());
  return filterfactory;
}


TAO_Notify_StructuredProxyPushSupplier_i*
TAO_Notify_Default_CO_Factory::create_struct_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent TAO_ENV_ARG_DECL)
{
  TAO_Notify_StructuredProxyPushSupplier_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_StructuredProxyPushSupplier_i (parent),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the parent qos to the supplier
      CosNotification::QoSProperties_var qos =
                           parent->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy->set_qos (qos.in ()
                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate parent qos\n");
    }
  ACE_ENDTRY;

  return proxy;
}

TAO_Notify_SequenceProxyPushSupplier_i*
TAO_Notify_Default_CO_Factory::create_seq_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent TAO_ENV_ARG_DECL)
{
  TAO_Notify_SequenceProxyPushSupplier_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_SequenceProxyPushSupplier_i (parent),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the parent qos to the supplier
      CosNotification::QoSProperties_var qos =
                           parent->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy->set_qos (qos.in ()
                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate parent qos\n");
    }
  ACE_ENDTRY;

  return proxy;
}

TAO_Notify_ProxyPushSupplier_i*
TAO_Notify_Default_CO_Factory::create_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent TAO_ENV_ARG_DECL)
{
  TAO_Notify_ProxyPushSupplier_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_ProxyPushSupplier_i (parent),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the parent qos to the supplier
      CosNotification::QoSProperties_var qos =
                           parent->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy->set_qos (qos.in ()
                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate parent qos\n");
    }
  ACE_ENDTRY;

  return proxy;
}


TAO_Notify_StructuredProxyPushConsumer_i*
TAO_Notify_Default_CO_Factory::create_struct_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent TAO_ENV_ARG_DECL)
{
  TAO_Notify_StructuredProxyPushConsumer_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_StructuredProxyPushConsumer_i (parent),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the parent qos to the consumer
      CosNotification::QoSProperties_var qos =
                           parent->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy->set_qos (qos.in ()
                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate parent qos\n");
    }
  ACE_ENDTRY;

  return proxy;
}

TAO_Notify_ProxyPushConsumer_i*
TAO_Notify_Default_CO_Factory::create_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent TAO_ENV_ARG_DECL)
{
  TAO_Notify_ProxyPushConsumer_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_ProxyPushConsumer_i (parent),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the parent qos to the consumer
      CosNotification::QoSProperties_var qos =
                           parent->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy->set_qos (qos.in ()
                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate parent qos\n");
    }
  ACE_ENDTRY;

  return proxy;
}

TAO_Notify_SequenceProxyPushConsumer_i*
TAO_Notify_Default_CO_Factory::create_seq_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent TAO_ENV_ARG_DECL)
{
  TAO_Notify_SequenceProxyPushConsumer_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_SequenceProxyPushConsumer_i (parent),
                    CORBA::NO_MEMORY ());
  ACE_TRY
    {
      // Propagate the parent qos to the consumer
      CosNotification::QoSProperties_var qos =
                           parent->get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy->set_qos (qos.in ()
                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to propagate parent qos\n");
    }
  ACE_ENDTRY;

  return proxy;
}


ACE_Lock*
TAO_Notify_Default_CO_Factory::create_channel_factory_lock (TAO_ENV_SINGLE_ARG_DECL)
{
  return create_event_channel_lock (TAO_ENV_SINGLE_ARG_PARAMETER);
}

ACE_Lock*
TAO_Notify_Default_CO_Factory::create_event_channel_lock (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_Lock* lock;
  ACE_NEW_THROW_EX (lock,
                    ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (),
                    CORBA::NO_MEMORY ());
  return lock;
}

ACE_Lock*
TAO_Notify_Default_CO_Factory::create_consumer_admin_lock (TAO_ENV_SINGLE_ARG_DECL)
{
  // Later:
  return create_event_channel_lock (TAO_ENV_SINGLE_ARG_PARAMETER);
}

ACE_Lock*
TAO_Notify_Default_CO_Factory::create_supplier_admin_lock (TAO_ENV_SINGLE_ARG_DECL)
{
  // Later:
  return create_event_channel_lock (TAO_ENV_SINGLE_ARG_PARAMETER);
}

ACE_Lock*
TAO_Notify_Default_CO_Factory::create_proxy_supplier_lock (TAO_ENV_SINGLE_ARG_DECL)
{
  // Later:
  return create_event_channel_lock (TAO_ENV_SINGLE_ARG_PARAMETER);
}

ACE_Lock*
TAO_Notify_Default_CO_Factory::create_proxy_consumer_lock (TAO_ENV_SINGLE_ARG_DECL)
{
  // Later:
  return create_event_channel_lock (TAO_ENV_SINGLE_ARG_PARAMETER);
}


// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Notify_Default_CO_Factory,
                       ACE_TEXT (TAO_NOTIFY_DEF_CO_FACTORY_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Notify_Default_CO_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Notify, TAO_Notify_Default_CO_Factory)

// ****************************************************************
