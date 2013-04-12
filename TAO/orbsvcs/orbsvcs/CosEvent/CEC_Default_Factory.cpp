// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "orbsvcs/CosEvent/CEC_MT_Dispatching.h"
#include "orbsvcs/CosEvent/CEC_Reactive_Pulling_Strategy.h"
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_SupplierAdmin.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushConsumer.h"
#include "orbsvcs/CosEvent/CEC_ProxyPullConsumer.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.h"
#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Reactive_ConsumerControl.h"
#include "orbsvcs/CosEvent/CEC_Reactive_SupplierControl.h"

#include "orbsvcs/ESF/ESF_Immediate_Changes.h"
#include "orbsvcs/ESF/ESF_Delayed_Changes.h"
#include "orbsvcs/ESF/ESF_Copy_On_Write.h"
#include "orbsvcs/ESF/ESF_Copy_On_Read.h"
#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/Messaging/Messaging_RT_PolicyC.h"

#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_strings.h"
#include "ace/Null_Mutex.h"
#include "ace/Null_Condition.h"
#include "ace/Recursive_Thread_Mutex.h"

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
#include "orbsvcs/CosEvent/CEC_TypedConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_TypedSupplierAdmin.h"
#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Default_Factory.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CEC_Default_Factory::~TAO_CEC_Default_Factory (void)
{
    if (orbid_dupped_ != 0)
      {
        ACE_OS::free (orbid_);
      }
}

int
TAO_CEC_Default_Factory::init_svcs (void)
{
  return
    ACE_Service_Config::static_svcs ()->insert (
        &ace_svc_desc_TAO_CEC_Default_Factory
      );
}

int
TAO_CEC_Default_Factory::parse_collection_arg (ACE_TCHAR* opt)
{
  int collection_type = 0;
  int synch_type = 0;
  int iteration_type = 0;

  ACE_TCHAR* aux = 0;
  for (ACE_TCHAR* arg = ACE_OS::strtok_r (opt, ACE_TEXT(":"), &aux);
       arg != 0;
       arg = ACE_OS::strtok_r (0, ACE_TEXT(":"), &aux))
    {
      if (ACE_OS::strcasecmp (arg, ACE_TEXT("mt")) == 0)
        synch_type = 0;
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("st")) == 0)
        synch_type = 1;
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("list")) == 0)
        collection_type = 0;
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("rb_tree")) == 0)
        collection_type = 1;
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("immediate")) == 0)
        iteration_type = 0;
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("copy_on_read")) == 0)
        iteration_type = 1;
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("copy_on_write")) == 0)
        iteration_type = 2;
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("delayed")) == 0)
        iteration_type = 3;
      else
        ORBSVCS_ERROR ((LM_ERROR,
                    "CEC_Default_Factory - "
                    "unknown collection modifier <%s>\n",
                    arg));
    }
  return (synch_type << 8) |(collection_type << 4) | iteration_type;
}

int
TAO_CEC_Default_Factory::init (int argc, ACE_TCHAR* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  this->dispatching_threads_flags_ =
    THR_SCHED_DEFAULT|THR_BOUND|THR_NEW_LWP;
  this->dispatching_threads_priority_ =
    ACE_THR_PRI_OTHER_DEF;

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECDispatching")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                {
                  this->dispatching_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("mt")) == 0)
                {
                  this->dispatching_ = 1;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              "CEC_Default_Factory - "
                              "unsupported dispatching <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECDispatchingThreads")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->dispatching_threads_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECProxyConsumerCollection")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR *current_arg = arg_shifter.get_current ();
              ACE_TCHAR *opt = ACE_OS::strdup (current_arg);
              this->consumer_collection_ = this->parse_collection_arg (opt);
              ACE_OS::free (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECProxySupplierCollection")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR *current_arg = arg_shifter.get_current();
              ACE_TCHAR* opt = ACE_OS::strdup(current_arg);
              this->supplier_collection_ =
                this->parse_collection_arg (opt);
              ACE_OS::free(opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECProxyConsumerLock")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                {
                  this->consumer_lock_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("thread")) == 0)
                {
                  this->consumer_lock_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("recursive")) == 0)
                {
                  this->consumer_lock_ = 2;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              "CEC_Default_Factory - "
                              "unsupported consumer lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECProxySupplierLock")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                {
                  this->supplier_lock_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("thread")) == 0)
                {
                  this->supplier_lock_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("recursive")) == 0)
                {
                  this->supplier_lock_ = 2;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              "CEC_Default_Factory - "
                              "unsupported supplier lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECReactivePullingPeriod")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->reactive_pulling_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECUseORBId")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              // Copy argument for later use
              this->orbid_ = ACE_OS::strdup(ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ()));
              orbid_dupped_ = 1;
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECConsumerControl")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                {
                  this->consumer_control_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                {
                  this->consumer_control_ = 1;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              "CEC_Default_Factory - "
                              "unsupported consumer control <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECSupplierControl")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                {
                  this->supplier_control_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                {
                  this->supplier_control_ = 1;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              "CEC_Default_Factory - "
                              "unsupported supplier control <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECConsumerControlPeriod")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->consumer_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECSupplierControlPeriod")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->supplier_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECConsumerControlTimeout")) == 0 ||
               ACE_OS::strcasecmp (arg, ACE_TEXT("-CECConsumerControlRoundtripTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
                          unsigned long timeout_ = ACE_OS::strtoul(opt, 0, 10);
              this->consumer_control_timeout_.usec(timeout_);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECSupplierControlTimeout")) == 0 ||
               ACE_OS::strcasecmp (arg, ACE_TEXT("-CECSupplierControlRoundtripTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
                          unsigned long timeout_ = ACE_OS::strtoul(opt, 0, 10);
              this->supplier_control_timeout_.usec(timeout_);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECConsumerOperationTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              unsigned long timeout = ACE_OS::strtoul (opt, 0, 10);
              this->consumer_timeout_.usec (timeout);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECSupplierOperationTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              unsigned long timeout = ACE_OS::strtoul (opt, 0, 10);
              this->supplier_timeout_.usec (timeout);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-CECProxyDisconnectRetries")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->proxy_disconnect_retries_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strncmp (arg, ACE_TEXT("-CEC"), 3) == 0)
        {
          arg_shifter.consume_arg ();
          ORBSVCS_ERROR ((LM_ERROR,
                      "CEC_Default_Factory - "
                      "unknown option <%s>\n",
                      arg));
        }
      else
        {
          arg_shifter.consume_arg ();
          ORBSVCS_DEBUG ((LM_DEBUG,
                      "CEC_Default_Factory - "
                      "ignoring option <%s>\n",
                      arg));
        }
    }
  return 0;
}

int
TAO_CEC_Default_Factory::fini (void)
{
  return 0;
}

// ****************************************************************

TAO_CEC_Dispatching*
TAO_CEC_Default_Factory::create_dispatching (TAO_CEC_EventChannel *)
{
  if (this->dispatching_ == 0)
    return new TAO_CEC_Reactive_Dispatching ();
  else if (this->dispatching_ == 1)
    return new TAO_CEC_MT_Dispatching (this->dispatching_threads_,
                                      this->dispatching_threads_flags_,
                                      this->dispatching_threads_priority_,
                                      this->dispatching_threads_force_active_);
  return 0;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_Dispatching*
TAO_CEC_Default_Factory::create_dispatching (TAO_CEC_TypedEventChannel *)
{
  if (this->dispatching_ == 0)
    return new TAO_CEC_Reactive_Dispatching ();
  else if (this->dispatching_ == 1)
    return new TAO_CEC_MT_Dispatching (this->dispatching_threads_,
                                      this->dispatching_threads_flags_,
                                      this->dispatching_threads_priority_,
                                      this->dispatching_threads_force_active_);
  return 0;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_dispatching (TAO_CEC_Dispatching *x)
{
  delete x;
}

TAO_CEC_Pulling_Strategy*
TAO_CEC_Default_Factory::create_pulling_strategy (TAO_CEC_EventChannel *ec)
{
  if (this->pulling_strategy_ == 0)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->reactive_pulling_period_);
      return new TAO_CEC_Reactive_Pulling_Strategy (
                                 rate, this->supplier_control_timeout_,
                                 ec, orb.in ());
    }
  return 0;
}

void
TAO_CEC_Default_Factory::destroy_pulling_strategy (TAO_CEC_Pulling_Strategy *x)
{
  delete x;
}

TAO_CEC_ConsumerAdmin*
TAO_CEC_Default_Factory::create_consumer_admin (TAO_CEC_EventChannel *ec)
{
  return new TAO_CEC_ConsumerAdmin (ec);
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_TypedConsumerAdmin*
TAO_CEC_Default_Factory::create_consumer_admin (TAO_CEC_TypedEventChannel *ec)
{
  return new TAO_CEC_TypedConsumerAdmin (ec);
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_consumer_admin (TAO_CEC_ConsumerAdmin *x)
{
  delete x;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Default_Factory::destroy_consumer_admin (TAO_CEC_TypedConsumerAdmin *x)
{
  delete x;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_CEC_SupplierAdmin*
TAO_CEC_Default_Factory::create_supplier_admin (TAO_CEC_EventChannel *ec)
{
  return new TAO_CEC_SupplierAdmin (ec);
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_TypedSupplierAdmin*
TAO_CEC_Default_Factory::create_supplier_admin (TAO_CEC_TypedEventChannel *ec)
{
  return new TAO_CEC_TypedSupplierAdmin (ec);
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_supplier_admin (TAO_CEC_SupplierAdmin *x)
{
  delete x;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Default_Factory::destroy_supplier_admin (TAO_CEC_TypedSupplierAdmin *x)
{
  delete x;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_CEC_ProxyPushSupplier*
TAO_CEC_Default_Factory::create_proxy_push_supplier (TAO_CEC_EventChannel *ec)
{
  TAO_CEC_ProxyPushSupplier *created;
  ACE_Time_Value timeout = this->consumer_control_ ? this->consumer_timeout_
    : ACE_Time_Value::zero;
  ACE_NEW_RETURN (created, TAO_CEC_ProxyPushSupplier (ec, timeout), 0);
  return created;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_ProxyPushSupplier*
TAO_CEC_Default_Factory::create_proxy_push_supplier (TAO_CEC_TypedEventChannel *ec)
{
  TAO_CEC_ProxyPushSupplier *created;
  ACE_Time_Value timeout = this->consumer_control_ ? this->consumer_timeout_
    : ACE_Time_Value::zero;
  ACE_NEW_RETURN (created, TAO_CEC_ProxyPushSupplier (ec, timeout), 0);
  return created;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_proxy_push_supplier (TAO_CEC_ProxyPushSupplier *x)
{
  delete x;
}

TAO_CEC_ProxyPullSupplier*
TAO_CEC_Default_Factory::create_proxy_pull_supplier (TAO_CEC_EventChannel *ec)
{
  TAO_CEC_ProxyPullSupplier *created;
  ACE_Time_Value timeout = this->consumer_control_ ? this->consumer_timeout_
    : ACE_Time_Value::zero;
  ACE_NEW_RETURN (created, TAO_CEC_ProxyPullSupplier (ec, timeout), 0);
  return created;
}

void
TAO_CEC_Default_Factory::destroy_proxy_pull_supplier (TAO_CEC_ProxyPullSupplier *x)
{
  delete x;
}

TAO_CEC_ProxyPushConsumer*
TAO_CEC_Default_Factory::create_proxy_push_consumer (TAO_CEC_EventChannel *ec)
{
  TAO_CEC_ProxyPushConsumer *created;
  ACE_Time_Value timeout = this->supplier_control_ ? this->supplier_timeout_
    : ACE_Time_Value::zero;
  ACE_NEW_RETURN (created, TAO_CEC_ProxyPushConsumer (ec, timeout), 0);
  return created;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_TypedProxyPushConsumer*
TAO_CEC_Default_Factory::create_proxy_push_consumer (TAO_CEC_TypedEventChannel *ec)
{
  TAO_CEC_TypedProxyPushConsumer *created;
  ACE_Time_Value timeout = this->supplier_control_ ? this->supplier_timeout_
    : ACE_Time_Value::zero;
  ACE_NEW_RETURN (created, TAO_CEC_TypedProxyPushConsumer (ec, timeout), 0);
  return created;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_proxy_push_consumer (TAO_CEC_ProxyPushConsumer *x)
{
  delete x;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Default_Factory::destroy_proxy_push_consumer (TAO_CEC_TypedProxyPushConsumer *x)
{
  delete x;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_CEC_ProxyPullConsumer*
TAO_CEC_Default_Factory::create_proxy_pull_consumer (TAO_CEC_EventChannel *ec)
{
  TAO_CEC_ProxyPullConsumer *created;
  ACE_Time_Value timeout = this->supplier_control_ ? this->supplier_timeout_
    : ACE_Time_Value::zero;
  ACE_NEW_RETURN (created, TAO_CEC_ProxyPullConsumer (ec, timeout), 0);
  return created;
}

void
TAO_CEC_Default_Factory::destroy_proxy_pull_consumer (TAO_CEC_ProxyPullConsumer *x)
{
  delete x;
}

// These typedefs workaround a SunCC 4.2 bug
typedef
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>::Iterator
    TAO_CEC_PushConsumer_List_Iterator;
typedef
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>::Iterator
    TAO_CEC_PushConsumer_RB_Tree_Iterator;
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
typedef
    TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>::Iterator
    TAO_CEC_TypedPushConsumer_List_Iterator;
typedef
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>::Iterator
    TAO_CEC_TypedPushConsumer_RB_Tree_Iterator;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
typedef
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>::Iterator
    TAO_CEC_PullConsumer_List_Iterator;
typedef
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>::Iterator
    TAO_CEC_PullConsumer_RB_Tree_Iterator;
typedef
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>::Iterator
    TAO_CEC_PushSupplier_List_Iterator;
typedef
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>::Iterator
    TAO_CEC_PushSupplier_RB_Tree_Iterator;
typedef
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>::Iterator
    TAO_CEC_PullSupplier_List_Iterator;
typedef
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>::Iterator
    TAO_CEC_PullSupplier_RB_Tree_Iterator;

TAO_CEC_ProxyPushConsumer_Collection*
TAO_CEC_Default_Factory::create_proxy_push_consumer_collection (TAO_CEC_EventChannel *)
{
  if (this->consumer_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_TypedProxyPushConsumer_Collection*
TAO_CEC_Default_Factory::create_proxy_push_consumer_collection (TAO_CEC_TypedEventChannel *)
{
  if (this->consumer_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_TypedProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_TypedProxyPushConsumer>,
      TAO_CEC_TypedPushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_proxy_push_consumer_collection (TAO_CEC_ProxyPushConsumer_Collection *x)
{
  delete x;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Default_Factory::destroy_proxy_push_consumer_collection (TAO_CEC_TypedProxyPushConsumer_Collection *x)
{
  delete x;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_CEC_ProxyPullConsumer_Collection*
TAO_CEC_Default_Factory::create_proxy_pull_consumer_collection (TAO_CEC_EventChannel *)
{
  if (this->consumer_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}

void
TAO_CEC_Default_Factory::destroy_proxy_pull_consumer_collection (TAO_CEC_ProxyPullConsumer_Collection *x)
{
  delete x;
}

TAO_CEC_ProxyPushSupplier_Collection*
TAO_CEC_Default_Factory::create_proxy_push_supplier_collection (TAO_CEC_EventChannel *)
{
  if (this->supplier_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_ProxyPushSupplier_Collection*
TAO_CEC_Default_Factory::create_proxy_push_supplier_collection (TAO_CEC_TypedEventChannel *)
{
  if (this->supplier_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_proxy_push_supplier_collection (TAO_CEC_ProxyPushSupplier_Collection *x)
{
  delete x;
}

TAO_CEC_ProxyPullSupplier_Collection*
TAO_CEC_Default_Factory::create_proxy_pull_supplier_collection (TAO_CEC_EventChannel *)
{
  if (this->supplier_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}

void
TAO_CEC_Default_Factory::destroy_proxy_pull_supplier_collection (TAO_CEC_ProxyPullSupplier_Collection *x)
{
  delete x;
}

ACE_Lock*
TAO_CEC_Default_Factory::create_consumer_lock (void)
{
  if (this->consumer_lock_ == 0)
    return new ACE_Lock_Adapter<ACE_Null_Mutex>;
  else if (this->consumer_lock_ == 1)
    return new ACE_Lock_Adapter<TAO_SYNCH_MUTEX> ();
  else if (this->consumer_lock_ == 2)
    return new ACE_Lock_Adapter<TAO_SYNCH_RECURSIVE_MUTEX> ();
  return 0;
}

void
TAO_CEC_Default_Factory::destroy_consumer_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_CEC_Default_Factory::create_supplier_lock (void)
{
  if (this->supplier_lock_ == 0)
    return new ACE_Lock_Adapter<ACE_Null_Mutex>;
  else if (this->supplier_lock_ == 1)
    return new ACE_Lock_Adapter<TAO_SYNCH_MUTEX> ();
  else if (this->supplier_lock_ == 2)
    return new ACE_Lock_Adapter<TAO_SYNCH_RECURSIVE_MUTEX> ();
  return 0;
}

void
TAO_CEC_Default_Factory::destroy_supplier_lock (ACE_Lock* x)
{
  delete x;
}

TAO_CEC_ConsumerControl*
TAO_CEC_Default_Factory::create_consumer_control (TAO_CEC_EventChannel* ec)
{
  if (this->consumer_control_ == 0)
    return new TAO_CEC_ConsumerControl ();
  else if (this->consumer_control_ == 1)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->consumer_control_period_);
      return new TAO_CEC_Reactive_ConsumerControl (
                                 rate, this->consumer_control_timeout_,
                                 this->proxy_disconnect_retries_,
                                 ec, orb.in ());
    }
  return 0;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_ConsumerControl*
TAO_CEC_Default_Factory::create_consumer_control (TAO_CEC_TypedEventChannel* ec)
{
  if (this->consumer_control_ == 0)
    return new TAO_CEC_ConsumerControl ();
  else if (this->consumer_control_ == 1)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->consumer_control_period_);
      return new TAO_CEC_Reactive_ConsumerControl (
                                 rate, this->consumer_control_timeout_,
                                 this->proxy_disconnect_retries_,
                                 ec, orb.in ());
    }
  return 0;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_consumer_control (TAO_CEC_ConsumerControl* x)
{
  delete x;
}

TAO_CEC_SupplierControl*
TAO_CEC_Default_Factory::create_supplier_control (TAO_CEC_EventChannel* ec)
{
  if (this->supplier_control_ == 0)
    return new TAO_CEC_SupplierControl ();
  else if (this->supplier_control_ == 1)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->supplier_control_period_);
      return new TAO_CEC_Reactive_SupplierControl (
                                 rate, this->supplier_control_timeout_,
                                 this->proxy_disconnect_retries_,
                                 ec, orb.in ());
    }
  return 0;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_SupplierControl*
TAO_CEC_Default_Factory::create_supplier_control (TAO_CEC_TypedEventChannel* ec)
{
  if (this->supplier_control_ == 0)
    return new TAO_CEC_SupplierControl ();
  else if (this->supplier_control_ == 1)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->supplier_control_period_);
      return new TAO_CEC_Reactive_SupplierControl (
                                 rate, this->supplier_control_timeout_,
                                 this->proxy_disconnect_retries_,
                                 ec, orb.in ());
    }
  return 0;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Default_Factory::destroy_supplier_control (TAO_CEC_SupplierControl* x)
{
  delete x;
}

CORBA::Policy_ptr TAO_CEC_Default_Factory::create_roundtrip_timeout_policy
(const ACE_Time_Value &timeout)
{
  //get the existing orb
  int fake_argc = 0;
  ACE_TCHAR **fake_argv = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (fake_argc, fake_argv, this->orbid_);

  CORBA::Any value;
  TimeBase::TimeT timet;
  ORBSVCS_Time::Time_Value_to_TimeT (timet, timeout);
  value <<= timet;
  return orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
    value);
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_CEC_Default_Factory,
                       ACE_TEXT ("CEC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CEC_Default_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Event_Serv, TAO_CEC_Default_Factory)
