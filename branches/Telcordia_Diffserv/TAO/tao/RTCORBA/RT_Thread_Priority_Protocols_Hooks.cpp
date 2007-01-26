#include "tao/RTCORBA/RT_Thread_Priority_Protocols_Hooks.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/RT_Stub.h"

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/MProfile.h"
#include "tao/Service_Context.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Protocol_Factory.h"
#include "tao/Transport_Acceptor.h"
#include "tao/Transport_Connector.h"
#include "tao/Policy_Set.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/Client_Network_Priority_Policy.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (RTCORBA,
           RT_Thread_Priority_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Thread_Priority_Protocols_Hooks::
TAO_RT_Thread_Priority_Protocols_Hooks (void)
  : orb_core_ (0)
  , current_ ()
{
}


TAO_RT_Thread_Priority_Protocols_Hooks::
~TAO_RT_Thread_Priority_Protocols_Hooks (void)
{
}

void
TAO_RT_Thread_Priority_Protocols_Hooks::init_hooks (TAO_ORB_Core *orb_core
                                    ACE_ENV_ARG_DECL)
{
  this->orb_core_ = orb_core;

  // Save a reference to the priority mapping manager.
  CORBA::Object_var obj =
    orb_core->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_PRIORITYMAPPINGMANAGER);

  this->mapping_manager_ =
    TAO_Priority_Mapping_Manager::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Save a reference to the priority mapping manager.
  CORBA::Object_var object =
    orb_core->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_NETWORKPRIORITYMAPPINGMANAGER);

  this->network_mapping_manager_ =
    TAO_Network_Priority_Mapping_Manager::_narrow (object.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  object =
    this->orb_core_->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_RTCURRENT);

  this->current_ =
    RTCORBA::Current::_narrow (object.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

int
TAO_RT_Thread_Priority_Protocols_Hooks::get_thread_CORBA_priority (
  CORBA::Short &priority ACE_ENV_ARG_DECL)
{
  CORBA::Short native_priority = 0;
  int const result =
    this->get_thread_CORBA_and_native_priority (priority,
                                                native_priority
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == -1)
    {
      return result;
    }

  return 0;
}

int
TAO_RT_Thread_Priority_Protocols_Hooks::get_thread_native_priority (
    CORBA::Short &native_priority
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;

  if (ACE_Thread::getprio (current, priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("RT_Protocols_Hooks::get_thread_priority: ")
                  ACE_TEXT (" ACE_Thread::get_prio\n")));

      return -1;
    }

  native_priority = CORBA::Short (priority);
  return 0;
}

int
TAO_RT_Thread_Priority_Protocols_Hooks::get_thread_CORBA_and_native_priority (
    CORBA::Short &priority,
    CORBA::Short &native_priority
    ACE_ENV_ARG_DECL
  )
{
  int result =
    this->get_thread_native_priority (native_priority
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == -1)
    {
      return result;
    }

  TAO_Priority_Mapping *priority_mapping =
    this->mapping_manager_.in ()->mapping ();

  if (priority_mapping->to_CORBA (native_priority, priority) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("RT_Protocols_Hooks::get_thread_priority: ")
                  ACE_TEXT ("Priority_Mapping::to_CORBA\n")));
      return -1;
    }

  return 0;
}

int
TAO_RT_Thread_Priority_Protocols_Hooks::set_thread_CORBA_priority (
  CORBA::Short priority ACE_ENV_ARG_DECL)
{
  TAO_Priority_Mapping *priority_mapping =
    this->mapping_manager_.in ()->mapping ();

  CORBA::Short native_priority;

  if (priority_mapping->to_native (priority, native_priority) == 0)
    {
      return -1;
    }

  return this->set_thread_native_priority (native_priority
                                           ACE_ENV_ARG_PARAMETER);
}

int
TAO_RT_Thread_Priority_Protocols_Hooks::set_thread_native_priority (
    CORBA::Short native_priority
    ACE_ENV_ARG_DECL_NOT_USED
   )
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  if (ACE_Thread::setprio (current, native_priority) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N,%l) Error setting thread ")
                         ACE_TEXT ("priority to %d, errno %d %m\n"),
                         native_priority,
                         errno ),
                       -1);
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_RT_Thread_Priority_Protocols_Hooks,
                       ACE_TEXT ("RT_Thread_Priority_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Thread_Priority_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, TAO_RT_Thread_Priority_Protocols_Hooks)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
