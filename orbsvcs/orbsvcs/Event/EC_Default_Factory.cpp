// $Id$

#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Reactive_Dispatching.h"
#include "orbsvcs/Event/EC_MT_Dispatching.h"
#include "orbsvcs/Event/EC_Basic_Filter_Builder.h"
#include "orbsvcs/Event/EC_Prefix_Filter_Builder.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/Event/EC_Default_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Default_ProxySupplier.h"
#include "orbsvcs/Event/EC_Trivial_Supplier_Filter.h"
#include "orbsvcs/Event/EC_Per_Supplier_Filter.h"
#include "orbsvcs/Event/EC_ObserverStrategy.h"
#include "orbsvcs/Event/EC_Null_Scheduling.h"
#include "orbsvcs/Event/EC_Group_Scheduling.h"
#include "orbsvcs/Event/EC_Reactive_Timeout_Generator.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_Reactive_ConsumerControl.h"
#include "orbsvcs/Event/EC_Reactive_SupplierControl.h"
#include "orbsvcs/Event/EC_Thread_Flags.h"

#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.h"
#include "orbsvcs/ESF/ESF_Immediate_Changes.h"
#include "orbsvcs/ESF/ESF_Copy_On_Read.h"
#include "orbsvcs/ESF/ESF_Copy_On_Write.h"
#include "orbsvcs/ESF/ESF_Delayed_Changes.h"
#include "orbsvcs/ESF/ESF_Delayed_Command.h"

#include "tao/ORB_Core.h"

#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_strings.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Default_Factory.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Default_Factory::~TAO_EC_Default_Factory (void)
{
}

int
TAO_EC_Default_Factory::init_svcs (void)
{
  TAO_EC_Simple_Queue_Full_Action::init_svcs();
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_EC_Default_Factory);
}

void
TAO_EC_Default_Factory::unsupported_option_value (const ACE_TCHAR * option_name,
                                                  const ACE_TCHAR * option_value)
{
  ACE_ERROR ((LM_ERROR,
              "EC_Default_Factory - "
              "Unsupported <%s> option value: <%s>. "
              "Ignoring this option - using defaults instead.\n",
              option_name,
              option_value));
}

int
TAO_EC_Default_Factory::init (int argc, ACE_TCHAR* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO) +
     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  this->dispatching_threads_priority_ =
    ACE_Sched_Params::next_priority (ACE_SCHED_FIFO, priority);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECDispatching")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                this->dispatching_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("mt")) == 0)
                this->dispatching_ = 1;
              else
                  this->unsupported_option_value (ACE_TEXT("-ECDispatching"), opt);
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECDispatchingThreadFlags")) == 0)
        {
          arg_shifter.consume_arg ();

          // Need to be in the form of <flags>:<priority>
          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* s = arg_shifter.get_current ();
              // need to parse the flags...ugh
              ACE_TCHAR* opt = ACE_OS::strdup (s);

              ACE_TCHAR* aux = 0;
              ACE_TCHAR* flags = ACE_OS::strtok_r (opt, ACE_TEXT (":"), &aux);

              TAO_EC_Thread_Flags tf(ACE_TEXT_ALWAYS_CHAR (flags)); // parse and set up
              this->dispatching_threads_flags_ = tf.flags ();

              ACE_TCHAR* arg = ACE_OS::strtok_r (0, ACE_TEXT(":"), &aux);
              if (arg)
                {
                  long prio = ACE_OS::strtol (arg, 0, 0);

                  this->dispatching_threads_priority_ = prio;
                }
              else
                {
                  // Should we set the default priority?
                  this->dispatching_threads_priority_ = tf.default_priority ();
                }
              ACE_OS::free (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECDispatchingThreads")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->dispatching_threads_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECFiltering")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->filtering_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("basic")) == 0)
                this->filtering_ = 1;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("prefix")) == 0)
                this->filtering_ = 2;
              else
                  this->unsupported_option_value (ACE_TEXT("-ECFiltering"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECSupplierFilter")) == 0
               // @@ TODO remove, only for backwards compatibility
               || ACE_OS::strcasecmp (arg, ACE_TEXT("-ECSupplierFiltering")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->supplier_filtering_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("per-supplier")) == 0)
                this->supplier_filtering_ = 1;
              else
                  this->unsupported_option_value (ACE_TEXT("-ECSupplierFilter"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                this->timeout_ = 0;
              else
                  this->unsupported_option_value (ACE_TEXT("-ECTimeout"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECObserver")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->observer_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("basic")) == 0)
                this->observer_ = 1;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                this->observer_ = 2;
              else
                  this->unsupported_option_value (ACE_TEXT("-ECObserver"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECScheduling")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->scheduling_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("group")) == 0)
                this->scheduling_ = 1;
              else
                  this->unsupported_option_value (ACE_TEXT("-ECScheduling"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECProxyPushConsumerCollection")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR *current_arg = arg_shifter.get_current ();
              ACE_TCHAR *opt = ACE_OS::strdup (current_arg);
              int collection_type = 0;
              int synch_type = 0;
              int iteration_type = 0;

              ACE_TCHAR* aux;
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
                    ACE_ERROR ((LM_ERROR,
                                "EC_Default_Factory - "
                                "Unknown consumer collection modifier <%s>.\n", arg));
                }
              ACE_OS::free (opt);
              this->consumer_collection_ =
                 (synch_type << 8) |(collection_type << 4) | iteration_type;
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECProxyPushSupplierCollection")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* current_arg = arg_shifter.get_current();
              ACE_TCHAR* opt = ACE_OS::strdup(current_arg);
              int collection_type = 0;
              int synch_type = 0;
              int iteration_type = 0;

              ACE_TCHAR* aux;
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
                    ACE_ERROR ((LM_ERROR,
                                "EC_Default_Factory - "
                                "Unknown supplier collection modifier <%s>.\n", arg));
                }
              ACE_OS::free(opt);
              this->supplier_collection_ =
                (synch_type << 8) | (collection_type << 4) | iteration_type;
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECProxyConsumerLock")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->consumer_lock_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("thread")) == 0)
                this->consumer_lock_ = 1;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("recursive")) == 0)
                this->consumer_lock_ = 2;
              else
                this->unsupported_option_value (ACE_TEXT("-ECProxyConsumerLock"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECProxySupplierLock")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->supplier_lock_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("thread")) == 0)
                this->supplier_lock_ = 1;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("recursive")) == 0)
                this->supplier_lock_ = 2;
              else
                this->unsupported_option_value (ACE_TEXT("-ECProxySupplierLock"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECUseORBId")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              // Save argument for later use
              this->orbid_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECConsumerControl")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->consumer_control_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                this->consumer_control_ = 1;
              else
                this->unsupported_option_value (ACE_TEXT("-ECConsumerControl"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECSupplierControl")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->supplier_control_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                this->supplier_control_ = 1;
              else
                this->unsupported_option_value (ACE_TEXT("-ECSupplierControl"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECConsumerControlPeriod")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->consumer_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECSupplierControlPeriod")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->supplier_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECConsumerControlTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              unsigned long timeout = ACE_OS::strtoul(opt, 0, 10);
              this->consumer_control_timeout_.usec(timeout);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECSupplierControlTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              unsigned long timeout = ACE_OS::strtoul(opt, 0, 10);
              this->supplier_control_timeout_.usec(timeout);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECConsumerValidateConnection")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->consumer_validate_connection_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECPushSupplierSet")) == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "obsolete option <%s>, ignored\n", arg));
          arg_shifter.consume_arg ();
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECQueueFullServiceObject")) == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->queue_full_service_object_name_.set(opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECConsumerAdminLock")) == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "obsolete option <%s>, ignored\n", arg));
          arg_shifter.consume_arg ();
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECSupplierAdminLock")) == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Default_Factory - "
                      "obsolete option <%s>, ignored\n", arg));
          arg_shifter.consume_arg ();
        }

      else if (ACE_OS::strncmp (arg, ACE_TEXT("-EC"), 3) == 0)
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

TAO_EC_Queue_Full_Service_Object*
TAO_EC_Default_Factory::find_service_object (const ACE_TCHAR* wanted,
                                             const ACE_TCHAR* fallback)
{
  TAO_EC_Queue_Full_Service_Object* so = 0;
  so = ACE_Dynamic_Service<TAO_EC_Queue_Full_Service_Object>::instance (wanted);
  if (so != 0)
    return so;

  ACE_ERROR ((LM_ERROR,
              "EC (%P|%t) EC_Default_Factory::find_service_object "
              "unable to find queue full service object '%s'; "
              "using '%s' instead\n",
              wanted,
              fallback));

  so = ACE_Dynamic_Service<TAO_EC_Queue_Full_Service_Object>::instance (fallback);
  if (so != 0)
    return so;

  ACE_ERROR ((LM_ERROR,
              "EC (%P|%t) EC_Default_Factory::find_service_object "
                      "unable find default queue full service object '%s'; "
                      "aborting.\n",
                      fallback));
  ACE_OS::abort ();
  return 0; // superfluous return to de-warn; we should never reach here
}

TAO_EC_Dispatching*
TAO_EC_Default_Factory::create_dispatching (TAO_EC_Event_Channel_Base *)
{
  if (this->dispatching_ == 0)
    return new TAO_EC_Reactive_Dispatching ();
  else if (this->dispatching_ == 1)
    {
      TAO_EC_Queue_Full_Service_Object* so =
        this->find_service_object (this->queue_full_service_object_name_.fast_rep(),
                                   TAO_EC_DEFAULT_QUEUE_FULL_SERVICE_OBJECT_NAME);
      return new TAO_EC_MT_Dispatching (this->dispatching_threads_,
                                        this->dispatching_threads_flags_,
                                        this->dispatching_threads_priority_,
                                        this->dispatching_threads_force_active_,
                                        so);
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_dispatching (TAO_EC_Dispatching *x)
{
  delete x;
}

TAO_EC_Filter_Builder*
TAO_EC_Default_Factory::create_filter_builder (TAO_EC_Event_Channel_Base *ec)
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
TAO_EC_Default_Factory::create_supplier_filter_builder (TAO_EC_Event_Channel_Base *ec)
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
TAO_EC_Default_Factory::create_consumer_admin (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_ConsumerAdmin (ec);
}

void
TAO_EC_Default_Factory::destroy_consumer_admin (TAO_EC_ConsumerAdmin *x)
{
  delete x;
}

TAO_EC_SupplierAdmin*
TAO_EC_Default_Factory::create_supplier_admin (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_SupplierAdmin (ec);
}

void
TAO_EC_Default_Factory::destroy_supplier_admin (TAO_EC_SupplierAdmin *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier*
TAO_EC_Default_Factory::create_proxy_push_supplier (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_Default_ProxyPushSupplier (ec, consumer_validate_connection_);
}

void
TAO_EC_Default_Factory::destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier *x)
{
  delete x;
}

TAO_EC_ProxyPushConsumer*
TAO_EC_Default_Factory::create_proxy_push_consumer (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_Default_ProxyPushConsumer (ec);
}

void
TAO_EC_Default_Factory::destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer *x)
{
  delete x;
}

TAO_EC_Timeout_Generator*
TAO_EC_Default_Factory::create_timeout_generator (TAO_EC_Event_Channel_Base *)
{
  if (this->timeout_ == 0)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_.c_str ());

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
TAO_EC_Default_Factory::create_observer_strategy (TAO_EC_Event_Channel_Base *ec)
{
  if (this->observer_ == 0)
    return new TAO_EC_Null_ObserverStrategy;
  else if (this->observer_ == 1)
    {
      // @@ The lock should also be under control of the user...
      ACE_Lock* lock = 0;
      ACE_NEW_RETURN (lock, ACE_Lock_Adapter<TAO_SYNCH_MUTEX>, 0);
      return new TAO_EC_Basic_ObserverStrategy (ec, lock);
    }
  else if (this->observer_ == 2)
    {
      // @@ The lock should also be under control of the user...
      ACE_Lock* lock = 0;
      ACE_NEW_RETURN (lock, ACE_Lock_Adapter<TAO_SYNCH_MUTEX>, 0);
      return new TAO_EC_Reactive_ObserverStrategy (ec, lock);
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_observer_strategy (TAO_EC_ObserverStrategy *x)
{
  delete x;
}

TAO_EC_Scheduling_Strategy*
TAO_EC_Default_Factory::create_scheduling_strategy (TAO_EC_Event_Channel_Base*)
{
  if (this->scheduling_ == 0)
    return new TAO_EC_Null_Scheduling;
  else if (this->scheduling_ == 1)
    return new TAO_EC_Group_Scheduling;
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
TAO_EC_Default_Factory::create_proxy_push_consumer_collection (TAO_EC_Event_Channel_Base *)
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
TAO_EC_Default_Factory::create_proxy_push_supplier_collection (TAO_EC_Event_Channel_Base *)
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
TAO_EC_Default_Factory::create_consumer_control (TAO_EC_Event_Channel_Base* ec)
{
  if (this->consumer_control_ == 0)
    return new TAO_EC_ConsumerControl ();
  else if (this->consumer_control_ == 1)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_.c_str ());

      ACE_Time_Value rate (0, this->consumer_control_period_);
      return new TAO_EC_Reactive_ConsumerControl (rate, consumer_control_timeout_, ec, orb.in ());
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_consumer_control (TAO_EC_ConsumerControl* x)
{
  delete x;
}

TAO_EC_SupplierControl*
TAO_EC_Default_Factory::create_supplier_control (TAO_EC_Event_Channel_Base* ec)
{
  if (this->supplier_control_ == 0)
    return new TAO_EC_SupplierControl ();
  else if (this->supplier_control_ == 1)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_.c_str ());

      ACE_Time_Value rate (0, this->supplier_control_period_);
      return new TAO_EC_Reactive_SupplierControl (rate, supplier_control_timeout_, ec, orb.in ());
    }
  return 0;
}

void
TAO_EC_Default_Factory::destroy_supplier_control (TAO_EC_SupplierControl* x)
{
  delete x;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_EC_Default_Factory,
                       ACE_TEXT ("EC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Default_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTEvent_Serv, TAO_EC_Default_Factory)
