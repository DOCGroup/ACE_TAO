// $Id$

#include "EC_Default_Factory.h"
#include "EC_Priority_Dispatching.h"
#include "EC_Basic_Filter_Builder.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_Trivial_Supplier_Filter.h"
#include "EC_Per_Supplier_Filter.h"
#include "EC_ObserverStrategy.h"
#include "EC_ProxyPushSupplier_Set_T.h"
#include "EC_Reactive_Timeout_Generator.h"
#include "ace/Arg_Shifter.h"

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

  while (arg_shifter.is_anything_left ())
    {
      char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, "-ECdispatching") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECfiltering") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECsupplierfiltering") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECtimeout") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECobserver") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECpushsupplierset") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECproxyconsumerlock") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECproxysupplierlock") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECconsumeradminlock") == 0)
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

      else if (ACE_OS::strcmp (arg, "-ECsupplieradminlock") == 0)
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
TAO_EC_Default_Factory::create_dispatching (TAO_EC_Event_Channel *)
{
  if (this->dispatching_ == 0)
    return new TAO_EC_Reactive_Dispatching ();
  else if (this->dispatching_ == 1)
    return new TAO_EC_Priority_Dispatching ();
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
TAO_EC_Default_Factory::create_timeout_generator (TAO_EC_Event_Channel *ec)
{
  if (this->timeout_ == 0)
    {
      // @@ TODO fixme
      TAO_ORB_Core* orb_core = TAO_ORB_Core_instance ();
      return new TAO_EC_Reactive_Timeout_Generator (orb_core->reactor ());
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

template class TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_SYNCH_MUTEX>;
template class TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_Null_Mutex>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_SYNCH_MUTEX>
#pragma instantiate TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
