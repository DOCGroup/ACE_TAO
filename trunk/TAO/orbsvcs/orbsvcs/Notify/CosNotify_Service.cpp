// $Id$

#include "orbsvcs/Notify/CosNotify_Service.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Default_Factory.h"
#include "orbsvcs/Notify/Builder.h"
#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"
#include "ace/Dynamic_Service.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/NotifyExtC.h"
#include "tao/debug.h"

ACE_RCSID (Notify,
           TAO_CosNotify_Service,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CosNotify_Service::TAO_CosNotify_Service (void)
{
}

TAO_CosNotify_Service::~TAO_CosNotify_Service ()
{
}

int
TAO_CosNotify_Service::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR *current_arg = 0;

  // Default to an all reactive system.
  int ec_threads = 0;
  int consumer_threads = 0;
  int supplier_threads = 0;

  int task_per_proxy = 0;

  TAO_Notify_Properties *properties = TAO_Notify_PROPERTIES::instance();

  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-MTDispatching")) == 0)
        {
          arg_shifter.consume_arg ();
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) The -MTDispatching option has been deprecated, use -DispatchingThreads \n")));
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-DispatchingThreads"))))
        {
          consumer_threads += ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-MTSourceEval")) == 0)
        {
          arg_shifter.consume_arg ();
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) The -MTSourceEval option has been deprecated, use -SourceThreads \n")));
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-SourceThreads"))))
        {
          supplier_threads += ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-MTLookup")) == 0)
        {
          arg_shifter.consume_arg ();
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) The -MTLookup option has been deprecated, use -SourceThreads \n")));
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-LookupThreads"))))
        {
          supplier_threads += ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) The -LookupThreads option has been deprecated, use -SourceThreads \n")));
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-MTListenerEval")) == 0)
        {
          arg_shifter.consume_arg ();
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) The -MTListenerEval option has been deprecated, use -DispatchingThreads \n")));
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ListenerThreads"))))
        {
          // Since this option is always added to consumer_threads, we'll
          // deprecate it in favor of that option.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) The -ListenerThreads option has been deprecated, use -DispatchingThreads \n")));
          consumer_threads += ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-AsynchUpdates")) == 0)
        {
          arg_shifter.consume_arg ();

          properties->asynch_updates (1);
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-NoUpdates")) == 0)
        {
          arg_shifter.consume_arg ();

          properties->updates (0);
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-AllocateTaskperProxy")) == 0)
        {
          task_per_proxy = 1;
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-AllowReconnect")) == 0)
      {
        arg_shifter.consume_arg ();
        TAO_Notify_PROPERTIES::instance()->allow_reconnect (true);
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Ignoring unknown option for Notify Factory: %s\n"),
                    arg_shifter.get_current()
          ));
        arg_shifter.consume_arg ();
      }
    }

  // Init the EC QoS
  {
    CosNotification::QoSProperties qos;
    this->set_threads (qos, ec_threads);
    properties->default_event_channel_qos_properties (qos);
  }

  if (task_per_proxy == 0)
    {
      // Set the per ConsumerAdmin QoS
      {
        if (consumer_threads > 0)
          ACE_DEBUG((LM_DEBUG, "Using %d threads for each ConsumerAdmin.\n", consumer_threads));
        CosNotification::QoSProperties qos;
        this->set_threads (qos, consumer_threads);
        properties->default_consumer_admin_qos_properties (qos);
      }

      // Set the per SupplierAdmin QoS
      {
        if (supplier_threads > 0)
          ACE_DEBUG((LM_DEBUG, "Using %d threads for each SupplierAdmin.\n", supplier_threads));
        CosNotification::QoSProperties qos;
        this->set_threads (qos, supplier_threads);
        properties->default_supplier_admin_qos_properties (qos);
      }
    }
  else
    {
      // Set the per ProxyConsumer QoS
      {
        if (supplier_threads > 0)
          ACE_DEBUG((LM_DEBUG, "Using %d threads for each Supplier.\n", supplier_threads));
        CosNotification::QoSProperties qos;
        this->set_threads (qos, supplier_threads); // lookup thread per proxy doesn't make sense.
        properties->default_proxy_consumer_qos_properties (qos);
      }

      // Set the per ProxySupplier QoS
      {
        if (consumer_threads > 0)
          ACE_DEBUG((LM_DEBUG, "Using %d threads for each Consumer.\n", consumer_threads));
        CosNotification::QoSProperties qos;
        this->set_threads (qos, consumer_threads);
        properties->default_proxy_supplier_qos_properties (qos);
      }
    }

  return 0;
}

void
TAO_CosNotify_Service::set_threads (CosNotification::QoSProperties &qos, int threads)
{
  NotifyExt::ThreadPoolParams tp_params =
    {NotifyExt::CLIENT_PROPAGATED, 0, 0, (unsigned)threads, 0, 0, 0, 0, 0 };

  qos.length (1);
  qos[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
  qos[0].value <<= tp_params;
}

int
TAO_CosNotify_Service::fini (void)
{
  return 0;
}

void
TAO_CosNotify_Service::init_service (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Loading the Cos Notification Service...\n"));

  this->init_i (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CosNotify_Service::init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  // Obtain the Root POA
  CORBA::Object_var object  =
    orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (object.in ()))
    ACE_ERROR ((LM_ERROR,
                " (%P|%t) Unable to resolve the RootPOA.\n"));

  PortableServer::POA_var default_poa = PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  /// Set the properties
    TAO_Notify_Properties* properties = TAO_Notify_PROPERTIES::instance();

    properties->orb (orb);
    properties->default_poa (default_poa.in ());

    // Init the factory
    this->factory_.reset (this->create_factory (ACE_ENV_SINGLE_ARG_PARAMETER));
    ACE_CHECK;
    ACE_ASSERT( this->factory_.get() != 0 );
    TAO_Notify_PROPERTIES::instance()->factory (this->factory_.get());

    this->builder_.reset (this->create_builder (ACE_ENV_SINGLE_ARG_PARAMETER));
    ACE_CHECK;
    ACE_ASSERT( this->builder_.get() != 0 );
    TAO_Notify_PROPERTIES::instance()->builder (this->builder_.get());
}

TAO_Notify_Factory*
TAO_CosNotify_Service::create_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Factory* factory = ACE_Dynamic_Service<TAO_Notify_Factory>::instance ("TAO_Notify_Factory");
  if (factory == 0)
    {
       ACE_NEW_THROW_EX (factory,
                         TAO_Notify_Default_Factory (),
                         CORBA::NO_MEMORY ());
       ACE_CHECK_RETURN (0);
    }
  return factory;
}

TAO_Notify_Builder*
TAO_CosNotify_Service::create_builder (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Builder* builder = 0;
  ACE_NEW_THROW_EX (builder,
                    TAO_Notify_Builder (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return builder;
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_CosNotify_Service::create (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  return this->builder().build_event_channel_factory (poa ACE_ENV_ARG_PARAMETER);
}

void
TAO_CosNotify_Service::remove (TAO_Notify_EventChannelFactory* /*ecf*/ ACE_ENV_ARG_DECL_NOT_USED)
{
  // NOP.
}

TAO_Notify_Factory&
TAO_CosNotify_Service::factory (void)
{
  ACE_ASSERT( this->factory_.get() != 0 );
  return *this->factory_;
}

TAO_Notify_Builder&
TAO_CosNotify_Service::builder (void)
{
  ACE_ASSERT( this->builder_.get() != 0 );
  return *this->builder_;
}

TAO_END_VERSIONED_NAMESPACE_DECL


/*********************************************************************************************************************/

ACE_STATIC_SVC_DEFINE (TAO_Notify_Default_EMO_Factory_OLD,
                       ACE_TEXT (TAO_NOTIFY_DEF_EMO_FACTORY_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CosNotify_Service),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

/*********************************************************************************************************************/

ACE_STATIC_SVC_DEFINE (TAO_CosNotify_Service,
                       ACE_TEXT (TAO_COS_NOTIFICATION_SERVICE_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CosNotify_Service),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)


ACE_FACTORY_DEFINE (TAO_Notify_Serv, TAO_CosNotify_Service)

/*********************************************************************************************************************/
