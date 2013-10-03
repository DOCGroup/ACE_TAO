// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/EC_TPC_Factory.h"

#include "orbsvcs/Event/EC_TPC_Dispatching.h"
#include "orbsvcs/Event/EC_TPC_ProxySupplier.h"
#include "orbsvcs/Event/EC_TPC_ProxyConsumer.h"

#include "tao/ORB_Core.h"

#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_strings.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

unsigned long TAO_EC_TPC_debug_level;

TAO_EC_TPC_Factory::TAO_EC_TPC_Factory (void)
{
  TAO_EC_TPC_debug_level = 0;
}

TAO_EC_TPC_Factory::~TAO_EC_TPC_Factory (void)
{
}

int
TAO_EC_TPC_Factory::init_svcs (void)
{
  TAO_EC_Simple_Queue_Full_Action::init_svcs();
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_EC_TPC_Factory);
}

int
TAO_EC_TPC_Factory::init (int argc, ACE_TCHAR* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECDispatching")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              // Here we print out a message indicating that this
              // option isn't valid with this factory and that it's
              // being ignored.

              ORBSVCS_DEBUG ((LM_ERROR,
                            "EC_TPC_Factory - "
                          "-ECDispatching not supported with TPC_Factory; ignoring the option and using thread-per-consumer dispatch strategy\n"));
              arg_shifter.consume_arg ();
            }
        }
      if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECTPCDebug")) == 0)
        {
          arg_shifter.consume_arg ();
          ++TAO_EC_TPC_debug_level;
        }
      else
        {
          arg_shifter.ignore_arg ();
        }

    }

  return TAO_EC_Default_Factory::init (argc, argv);
}

// ****************************************************************

TAO_EC_Dispatching*
TAO_EC_TPC_Factory::create_dispatching (TAO_EC_Event_Channel_Base *)
{
  if (TAO_EC_TPC_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "EC (%P|%t) EC_TPC_Factory::create_dispatching\n"));

  TAO_EC_Queue_Full_Service_Object* so =
    this->find_service_object (this->queue_full_service_object_name_.fast_rep(),
                               TAO_EC_DEFAULT_QUEUE_FULL_SERVICE_OBJECT_NAME);

  return new TAO_EC_TPC_Dispatching (this->dispatching_threads_,
                                     this->dispatching_threads_flags_,
                                     this->dispatching_threads_priority_,
                                     this->dispatching_threads_force_active_,
                                     so);
}


TAO_EC_ProxyPushSupplier*
TAO_EC_TPC_Factory::create_proxy_push_supplier (TAO_EC_Event_Channel_Base *ec)
{
  if (TAO_EC_TPC_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "EC (%P|%t) EC_TPC_Factory::create_proxy_push_supplier\n"));
  return new TAO_EC_TPC_ProxyPushSupplier (ec, this->consumer_validate_connection_);
}


TAO_EC_ProxyPushConsumer*
TAO_EC_TPC_Factory::create_proxy_push_consumer (TAO_EC_Event_Channel_Base *ec)
{
  if (TAO_EC_TPC_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "EC (%P|%t) EC_TPC_Factory::create_proxy_push_consumer\n"));
  return new TAO_EC_TPC_ProxyPushConsumer (ec);
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_EC_TPC_Factory,
                       ACE_TEXT ("EC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_TPC_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTEvent_Serv, TAO_EC_TPC_Factory)

// ****************************************************************

