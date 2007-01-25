// $Id$

#include "tao/Default_Network_Priority_Protocols_Hooks.h"
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
           Default_Network_Priority_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Network_Priority_Protocols_Hooks::
~TAO_Default_Network_Priority_Protocols_Hooks (void)
{
}

void
TAO_Default_Network_Priority_Protocols_Hooks::add_rep_np_service_context_hook (
   TAO_Service_Context &,
   CORBA::Long &
   ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Network_Priority_Protocols_Hooks::np_service_context (
    TAO_Stub *,
    TAO_Service_Context &,
    CORBA::Boolean
    ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Network_Priority_Protocols_Hooks::set_dscp_codepoint (
   CORBA::Long &
   ACE_ENV_ARG_DECL_NOT_USED)
{
}

CORBA::Long
TAO_Default_Network_Priority_Protocols_Hooks::get_dscp_codepoint (void)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Network_Priority_Protocols_Hooks,
                       ACE_TEXT ("Network_Priority_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (
                         TAO_Default_Network_Priority_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Network_Priority_Protocols_Hooks)
