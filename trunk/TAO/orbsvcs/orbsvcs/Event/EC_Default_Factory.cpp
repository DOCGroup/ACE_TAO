// $Id$

#include "EC_Default_Factory.h"
#include "EC_MT_Dispatching.h"
#include "EC_Basic_Filter_Builder.h"
#include "EC_Prefix_Filter_Builder.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_Trivial_Supplier_Filter.h"
#include "EC_Per_Supplier_Filter.h"
#include "EC_ObserverStrategy.h"
#include "EC_Null_Scheduling.h"
#include "EC_Reactive_Timeout_Generator.h"
#include "EC_Event_Channel.h"
#include "EC_Reactive_ConsumerControl.h"
#include "EC_Reactive_SupplierControl.h"

#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.h"
#include "orbsvcs/ESF/ESF_Immediate_Changes.h"
#include "orbsvcs/ESF/ESF_Copy_On_Read.h"
#include "orbsvcs/ESF/ESF_Copy_On_Write.h"
#include "orbsvcs/ESF/ESF_Delayed_Changes.h"
#include "orbsvcs/ESF/ESF_Delayed_Command.h"

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
              else if (ACE_OS::strcasecmp (opt, "mt") == 0)
                {
                  this->dispatching_ = 1;
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
              else if (ACE_OS::strcasecmp (opt, "prefix") == 0)
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

      else if (ACE_OS::strcasecmp (arg, "-ECSupplierFilter") == 0
               // @@ TODO remove, only for backwards compatibility
               || ACE_OS::strcasecmp (arg, "-ECSupplierFiltering") == 0)
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

      else if (ACE_OS::strcasecmp (arg, "-ECProxyPushConsumerCollection") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              int collection_type = 0;
              int synch_type = 0;
              int iteration_type = 0;

              char* aux;
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
                                "EC_Default_Factory - "
                                "unknown collection modifier <%s>\n",
                                arg));
                }
              this->consumer_collection_ =
                 (synch_type << 8) |(collection_type << 4) | iteration_type;
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECProxyPushSupplierCollection") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              int collection_type = 0;
              int synch_type = 0;
              int iteration_type = 0;

              char* aux;
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
                                "EC_Default_Factory - "
                                "unknown collection modifier <%s>\n",
                                arg));
                }
              this->supplier_collection_ =
                (synch_type << 8) | (collection_type << 4) | iteration_type;
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

      else if (ACE_OS::strcasecmp (arg, "-ECPushSupplierSet") == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "obsolete option <%s>, ignored\n", arg));
          arg_shifter.consume_arg ();
        }

      else if (ACE_OS::strcasecmp (arg, "-ECConsumerAdminLock") == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "obsolete option <%s>, ignored\n", arg));
          arg_shifter.consume_arg ();
        }

      else if (ACE_OS::strcasecmp (arg, "-ECSupplierAdminLock") == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "obsolete option <%s>, ignored\n", arg));
          arg_shifter.consume_arg ();
        }

      else if (ACE_OS::strncmp (arg, "-EC", 3) == 0)
        {
          arg_shifter.consume_arg ();
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "unknown option <%s>\n",
                      arg));
        }
      else
        {
          arg_shifter.consume_arg ();
          ACE_DEBUG ((LM_DEBUG,
                      "EC_Default_Factory - "
                      "ignoring option <%s>\n",
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
TAO_EC_Default_Factory::create_dispatching (TAO_EC_Event_Channel *)
{
  if (this->dispatching_ == 0)
    return new TAO_EC_Reactive_Dispatching ();
  else if (this->dispatching_ == 1)
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
    return new TAO_EC_Prefix_Filter_Builder (ec);
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
      ACE_NEW_RETURN (lock, ACE_Lock_Adapter<TAO_SYNCH_MUTEX>, 0);
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
TAO_EC_Default_Factory::create_scheduling_strategy (TAO_EC_Event_Channel*)
{
  if (this->scheduling_ == 0)
    return new TAO_EC_Null_Scheduling;
  return 0;
}

void
TAO_EC_Default_Factory::destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy* x)
{
  delete x;
}

// These typedefs workaround a SunCC 4.2 bug
typedef
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>::Iterator
    TAO_EC_Consumer_List_Iterator;
typedef
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>::Iterator
    TAO_EC_Consumer_RB_Tree_Iterator;
typedef
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>::Iterator
    TAO_EC_Supplier_List_Iterator;
typedef
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>::Iterator
    TAO_EC_Supplier_RB_Tree_Iterator;


TAO_EC_ProxyPushConsumer_Collection*
TAO_EC_Default_Factory::create_proxy_push_consumer_collection (TAO_EC_Event_Channel *)
{
  if (this->consumer_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->consumer_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->consumer_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->consumer_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->consumer_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}

void
TAO_EC_Default_Factory::destroy_proxy_push_consumer_collection (TAO_EC_ProxyPushConsumer_Collection *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier_Collection*
TAO_EC_Default_Factory::create_proxy_push_supplier_collection (TAO_EC_Event_Channel *)
{
  if (this->supplier_collection_ == 0x000)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x001)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x002)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x003)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x010)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x011)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX> ();
  else if (this->supplier_collection_ == 0x012)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x013)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH> ();
  else if (this->supplier_collection_ == 0x100)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x101)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x102)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x103)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x110)
    return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x111)
    return new TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_Null_Mutex> ();
  else if (this->supplier_collection_ == 0x112)
    return new TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();
  else if (this->supplier_collection_ == 0x113)
    return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> ();

  return 0;
}

void
TAO_EC_Default_Factory::destroy_proxy_push_supplier_collection (TAO_EC_ProxyPushSupplier_Collection *x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Default_Factory::create_consumer_lock (void)
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
    return new ACE_Lock_Adapter<TAO_SYNCH_MUTEX> ();
  else if (this->supplier_lock_ == 2)
    return new ACE_Lock_Adapter<TAO_SYNCH_RECURSIVE_MUTEX> ();
  return 0;
}

void
TAO_EC_Default_Factory::destroy_supplier_lock (ACE_Lock* x)
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

      ACE_Time_Value rate (0, this->supplier_control_period_);
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
                       ACE_TEXT ("EC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Default_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTEvent, TAO_EC_Default_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_Command_Base*>;
template class ACE_Unbounded_Queue<ACE_Command_Base*>;
template class ACE_Unbounded_Queue_Iterator<ACE_Command_Base*>;

template class TAO_ESF_Proxy_Collection<TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>;
template class ACE_Unbounded_Set<TAO_EC_ProxyPushConsumer *>;
template class ACE_Node<TAO_EC_ProxyPushConsumer *>;
template class ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushConsumer *>;
template class ACE_RB_Tree<TAO_EC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_EC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<TAO_EC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_EC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<TAO_EC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_EC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<TAO_EC_ProxyPushConsumer *, int, ACE_Less_Than<TAO_EC_ProxyPushConsumer *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<TAO_EC_ProxyPushConsumer *, int>;
template class ACE_Less_Than<TAO_EC_ProxyPushConsumer *>;
template class TAO_ESF_Proxy_RB_Tree_Iterator<TAO_EC_ProxyPushConsumer>;

template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_List_Iterator >;
template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_List_Iterator >;
template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_RB_Tree_Iterator >;
template class TAO_ESF_Copy_On_Write_Collection<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_RB_Tree_Iterator >;

template class TAO_ESF_Proxy_Collection<TAO_EC_ProxyPushSupplier>;
template class TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>;
template class TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>;
template class ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier *>;
template class ACE_Node<TAO_EC_ProxyPushSupplier *>;
template class ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier *>;
template class ACE_RB_Tree<TAO_EC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_EC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<TAO_EC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_EC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<TAO_EC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_EC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<TAO_EC_ProxyPushSupplier *, int, ACE_Less_Than<TAO_EC_ProxyPushSupplier *>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<TAO_EC_ProxyPushSupplier *, int>;
template class ACE_Less_Than<TAO_EC_ProxyPushSupplier *>;
template class TAO_ESF_Proxy_RB_Tree_Iterator<TAO_EC_ProxyPushSupplier>;

#if defined (ACE_HAS_THREADS)
//
// To avoid duplicate instantiations of templates we must put the MT
// versions on this #ifdef, otherwise the ACE_SYNCH* macros expand to
// the ACE_NULL* versions, duplicating the non-MT versions below.
// We *cannot* use explicit ACE_Synch classes because that will not
// compile in platforms without threads.
//
template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
     TAO_EC_Consumer_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_SYNCH> >;

template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_SYNCH> >;

template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_List_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_List_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_RB_Tree_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_RB_Tree_Iterator,
        TAO_SYNCH_MUTEX>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_List_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_List_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_RB_Tree_Iterator,
        ACE_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_RB_Tree_Iterator,
        ACE_SYNCH>;

#endif /* ACE_HAS_THREADS */

template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_NULL_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushConsumer >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;

template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_NULL_SYNCH> >;
template class TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Read<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>;
template class TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;
template class ACE_Guard< TAO_ESF_Busy_Lock_Adapter<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> > >;
template class TAO_ESF_Connected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Reconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Disconnected_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH>,TAO_EC_ProxyPushSupplier >;
template class TAO_ESF_Shutdown_Command<
  TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_RB_Tree_Iterator,
      ACE_NULL_SYNCH> >;

template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_List_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_List_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_RB_Tree_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Read_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_RB_Tree_Iterator,
        ACE_Null_Mutex>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_List_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_List_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushConsumer>,
    TAO_EC_Consumer_RB_Tree_Iterator,
        ACE_NULL_SYNCH>;
template class TAO_ESF_Copy_On_Write_Write_Guard<
    TAO_ESF_Proxy_RB_Tree<TAO_EC_ProxyPushSupplier>,
    TAO_EC_Supplier_RB_Tree_Iterator,
        ACE_NULL_SYNCH>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// @@ TODO!!!

#if defined (ACE_HAS_THREADS)
#endif /* ACE_HAS_THREADS */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
