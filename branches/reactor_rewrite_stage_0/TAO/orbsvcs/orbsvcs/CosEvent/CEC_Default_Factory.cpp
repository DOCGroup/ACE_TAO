// $Id$

#include "CEC_Default_Factory.h"
#include "CEC_MT_Dispatching.h"
#include "CEC_Reactive_Pulling_Strategy.h"
#include "CEC_ConsumerAdmin.h"
#include "CEC_SupplierAdmin.h"
#include "CEC_ProxyPushConsumer.h"
#include "CEC_ProxyPullConsumer.h"
#include "CEC_ProxyPushSupplier.h"
#include "CEC_ProxyPullSupplier.h"
#include "CEC_EventChannel.h"
#include "CEC_Reactive_ConsumerControl.h"
#include "CEC_Reactive_SupplierControl.h"
#include "orbsvcs/ESF/ESF_Immediate_Changes.h"
#include "orbsvcs/ESF/ESF_Delayed_Changes.h"
#include "orbsvcs/ESF/ESF_Copy_On_Write.h"
#include "orbsvcs/ESF/ESF_Copy_On_Read.h"
#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.h"
#include "orbsvcs/ESF/ESF_Delayed_Command.h"
#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_Default_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_Default_Factory, "$Id$")

TAO_CEC_Default_Factory::~TAO_CEC_Default_Factory (void)
{
}

int
TAO_CEC_Default_Factory::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_CEC_Default_Factory);
}

int
TAO_CEC_Default_Factory::parse_collection_arg (char *opt)
{
  int collection_type = 0;
  int synch_type = 0;
  int iteration_type = 0;

  char* aux = 0;
  for (char* arg = ACE_OS::strtok_r (opt, ":", &aux);
       arg != 0;
       arg = ACE_OS::strtok_r (0, ":", &aux))
    {
      if (ACE_OS::strcasecmp (arg, "mt") == 0)
        synch_type = 0;
      else if (ACE_OS::strcasecmp (arg, "st") == 0)
        synch_type = 1;
      else if (ACE_OS::strcasecmp (arg, "list") == 0)
        collection_type = 0;
      else if (ACE_OS::strcasecmp (arg, "rb_tree") == 0)
        collection_type = 1;
      else if (ACE_OS::strcasecmp (arg, "immediate") == 0)
        iteration_type = 0;
      else if (ACE_OS::strcasecmp (arg, "copy_on_read") == 0)
        iteration_type = 1;
      else if (ACE_OS::strcasecmp (arg, "copy_on_write") == 0)
        iteration_type = 2;
      else if (ACE_OS::strcasecmp (arg, "delayed") == 0)
        iteration_type = 3;
      else
        ACE_ERROR ((LM_ERROR,
                    "CEC_Default_Factory - "
                    "unknown collection modifier <%s>\n",
                    arg));
    }
  return (synch_type << 8) |(collection_type << 4) | iteration_type;
}

int
TAO_CEC_Default_Factory::init (int argc, char* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  this->dispatching_threads_flags_ =
    THR_SCHED_DEFAULT|THR_BOUND|THR_NEW_LWP;
  this->dispatching_threads_priority_ =
    ACE_THR_PRI_OTHER_DEF;

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, "-CECDispatching") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "reactive") == 0)
                {
                  this->dispatching_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "mt") == 0)
                {
                  this->dispatching_ = 1;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "CEC_Default_Factory - "
                              "unsupported dispatching <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECDispatchingThreads") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              this->dispatching_threads_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECProxyConsumerCollection") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char *current_arg = arg_shifter.get_current ();
              char *opt = ACE_OS::strdup (current_arg);
              this->consumer_collection_ = this->parse_collection_arg (opt);
              ACE_OS::free (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECProxySupplierCollection") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char *current_arg = arg_shifter.get_current();
              char* opt = ACE_OS::strdup(current_arg);
              this->supplier_collection_ =
                this->parse_collection_arg (opt);

	      ACE_OS::free(opt);  
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECProxyConsumerLock") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
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
                              "CEC_Default_Factory - "
                              "unsupported consumer lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECProxySupplierLock") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
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
                              "CEC_Default_Factory - "
                              "unsupported supplier lock <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECReactivePullingPeriod") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              this->reactive_pulling_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECUseORBId") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              // Save argument for later use
              this->orbid_ = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECConsumerControl") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
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
                              "CEC_Default_Factory - "
                              "unsupported consumer control <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECSupplierControl") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
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
                              "CEC_Default_Factory - "
                              "unsupported supplier control <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECConsumerControlPeriod") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              this->consumer_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-CECSupplierControlPeriod") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              this->supplier_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strncmp (arg, "-CEC", 3) == 0)
        {
          arg_shifter.consume_arg ();
          ACE_ERROR ((LM_ERROR,
                      "CEC_Default_Factory - "
                      "unknown option <%s>\n",
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
      char **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->reactive_pulling_period_);
      return new TAO_CEC_Reactive_Pulling_Strategy (rate, ec,
                                                    orb.in ());
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

void
TAO_CEC_Default_Factory::destroy_consumer_admin (TAO_CEC_ConsumerAdmin *x)
{
  delete x;
}

TAO_CEC_SupplierAdmin*
TAO_CEC_Default_Factory::create_supplier_admin (TAO_CEC_EventChannel *ec)
{
  return new TAO_CEC_SupplierAdmin (ec);
}

void
TAO_CEC_Default_Factory::destroy_supplier_admin (TAO_CEC_SupplierAdmin *x)
{
  delete x;
}

TAO_CEC_ProxyPushSupplier*
TAO_CEC_Default_Factory::create_proxy_push_supplier (TAO_CEC_EventChannel *ec)
{
  return new TAO_CEC_ProxyPushSupplier (ec);
}

void
TAO_CEC_Default_Factory::destroy_proxy_push_supplier (TAO_CEC_ProxyPushSupplier *x)
{
  delete x;
}

TAO_CEC_ProxyPullSupplier*
TAO_CEC_Default_Factory::create_proxy_pull_supplier (TAO_CEC_EventChannel *ec)
{
  return new TAO_CEC_ProxyPullSupplier (ec);
}

void
TAO_CEC_Default_Factory::destroy_proxy_pull_supplier (TAO_CEC_ProxyPullSupplier *x)
{
  delete x;
}

TAO_CEC_ProxyPushConsumer*
TAO_CEC_Default_Factory::create_proxy_push_consumer (TAO_CEC_EventChannel *ec)
{
  return new TAO_CEC_ProxyPushConsumer (ec);
}

void
TAO_CEC_Default_Factory::destroy_proxy_push_consumer (TAO_CEC_ProxyPushConsumer *x)
{
  delete x;
}

TAO_CEC_ProxyPullConsumer*
TAO_CEC_Default_Factory::create_proxy_pull_consumer (TAO_CEC_EventChannel *ec)
{
  return new TAO_CEC_ProxyPullConsumer (ec);
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

void
TAO_CEC_Default_Factory::destroy_proxy_push_consumer_collection (TAO_CEC_ProxyPushConsumer_Collection *x)
{
  delete x;
}

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
      char **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->consumer_control_period_);
      return new TAO_CEC_Reactive_ConsumerControl (rate, ec, orb.in ());
    }
  return 0;
}

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
      char **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_);

      ACE_Time_Value rate (0, this->consumer_control_period_);
      return new TAO_CEC_Reactive_SupplierControl (rate, ec, orb.in ());
    }
  return 0;
}

void
TAO_CEC_Default_Factory::destroy_supplier_control (TAO_CEC_SupplierControl* x)
{
  delete x;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_CEC_Default_Factory,
                       ACE_TEXT ("CEC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CEC_Default_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_ORBSVCS, TAO_CEC_Default_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if !defined(TAO_ORBSVCS_HAS_Event2)
template class ACE_Node<ACE_Command_Base*>;
template class ACE_Unbounded_Queue<ACE_Command_Base*>;
template class ACE_Unbounded_Queue_Iterator<ACE_Command_Base*>;
#endif /* ! TAO_ORBSVCS_HAS_Event2 */

template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_List_Iterator >;
template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_RB_Tree_Iterator >;

template class TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPushConsumer>;
template class TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>;
template class TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>;
template class ACE_Unbounded_Set<TAO_CEC_ProxyPushConsumer *>;
template class ACE_Node<TAO_CEC_ProxyPushConsumer *>;
template class ACE_Unbounded_Set_Iterator<TAO_CEC_ProxyPushConsumer *>;
template class ACE_RB_Tree<TAO_CEC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<TAO_CEC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<TAO_CEC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<TAO_CEC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<TAO_CEC_ProxyPushConsumer *, int>;
template class ACE_Less_Than<TAO_CEC_ProxyPushConsumer *>;
template class TAO_ESF_Proxy_RB_Tree_Iterator<TAO_CEC_ProxyPushConsumer>;

template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_List_Iterator >;
template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_RB_Tree_Iterator >;

template class TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPushSupplier>;
template class TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>;
template class TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>;
template class ACE_Unbounded_Set<TAO_CEC_ProxyPushSupplier *>;
template class ACE_Node<TAO_CEC_ProxyPushSupplier *>;
template class ACE_Unbounded_Set_Iterator<TAO_CEC_ProxyPushSupplier *>;
template class ACE_RB_Tree<TAO_CEC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<TAO_CEC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<TAO_CEC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<TAO_CEC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<TAO_CEC_ProxyPushSupplier *, int>;
template class ACE_Less_Than<TAO_CEC_ProxyPushSupplier *>;
template class TAO_ESF_Proxy_RB_Tree_Iterator<TAO_CEC_ProxyPushSupplier>;

template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_List_Iterator >;
template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_RB_Tree_Iterator >;

template class TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPullConsumer>;
template class TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>;
template class TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>;
template class ACE_Unbounded_Set<TAO_CEC_ProxyPullConsumer *>;
template class ACE_Node<TAO_CEC_ProxyPullConsumer *>;
template class ACE_Unbounded_Set_Iterator<TAO_CEC_ProxyPullConsumer *>;
template class ACE_RB_Tree<TAO_CEC_ProxyPullConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPullConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<TAO_CEC_ProxyPullConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPullConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<TAO_CEC_ProxyPullConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPullConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<TAO_CEC_ProxyPullConsumer *, int, ACE_Less_Than<TAO_CEC_ProxyPullConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<TAO_CEC_ProxyPullConsumer *, int>;
template class ACE_Less_Than<TAO_CEC_ProxyPullConsumer *>;
template class TAO_ESF_Proxy_RB_Tree_Iterator<TAO_CEC_ProxyPullConsumer>;

template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_List_Iterator >;
template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_RB_Tree_Iterator >;

template class TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPullSupplier>;
template class TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>;
template class TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>;
template class ACE_Unbounded_Set<TAO_CEC_ProxyPullSupplier *>;
template class ACE_Node<TAO_CEC_ProxyPullSupplier *>;
template class ACE_Unbounded_Set_Iterator<TAO_CEC_ProxyPullSupplier *>;
template class ACE_RB_Tree<TAO_CEC_ProxyPullSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPullSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<TAO_CEC_ProxyPullSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPullSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<TAO_CEC_ProxyPullSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPullSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<TAO_CEC_ProxyPullSupplier *, int, ACE_Less_Than<TAO_CEC_ProxyPullSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<TAO_CEC_ProxyPullSupplier *, int>;
template class ACE_Less_Than<TAO_CEC_ProxyPullSupplier *>;
template class TAO_ESF_Proxy_RB_Tree_Iterator<TAO_CEC_ProxyPullSupplier>;

#if defined (ACE_HAS_THREADS)
//
// To avoid duplicate instantiations of templates we must put the MT
// versions on this #ifdef, otherwise the ACE_SYNCH* macros expand to
// the ACE_NULL* versions, duplicating the non-MT versions below.
// We *cannot* use explicit ACE_Synch classes because that will not
// compile in platforms without threads.
//
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
     TAO_CEC_PushConsumer_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_SYNCH> >;

template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_SYNCH> >;

template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_List_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_List_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_RB_Tree_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_RB_Tree_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_List_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_List_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_RB_Tree_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_RB_Tree_Iterator,
        ACE_SYNCH>;

template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_List_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_List_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_RB_Tree_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_RB_Tree_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_List_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_List_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_RB_Tree_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_RB_Tree_Iterator,
        ACE_SYNCH>;

#endif /* ACE_HAS_THREADS */

template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_List_Iterator,
      ACE_NULL_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
      TAO_CEC_PushConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;

template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_List_Iterator,
      ACE_NULL_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
      TAO_CEC_PushSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;

#if defined (ACE_HAS_THREADS)
//
// To avoid duplicate instantiations of templates we must put the MT
// versions on this #ifdef, otherwise the ACE_SYNCH* macros expand to
// the ACE_NULL* versions, duplicating the non-MT versions below.
// We *cannot* use explicit ACE_Synch classes because that will not
// compile in platforms without threads.
//
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
     TAO_CEC_PullConsumer_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_SYNCH> >;

template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_SYNCH> >;

#endif /* ACE_HAS_THREADS */

template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_List_Iterator,
      ACE_NULL_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
      TAO_CEC_PullConsumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;

template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_List_Iterator,
      ACE_NULL_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_CEC_ProxyPullSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_CEC_ProxyPullSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
      TAO_CEC_PullSupplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;

template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_List_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_List_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_RB_Tree_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_RB_Tree_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_List_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_List_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushConsumer>,
    TAO_CEC_PushConsumer_RB_Tree_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPushSupplier>,
    TAO_CEC_PushSupplier_RB_Tree_Iterator,
        ACE_NULL_SYNCH>;

template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_List_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_List_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_RB_Tree_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_RB_Tree_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_List_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_List_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullConsumer>,
    TAO_CEC_PullConsumer_RB_Tree_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_CEC_ProxyPullSupplier>,
    TAO_CEC_PullSupplier_RB_Tree_Iterator,
        ACE_NULL_SYNCH>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if !defined(TAO_ORBSVCS_HAS_Event2)
#pragma instantiate ACE_Node<ACE_Command_Base*>
#pragma instantiate ACE_Unbounded_Queue<ACE_Command_Base*>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Command_Base*>
#endif /* ! TAO_ORBSVCS_HAS_Event2 */

#if defined (ACE_HAS_THREADS)
#endif /* ACE_HAS_THREADS */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
