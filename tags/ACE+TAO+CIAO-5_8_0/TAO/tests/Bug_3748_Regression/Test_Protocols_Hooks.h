// -*- C++ -*-
//
// $Id$
//

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
  Test_Protocols_Hooks (void);

  /// Destructor
  virtual ~Test_Protocols_Hooks (void);

  /// Initialize the protocols hooks instance.
  void init_hooks (TAO_ORB_Core *orb_core);

  CORBA::Boolean set_client_network_priority (IOP::ProfileId protocol_tag,
                                              TAO_Stub *stub);

  CORBA::Boolean set_server_network_priority (IOP::ProfileId protocol_tag,
                                              CORBA::Policy *policy);

  void server_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &protocol_properties);

  void client_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &protocol_properties);

  void server_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &protocol_properties);

  void client_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &protocol_properties);

  void server_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &protocol_properties);

  void client_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &protocol_properties);

  void server_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &protocol_properties);

  void client_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &protocol_properties);

  void server_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &protocol_properties);

  void client_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &protocol_properties);

  CORBA::Long get_dscp_codepoint (void);

  void get_selector_hook (CORBA::Policy *model_policy,
                          CORBA::Boolean
                          &is_client_propagated,
                          CORBA::Short &server_priority);

  void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                       CORBA::Short priority,
                                       CORBA::Short &min_priority,
                                       CORBA::Short &max_priority,
                                       bool &in_range);

  /**
   * Accessor and modifier to the current thread priority, used to
   * implement the RTCORBA::Current interface, but it is faster for
   * some critical components.
   */
  //@{
  int get_thread_CORBA_priority (CORBA::Short &priority);

  int get_thread_native_priority (CORBA::Short &);

  int get_thread_CORBA_and_native_priority (CORBA::Short &, CORBA::Short &);

  int get_thread_implicit_CORBA_priority (CORBA::Short&);

  int set_thread_CORBA_priority (CORBA::Short);

  int restore_thread_CORBA_and_native_priority (CORBA::Short,
                                                CORBA::Short);

  //@}

private:
  CORBA::ULong failure_count_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (ACE_Local_Service, Test_Protocols_Hooks)
ACE_FACTORY_DECLARE (ACE_Local_Service, Test_Protocols_Hooks)

#include /**/ "ace/post.h"
#endif /* TAO_RT_PROTOCOLS_HOOKS_H */
