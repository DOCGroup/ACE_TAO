// $Id$

#include "EC_Default_Factory.h"
#include "EC_Priority_Dispatching.h"
#include "EC_MT_Dispatching.h"
#include "EC_Basic_Filter_Builder.h"
#include "EC_Sched_Filter_Builder.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_Trivial_Supplier_Filter.h"
#include "EC_Per_Supplier_Filter.h"
#include "EC_ObserverStrategy.h"
#include "EC_Null_Scheduling.h"
#include "EC_Priority_Scheduling.h"
#include "EC_ProxyPushSupplier_Set_T.h"
#include "EC_Reactive_Timeout_Generator.h"
#include "EC_Event_Channel.h"
#include "EC_Reactive_ConsumerControl.h"
#include "EC_Reactive_SupplierControl.h"
#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Default_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Default_Factory, "$Id$")

TAO_EC_Default_Factory::~TAO_EC_Default_Factory (void)
{
}

int
TAO_EC_Default_Factory::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_EC_Default_Factory);
}

int
TAO_EC_Default_Factory::init (int argc, char* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO) +
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  this->dispatching_threads_priority_ =
    ACE_Sched_Params::next_priority (ACE_SCHED_FIFO, priority);

  while (arg_shifter.is_anything_left ())
    {
      char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, "-ECDispatching") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "reactive") == 0)
                {
                  this->dispatching_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->dispatching_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "mt") == 0)
                {
                  this->dispatching_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported dispatching <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECDispatchingThreads") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              this->dispatching_threads_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECFiltering") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->filtering_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "basic") == 0)
                {
                  this->filtering_ = 1;
                }
               else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->filtering_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported filtering <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECSupplierFiltering") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->supplier_filtering_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "per-supplier") == 0)
                {
                  this->supplier_filtering_ = 1;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported supplier filtering <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECTimeout") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "reactive") == 0)
                {
                  this->timeout_ = 0;
                }
#if 0
              else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->timeout_ = 1;
                }
#endif /* 0 */
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported timeout <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECObserver") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->observer_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "basic") == 0)
                {
                  this->observer_ = 1;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported observer <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECScheduling") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->scheduling_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->scheduling_ = 1;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported scheduling <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECPushSupplierSet") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "immediate") == 0)
                {
                  this->supplier_set_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "delayed") == 0)
                {
                  this->supplier_set_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "immediate_st") == 0)
                {
                  this->supplier_set_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported supplier set <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECProxyConsumerLock") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->consumer_lock_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "thread") == 0)
                {
                  this->consumer_lock_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "recursive") == 0)
                {
                  this->consumer_lock_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported consumer lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECProxySupplierLock") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->supplier_lock_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "thread") == 0)
                {
                  this->supplier_lock_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "recursive") == 0)
                {
                  this->supplier_lock_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported supplier lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECConsumerAdminLock") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->consumer_admin_lock_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "thread") == 0)
                {
                  this->consumer_admin_lock_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "recursive") == 0)
                {
                  this->consumer_admin_lock_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported consumer admin lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECSupplierAdminLock") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->supplier_admin_lock_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "thread") == 0)
                {
                  this->supplier_admin_lock_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "recursive") == 0)
                {
                  this->supplier_admin_lock_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported supplier lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECUseORBId") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              // Save argument for later use
              this->orbid_ = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECConsumerControl") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->consumer_control_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "reactive") == 0)
                {
                  this->consumer_control_ = 1;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported consumer control <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECSupplierControl") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->supplier_control_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "reactive") == 0)
                {
                  this->supplier_control_ = 1;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Default_Factory - "
                              "unsupported supplier control <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECConsumerControlPeriod") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              this->consumer_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECSupplierControlPeriod") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              this->supplier_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strncmp (arg, "-EC", 3) == 0)
        {
          arg_shifter.consume_arg ();
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "unknowns option <%s>\n",
                      arg));
        }
    }
  return 0;
}

int
TAO_EC_Default_Factory::fini (void)
{
  return 0;
}

// ****************************************************************

TAO_EC_Dispatching*
TAO_EC_Default_Factory::create_dispatching (TAO_EC_Event_Channel *ec)
{
  if (this->dispatching_ == 0)
    return new TAO_EC_Reactive_Dispatching ();
  else if (this->dispatching_ == 1)
    return new TAO_EC_Priority_Dispatching (ec);
  else if (this->dispatching_ == 2)
    return new TAO_EC_MT_Dispatching (this->dispatching_threads_,
                                      this->dispatching_threads_flags_,
                                      this->dispatching_threads_priority_,
                                      this->dispatching_threads_force_active_);
  return 0;
}

void
TAO_EC_Default_Factory::destroy_dispatching (TAO_EC_Dispatching *x)
{
  delete x;
}

TAO_EC_Filter_Builder*
TAO_EC_Default_Factory::create_filter_builder (TAO_EC_Event_Channel *ec)
{
  if (this->filtering_ == 0)
    return new TAO_EC_Null_Filter_Builder ();
  else if (this->filtering_ == 1)
    return new TAO_EC_Basic_Filter_Builder (ec);
  else if (this->filtering_ == 2)
    return new TAO_EC_Sched_Filter_Builder (ec);
  return 0;
}

void
TAO_EC_Default_Factory::destroy_filter_builder (TAO_EC_Filter_Builder *x)
{
  delete x;
}

TAO_EC_Supplier_Filter_Builder*
TAO_EC_Default_Factory::create_supplier_filter_builder (TAO_EC_Event_Channel *ec)
{
  if (this->supplier_filtering_ == 0)
    return new TAO_EC_Trivial_Supplier_Filter_Builder (ec);
  else if (this->supplier_filtering_ == 1)
    return new TAO_EC_Per_Supplier_Filter_Builder (ec);
  return 0;
}

void
TAO_EC_Default_Factory::destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder *x)
{
  delete x;
}

TAO_EC_ConsumerAdmin*
TAO_EC_Default_Factory::create_consumer_admin (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_ConsumerAdmin (ec);
}

void
TAO_EC_Default_Factory::destroy_consumer_admin (TAO_EC_ConsumerAdmin *x)
{
  delete x;
}

TAO_EC_SupplierAdmin*
TAO_EC_Default_Factory::create_supplier_admin (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_SupplierAdmin (ec);
}

void
TAO_EC_Default_Factory::destroy_supplier_admin (TAO_EC_SupplierAdmin *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier*
TAO_EC_Default_Factory::create_proxy_push_supplier (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_ProxyPushSupplier (ec);
}

void
TAO_EC_Default_Factory::destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier *x)
{
  delete x;
}

TAO_EC_ProxyPushConsumer*
TAO_EC_Default_Factory::create_proxy_push_consumer (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_ProxyPushConsumer (ec);
}

void
TAO_EC_Default_Factory::destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer *x)
{
  delete x;
}

TAO_EC_Timeout_Generator*
TAO_EC_Default_Factory::create_timeout_generator (TAO_EC_Event_Channel *)
{
  if (this->timeout_ == 0)
    {
      int argc = 0;
      char **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv,
                         this->orbid_);
      ACE_Reactor *reactor = orb->orb_core ()->reactor ();
      return new TAO_EC_Reactive_Timeout_Generator (reactor);
    }
#if 0
  else if (this->timeout_ == 1)
    {
    }
#endif
  return 0;
}

void
TAO_EC_Default_Factory::destroy_timeout_generator (TAO_EC_Timeout_Generator *x)
{
  delete x;
}

TAO_EC_ObserverStrategy*
TAO_EC_Default_Factory::create_observer_strategy (TAO_EC_Event_Channel *ec)
{
  if (this->observer_ == 0)
    return new TAO_EC_Null_ObserverStrategy;
  else if (this->observer_ == 1)
    {
      // @@ The lock should also be under control of the user...
      ACE_Lock* lock;
      ACE_NEW_RETURN (lock, ACE_Lock_Adapter<ACE_SYNCH_MUTEX>, 0);
      return new TAO_EC_Basic_ObserverStrategy (ec, lock);
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_observer_strategy (TAO_EC_ObserverStrategy *x)
{
  delete x;
}

TAO_EC_Scheduling_Strategy*
TAO_EC_Default_Factory::create_scheduling_strategy (TAO_EC_Event_Channel* ec)
{
  if (this->scheduling_ == 0)
    return new TAO_EC_Null_Scheduling;
  else if (this->scheduling_ == 1)
    {
      RtecScheduler::Scheduler_var scheduler = ec->scheduler ();
      return new TAO_EC_Priority_Scheduling (scheduler.in ());
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy* x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier_Set*
TAO_EC_Default_Factory::create_proxy_push_supplier_set (TAO_EC_Event_Channel *)
{
  if (this->supplier_set_ == 0)
    return new TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_SYNCH_MUTEX> ();
  else if (this->supplier_set_ == 1)
    return new TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH> ();
  else if (this->supplier_set_ == 2)
    return new TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_Null_Mutex> ();
  return 0;
}

void
TAO_EC_Default_Factory::destroy_proxy_push_supplier_set (TAO_EC_ProxyPushSupplier_Set *x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Default_Factory::create_consumer_lock (void)
{
  if (this->consumer_lock_ == 0)
    return new ACE_Lock_Adapter<ACE_Null_Mutex>;
  else if (this->consumer_lock_ == 1)
    return new ACE_Lock_Adapter<ACE_SYNCH_MUTEX> ();
  else if (this->consumer_lock_ == 2)
    return new ACE_Lock_Adapter<ACE_SYNCH_RECURSIVE_MUTEX> ();
  return 0;
}

void
TAO_EC_Default_Factory::destroy_consumer_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Default_Factory::create_supplier_lock (void)
{
  if (this->supplier_lock_ == 0)
    return new ACE_Lock_Adapter<ACE_Null_Mutex>;
  else if (this->supplier_lock_ == 1)
    return new ACE_Lock_Adapter<ACE_SYNCH_MUTEX> ();
  else if (this->supplier_lock_ == 2)
    return new ACE_Lock_Adapter<ACE_SYNCH_RECURSIVE_MUTEX> ();
  return 0;
}

void
TAO_EC_Default_Factory::destroy_supplier_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Default_Factory::create_consumer_admin_lock (void)
{
  if (this->consumer_admin_lock_ == 0)
    return new ACE_Lock_Adapter<ACE_Null_Mutex>;
  else if (this->consumer_admin_lock_ == 1)
    return new ACE_Lock_Adapter<ACE_SYNCH_MUTEX> ();
  else if (this->consumer_admin_lock_ == 2)
    return new ACE_Lock_Adapter<ACE_SYNCH_RECURSIVE_MUTEX> ();
  return 0;
}

void
TAO_EC_Default_Factory::destroy_consumer_admin_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Default_Factory::create_supplier_admin_lock (void)
{
  if (this->supplier_admin_lock_ == 0)
    return new ACE_Lock_Adapter<ACE_Null_Mutex>;
  else if (this->supplier_admin_lock_ == 1)
    return new ACE_Lock_Adapter<ACE_SYNCH_MUTEX> ();
  else if (this->supplier_admin_lock_ == 2)
    return new ACE_Lock_Adapter<ACE_SYNCH_RECURSIVE_MUTEX> ();
  return 0;
}

void
TAO_EC_Default_Factory::destroy_supplier_admin_lock (ACE_Lock* x)
{
  delete x;
}

TAO_EC_ConsumerControl*
TAO_EC_Default_Factory::create_consumer_control (TAO_EC_Event_Channel* ec)
{
  if (this->consumer_control_ == 0)
    return new TAO_EC_ConsumerControl ();
  else if (this->consumer_control_ == 1)
    {
      int argc = 0;
      char **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->consumer_control_period_);
      return new TAO_EC_Reactive_ConsumerControl (rate, ec, orb.in ());
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_consumer_control (TAO_EC_ConsumerControl* x)
{
  delete x;
}

TAO_EC_SupplierControl*
TAO_EC_Default_Factory::create_supplier_control (TAO_EC_Event_Channel* ec)
{
  if (this->supplier_control_ == 0)
    return new TAO_EC_SupplierControl ();
  else if (this->supplier_control_ == 1)
    {
      int argc = 0;
      char **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->consumer_control_period_);
      return new TAO_EC_Reactive_SupplierControl (rate, ec, orb.in ());
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_supplier_control (TAO_EC_SupplierControl* x)
{
  delete x;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_EC_Default_Factory,
                       ASYS_TEXT ("EC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Default_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_ORBSVCS, TAO_EC_Default_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if defined (ACE_HAS_THREADS)
template class TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_SYNCH_MUTEX>;
#endif /* ACE_HAS_THREADS */

template class TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_Null_Mutex>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if defined (ACE_HAS_THREADS)
#pragma instantiate TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_THREADS */

#pragma instantiate TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
