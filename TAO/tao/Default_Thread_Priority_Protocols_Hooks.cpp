// $Id$

#include "tao/Default_Thread_Priority_Protocols_Hooks.h"
#include "ace/Log_Msg.h"
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
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"
#include "tao/Client_Network_Priority_Policy.h"

ACE_RCSID (tao,
           Default_Thread_Priority_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Thread_Priority_Protocols_Hooks::
~TAO_Default_Thread_Priority_Protocols_Hooks (void)
{
}

int
TAO_Default_Thread_Priority_Protocols_Hooks::get_thread_CORBA_priority (
  CORBA::Short &
  ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Thread_Priority_Protocols_Hooks::get_thread_native_priority (
  CORBA::Short &
  ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Thread_Priority_Protocols_Hooks::
get_thread_CORBA_and_native_priority (CORBA::Short &,
                                      CORBA::Short &
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Thread_Priority_Protocols_Hooks::set_thread_CORBA_priority (
  CORBA::Short
  ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
TAO_Default_Thread_Priority_Protocols_Hooks::set_thread_native_priority (
  CORBA::Short
  ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Thread_Priority_Protocols_Hooks,
                       ACE_TEXT ("Thread_Priority_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (
                          TAO_Default_Thread_Priority_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Thread_Priority_Protocols_Hooks)
