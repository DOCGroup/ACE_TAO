// -*- C++ -*-

#ifndef TEST_PROTOCOLS_HOOKS_H
#define TEST_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"

#include "tao/Protocols_Hooks.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Test_Protocols_Hooks : public TAO_Protocols_Hooks
{
public:
  /// Constructor
  Test_Protocols_Hooks () = default;

  /// Destructor
  ~Test_Protocols_Hooks () override = default;
  Test_Protocols_Hooks (Test_Protocols_Hooks const &) = delete;
  Test_Protocols_Hooks (Test_Protocols_Hooks &&) = delete;
  Test_Protocols_Hooks &operator= (Test_Protocols_Hooks const &) = delete;
  Test_Protocols_Hooks &operator= (Test_Protocols_Hooks &&) = delete;

  /// Initialize the protocols hooks instance.
  void init_hooks (TAO_ORB_Core *orb_core) override;

  CORBA::Boolean set_client_network_priority (IOP::ProfileId protocol_tag,
                                              TAO_Stub *stub) override;

  CORBA::Boolean set_server_network_priority (IOP::ProfileId protocol_tag,
                                              CORBA::Policy *policy) override;

  void server_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &protocol_properties) override;

  void client_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &protocol_properties) override;

  void server_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &protocol_properties) override;

  void client_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &protocol_properties) override;

  void server_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &protocol_properties) override;

  void client_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &protocol_properties) override;

  void server_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &protocol_properties) override;

  void client_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &protocol_properties) override;

  void server_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &protocol_properties) override;

  void client_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &protocol_properties) override;

  CORBA::Long get_dscp_codepoint () override;

  void get_selector_hook (CORBA::Policy *model_policy,
                          CORBA::Boolean
                          &is_client_propagated,
                          CORBA::Short &server_priority) override;

  void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                       CORBA::Short priority,
                                       CORBA::Short &min_priority,
                                       CORBA::Short &max_priority,
                                       bool &in_range) override;

  /**
   * Accessor and modifier to the current thread priority, used to
   * implement the RTCORBA::Current interface, but it is faster for
   * some critical components.
   */
  //@{
  int get_thread_CORBA_priority (CORBA::Short &priority) override;

  int get_thread_native_priority (CORBA::Short &) override;

  int get_thread_CORBA_and_native_priority (CORBA::Short &, CORBA::Short &) override;

  int get_thread_implicit_CORBA_priority (CORBA::Short &) override;

  int set_thread_CORBA_priority (CORBA::Short) override;

  int restore_thread_CORBA_and_native_priority (CORBA::Short,
                                                CORBA::Short) override;

  //@}

private:
  CORBA::ULong failure_count_ {};
};

ACE_STATIC_SVC_DECLARE_EXPORT (ACE_Local_Service, Test_Protocols_Hooks)
ACE_FACTORY_DECLARE (ACE_Local_Service, Test_Protocols_Hooks)

#include /**/ "ace/post.h"
#endif /* TAO_RT_PROTOCOLS_HOOKS_H */
