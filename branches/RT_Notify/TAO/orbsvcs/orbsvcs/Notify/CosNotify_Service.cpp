// $Id$

#include "CosNotify_Service.h"
#include "Properties.h"
#include "Default_Factory.h"
#include "Builder.h"
#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"
#include "ace/Dynamic_Service.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/NotifyExtC.h"
#include "tao/debug.h"

ACE_RCSID(RT_Notify, TAO_NS_CosNotify_Service, "$Id$")

TAO_NS_CosNotify_Service::TAO_NS_CosNotify_Service (void)
  : factory_ (0)
  , builder_ (0)
{
}

TAO_NS_CosNotify_Service::~TAO_NS_CosNotify_Service ()
{
  delete this->factory_;
  delete this->builder_;
}

int
TAO_NS_CosNotify_Service::init (int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR *current_arg = 0;

  // Default to an all reactive system.
  int ec_threads = 0;
  int dispatching_threads = 0;
  int listener_threads = 0;
  int source_threads = 0;
  int lookup_threads = 0;

  int task_per_proxy = 0;

  TAO_NS_Properties *properties = TAO_NS_PROPERTIES::instance();

  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_LIB_TEXT("-MTDispatching")) == 0)
        {
          // If Dispatching Threads are initalized, the option is implicit.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-DispatchingThreads"))))
        {
          dispatching_threads = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_LIB_TEXT("-MTSourceEval")) == 0)
        {
          // If Source Threads are initalized, the option is implicit.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-SourceThreads"))))
        {
          source_threads = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_LIB_TEXT("-MTLookup")) == 0)
        {
          // If Source Threads are initalized, the option is implicit.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-LookupThreads"))))
        {
          lookup_threads = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_LIB_TEXT("-MTListenerEval")) == 0)
        {
          // If Source Threads are initalized, the option is implicit.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-ListenerThreads"))))
        {
          listener_threads = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_LIB_TEXT("-AsynchUpdates")) == 0)
        {
          arg_shifter.consume_arg ();

          properties->asynch_updates (1);
        }
       else if (arg_shifter.cur_arg_strncasecmp (ACE_LIB_TEXT("-AllocateTaskperProxy")) == 0)
        {
          task_per_proxy = 1;
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
        CosNotification::QoSProperties qos;
        this->set_threads (qos, dispatching_threads + listener_threads);
        properties->default_consumer_admin_qos_properties (qos);
      }

      // Set the per SupplierAdmin QoS
      {
        CosNotification::QoSProperties qos;
        this->set_threads (qos, lookup_threads + source_threads);
        properties->default_supplier_admin_qos_properties (qos);
      }
    }
  else
    {
      // Set the per ProxyConsumer QoS
      {
        CosNotification::QoSProperties qos;
        this->set_threads (qos, dispatching_threads + listener_threads);
        properties->default_proxy_consumer_qos_properties (qos);
      }

      // Set the per ProxySupplier QoS
      {
        CosNotification::QoSProperties qos;
        this->set_threads (qos, source_threads); // lookup thread per proxy doesn't make sense.
        properties->default_proxy_supplier_qos_properties (qos);
      }
    }

  return 0;
}

void
TAO_NS_CosNotify_Service::set_threads (CosNotification::QoSProperties &qos, int threads)
{
  NotifyExt::ThreadPoolParams tp_params =
    {0, (unsigned)threads, 0, 0, 0, 0, 0 };

  qos.length (1);
  qos[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
  qos[0].value <<= tp_params;
}

int
TAO_NS_CosNotify_Service::fini (void)
{
  return 0;
}

void
TAO_NS_CosNotify_Service::init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Loading the Cos Notification Service...\n"));

  this->init_i (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_CosNotify_Service::init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  /// first, init the main thread.
  //this->init_main_thread (orb ACE_ENV_ARG_PARAMETER);
  //ACE_CHECK;

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
  TAO_NS_Properties* properties = TAO_NS_PROPERTIES::instance();

  properties->orb (orb);
  properties->default_poa (default_poa.in ());
  properties->sched_policy (orb->orb_core ()->orb_params ()->sched_policy ());
  properties->scope_policy (orb->orb_core ()->orb_params ()->scope_policy ());

  // Init the factory
  this->init_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->init_builder (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_CosNotify_Service::init_main_thread (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Sched_Params::Policy sched_policy;
  long thr_sched_policy = orb->orb_core ()->orb_params ()->sched_policy ();

  //long thr_scope_policy = orb->orb_core ()->orb_params ()->scope_policy ();

  if (thr_sched_policy == THR_SCHED_FIFO)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_FIFO\n"));

      sched_policy = ACE_SCHED_FIFO;
    }
  else if (thr_sched_policy == THR_SCHED_RR)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_RR\n"));

      sched_policy = ACE_SCHED_RR;
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_OTHER\n"));

      sched_policy = ACE_SCHED_OTHER;
    }

  /// Check sched.
  int min_priority = ACE_Sched_Params::priority_min (sched_policy);
  int max_priority = ACE_Sched_Params::priority_max (sched_policy);

   if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "max_priority = %d, min_priority = %d\n",
                  max_priority, min_priority));

      if (max_priority == min_priority)
        {
          ACE_DEBUG ((LM_DEBUG,"Detected max_priority == min_priority\n"));
        }
    }

  // Set the main thread to min priority...
    int priority = min_priority;

    if (ACE_OS::sched_params (ACE_Sched_Params (sched_policy ,
                                                priority,
                                                ACE_SCOPE_PROCESS)) != 0)
      {
        if (ACE_OS::last_error () == EPERM)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t): user is not superuser, "
                        "test runs in time-shared class\n"));
          }
        else
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t): sched_params failed\n"));
      }
}

void
TAO_NS_CosNotify_Service::init_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  this->factory_ = ACE_Dynamic_Service<TAO_NS_Factory>::instance ("TAO_NS_Factory");

  if (this->factory_ == 0)
    ACE_NEW_THROW_EX (this->factory_,
                      TAO_NS_Default_Factory (),
                      CORBA::NO_MEMORY ());
  ACE_CHECK;

  TAO_NS_PROPERTIES::instance()->factory (this->factory_);
}

void
TAO_NS_CosNotify_Service::init_builder (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->builder_,
                    TAO_NS_Builder (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  TAO_NS_PROPERTIES::instance()->builder (this->builder_);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_NS_CosNotify_Service::create (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  return this->builder_->build_event_channel_factory (poa ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_CosNotify_Service::remove (TAO_NS_EventChannelFactory* /*ecf*/ ACE_ENV_ARG_DECL_NOT_USED)
{
  // NOP.
}

/*********************************************************************************************************************/

ACE_STATIC_SVC_DEFINE (TAO_Notify_Default_EMO_Factory_OLD,
                       ACE_TEXT (TAO_NOTIFY_DEF_EMO_FACTORY_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_NS_CosNotify_Service),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

/*********************************************************************************************************************/
/*
ACE_STATIC_SVC_DEFINE (TAO_NS_CosNotify_Service,
                       ACE_TEXT (TAO_NS_COS_NOTIFICATION_SERVICE_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_NS_CosNotify_Service),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

*/
ACE_FACTORY_DEFINE (TAO_Notify, TAO_NS_CosNotify_Service)

/*********************************************************************************************************************/
