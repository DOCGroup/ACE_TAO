// $Id$

#include "tao/Default_Network_Priority_Protocols_Hooks.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Service_Context.h"

ACE_RCSID (tao,
           Default_Network_Priority_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Network_Priority_Protocols_Hooks::
~TAO_Default_Network_Priority_Protocols_Hooks (void)
{
}

void
TAO_Default_Network_Priority_Protocols_Hooks::init_hooks (TAO_ORB_Core *)
{
}


void
TAO_Default_Network_Priority_Protocols_Hooks::add_rep_np_service_context_hook (
   TAO_Service_Context &,
   CORBA::Long &)
{
}

void
TAO_Default_Network_Priority_Protocols_Hooks::np_service_context (
    TAO_Stub *,
    TAO_Service_Context &,
    CORBA::Boolean)
{
}

CORBA::Long
TAO_Default_Network_Priority_Protocols_Hooks::get_dscp_codepoint (
  TAO_Service_Context &)
{
  return 0;
}

CORBA::Long
TAO_Default_Network_Priority_Protocols_Hooks::get_dscp_codepoint (
  TAO_Stub *, CORBA::Object *)
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
