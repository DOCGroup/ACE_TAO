// $Id$

#include "Default_Protocols_Hooks.h"
#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "Default_Protocols_Hooks.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Default_Protocols_Hooks, "$Id$");

/// constructor
TAO_Default_Protocols_Hooks::TAO_Default_Protocols_Hooks (void)
{
}

/// destructor
TAO_Default_Protocols_Hooks::~TAO_Default_Protocols_Hooks (void)
{
}

int
TAO_Default_Protocols_Hooks::call_client_protocols_hook (
                                TAO_ORB_Core *,
                                RTCORBA::ProtocolProperties_var &,
                                const char *)
{
  return 0;
}

int
TAO_Default_Protocols_Hooks::call_server_protocols_hook (
                                TAO_ORB_Core *,
                                RTCORBA::ProtocolProperties_var &,
                                const char *)
{
  return 0;
}

void
TAO_Default_Protocols_Hooks::call_policy_type_hook (
                                 CORBA::PolicyList *&,
                                 CORBA::ULong &)
{
  return;
}

void
TAO_Default_Protocols_Hooks::validate_policy_type (CORBA::ULong,
                                                   CORBA::ULong &,
                                                   CORBA::Environment &)
{
  return;
}

void
TAO_Default_Protocols_Hooks::add_rt_service_context_hook (
                                TAO_GIOP_Invocation *,
                                CORBA::Policy *,
                                CORBA::Short &,
                                CORBA::Environment &)
{
  return;
}

void
TAO_Default_Protocols_Hooks::get_selector_hook (CORBA::Policy *,
                                                CORBA::Boolean &,
                                                CORBA::Short &)
{
  return;
}

void
TAO_Default_Protocols_Hooks::get_selector_bands_policy_hook (
                                          CORBA::Policy *,
                                          CORBA::Short &,
                                          CORBA::Short &,
                                          CORBA::Short &,
                                          int &)
{
  return;
}

void
TAO_Default_Protocols_Hooks::select_endpoint_hook (
                                TAO_GIOP_Invocation *,
                                CORBA::Policy *,
                                TAO_Profile *&,
                                CORBA::Environment &)
{
  return;
}

CORBA::Policy *
TAO_Default_Protocols_Hooks::effective_priority_banded_connection_hook (CORBA::Policy *,
                                                                        CORBA::Policy *,
                                                                        CORBA::Environment &)
{
  return CORBA::Policy::_nil ();
}

CORBA::Policy *
TAO_Default_Protocols_Hooks::effective_client_protocol_hook (CORBA::Policy *,
                                                             CORBA::Policy *,
                                                             CORBA::Environment &)
{
   return CORBA::Policy::_nil ();
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Protocols_Hooks,
                       ACE_TEXT ("Protocol_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Protocols_Hooks)
