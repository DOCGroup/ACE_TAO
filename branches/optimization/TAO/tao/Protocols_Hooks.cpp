// $Id$

#include "Protocols_Hooks.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, Protocols_Hooks, "$Id$")

/// constructor
TAO_Protocols_Hooks::TAO_Protocols_Hooks (void)
{
}

/// destructor
TAO_Protocols_Hooks::~TAO_Protocols_Hooks (void)
{
}

#if (TAO_HAS_RT_CORBA == 1)

int
TAO_Protocols_Hooks::call_client_protocols_hook (TAO_ORB_Core *,
                                                 RTCORBA::ProtocolProperties_var &,
                                                 const char *)
{
  return 0;
}


int
TAO_Protocols_Hooks::call_server_protocols_hook (TAO_ORB_Core *,
                                                 RTCORBA::ProtocolProperties_var &,
                                                 const char *)
{
  return 0;
}

#endif /* TAO_HAS_RT_CORBA == 1 */

void
TAO_Protocols_Hooks::call_policy_type_hook (CORBA::PolicyList *&,
                                            CORBA::ULong &)
{
  return;
}

void
TAO_Protocols_Hooks::validate_policy_type (CORBA::ULong,
                                           CORBA::ULong &,
                                           CORBA::Environment &)
{
  return;
}

void
TAO_Protocols_Hooks::add_rt_service_context_hook (TAO_GIOP_Invocation *,
                                                  CORBA::Policy *,
                                                  CORBA::Short &,
                                                  CORBA::Environment
                                                  &)
{
  return;
}

void
TAO_Protocols_Hooks::get_selector_hook (CORBA::Policy *,
                                        CORBA::Boolean &,
                                        CORBA::Short &)
{
  return;
}

void
TAO_Protocols_Hooks:: get_selector_bands_policy_hook (
                                  CORBA::Policy *,
                                  CORBA::Short &,
                                  CORBA::Short &,
                                  CORBA::Short &,
                                  int &)
{
  return;
}

CORBA::Policy *
TAO_Protocols_Hooks::effective_priority_banded_connection_hook (CORBA::Policy *,
                                                                CORBA::Policy *,
                                                                CORBA::Environment &)
{
  return CORBA::Policy::_nil ();
}

CORBA::Policy *
TAO_Protocols_Hooks::effective_client_protocol_hook (CORBA::Policy *,
                                                     CORBA::Policy *,
                                                     CORBA::Environment &)
{
   return CORBA::Policy::_nil ();
}

int
TAO_Protocols_Hooks::get_thread_priority (TAO_ORB_Core *,
                                          CORBA::Short &,
                                          CORBA::Environment &)
{
  return 0;
}

int
TAO_Protocols_Hooks::set_thread_priority (TAO_ORB_Core *,
                                          CORBA::Short,
                                          CORBA::Environment &)
{
  return 0;
}

void
TAO_Protocols_Hooks::set_priority_mapping (TAO_ORB_Core *,
                                           TAO_Resource_Factory *,
                                           CORBA::Environment &)
{
  return;
}

int 
TAO_Protocols_Hooks::set_default_policies (TAO_ORB_Core *)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Protocols_Hooks>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Protocols_Hooks>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
