// $Id$

#include "tao/Default_Protocols_Hooks.h"
#include "ace/Log_Msg.h"

ACE_RCSID (tao,
           Default_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Protocols_Hooks::~TAO_Default_Protocols_Hooks (void)
{
}

void
TAO_Default_Protocols_Hooks::init_hooks (TAO_ORB_Core *
                                         )
{
}

void
TAO_Default_Protocols_Hooks::add_rt_service_context_hook (TAO_Service_Context &,
                                                          CORBA::Policy *,
                                                          CORBA::Short &
                                                          )
{
}

void
TAO_Default_Protocols_Hooks::get_selector_hook (CORBA::Policy *,
                                                CORBA::Boolean &,
                                                CORBA::Short &)
{
}

void
TAO_Default_Protocols_Hooks::get_selector_bands_policy_hook (CORBA::Policy *,
                                                             CORBA::Short,
                                                             CORBA::Short &,
                                                             CORBA::Short &,
                                                             bool &)
{
}

int
TAO_Default_Protocols_Hooks::get_thread_CORBA_priority (CORBA::Short &)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::get_thread_native_priority (CORBA::Short &)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::get_thread_CORBA_and_native_priority (CORBA::Short &,
                                                                   CORBA::Short &
                                                                   )
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::set_thread_CORBA_priority (CORBA::Short)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::set_thread_native_priority (CORBA::Short)
{
  return 0;
}

void
TAO_Default_Protocols_Hooks::rt_service_context (TAO_Stub *,
                                                 TAO_Service_Context &,
                                                 CORBA::Boolean
                                                 )
{
}

CORBA::Boolean
TAO_Default_Protocols_Hooks::set_client_network_priority (IOP::ProfileId,
                                                          TAO_Stub *)
{
  return false;
}

CORBA::Boolean
TAO_Default_Protocols_Hooks::set_server_network_priority (IOP::ProfileId,
                                                          CORBA::Policy *
                                                          )
{
  return false;
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &
                                                                      )
{
}

void
TAO_Default_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &
                                                                      )
{
}

CORBA::Long
TAO_Default_Protocols_Hooks::get_dscp_codepoint (void)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Protocols_Hooks,
                       ACE_TEXT ("Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Protocols_Hooks)
