// -*- C++ -*-
//
// $Id$
//

#include "Test_Protocols_Hooks.h"
#include "TestC.h"

Test_Protocols_Hooks::Test_Protocols_Hooks (void)
  : failure_count_ (0)
{
}


Test_Protocols_Hooks::~Test_Protocols_Hooks (void)
{
}

void
Test_Protocols_Hooks::init_hooks (TAO_ORB_Core *)
{
  // No-op.
}

CORBA::Boolean
Test_Protocols_Hooks::set_client_network_priority (IOP::ProfileId,
                                                   TAO_Stub *)
{
  return false;
}

CORBA::Boolean
Test_Protocols_Hooks::set_server_network_priority (IOP::ProfileId,
                                                   CORBA::Policy *)
{
  return false;
}

void
Test_Protocols_Hooks::server_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &)
{
  if (++this->failure_count_ < Test::expected_failure_number)
    throw ::CORBA::INTERNAL ();
}

void
Test_Protocols_Hooks::client_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::server_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::client_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::server_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::client_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::server_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::client_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::server_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &)
{
  // No-op.
}

void
Test_Protocols_Hooks::client_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &)
{
  // No-op.
}

CORBA::Long
Test_Protocols_Hooks::get_dscp_codepoint (void)
{
  return -1;
}

void
Test_Protocols_Hooks::get_selector_hook (
    CORBA::Policy *,
    CORBA::Boolean &,
    CORBA::Short &)
{
  // No-op.
}

void
Test_Protocols_Hooks::get_selector_bands_policy_hook (
    CORBA::Policy *,
    CORBA::Short,
    CORBA::Short &,
    CORBA::Short &,
    bool &
  )
{
  // No-op.
}

int
Test_Protocols_Hooks::get_thread_CORBA_priority (CORBA::Short &)
{
  return -1;
}

int
Test_Protocols_Hooks::get_thread_native_priority (
    CORBA::Short &)
{
  return -1;
}

int
Test_Protocols_Hooks::get_thread_CORBA_and_native_priority (
    CORBA::Short &,
    CORBA::Short &)
{
  return -1;
}

int
Test_Protocols_Hooks::get_thread_implicit_CORBA_priority (CORBA::Short &)
{
  return -1;
}

int
Test_Protocols_Hooks::restore_thread_CORBA_and_native_priority (
    CORBA::Short,
    CORBA::Short
  )
{
  return -1;
}

int
Test_Protocols_Hooks::set_thread_CORBA_priority (CORBA::Short)
{
  return -1;
}

ACE_STATIC_SVC_DEFINE (Test_Protocols_Hooks,
                       ACE_TEXT ("Test_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Test_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (ACE_Local_Service, Test_Protocols_Hooks)
