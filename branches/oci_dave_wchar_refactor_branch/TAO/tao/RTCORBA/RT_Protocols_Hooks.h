// -*- C++ -*-

// ===================================================================
/**
 *  @file   RT_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
// ===================================================================

#ifndef TAO_RT_PROTOCOLS_HOOKS_H
#define TAO_RT_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"

#include "tao/Protocols_Hooks.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTCORBA_Export TAO_RT_Protocols_Hooks : public TAO_Protocols_Hooks
{
public:

  /// Constructor
  TAO_RT_Protocols_Hooks (void);

  /// Destructor
  virtual ~TAO_RT_Protocols_Hooks (void);

  /// Initialize the protocols hooks instance.
  void init_hooks (TAO_ORB_Core *orb_core
                   ACE_ENV_ARG_DECL);

  CORBA::Boolean set_client_network_priority (IOP::ProfileId protocol_tag,
                                              TAO_Stub *stub
                                              ACE_ENV_ARG_DECL);

  CORBA::Boolean set_server_network_priority (IOP::ProfileId protocol_tag,
                                              CORBA::Policy *policy
                                              ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  CORBA::Long get_dscp_codepoint (void);

  void rt_service_context (TAO_Stub *stub,
                           TAO_Service_Context &service_context,
                           CORBA::Boolean restart
                           ACE_ENV_ARG_DECL);

  void add_rt_service_context_hook (TAO_Service_Context &service_context,
                                    CORBA::Policy *model_policy,
                                    CORBA::Short &client_priority
                                    ACE_ENV_ARG_DECL);

  void get_selector_hook (CORBA::Policy *model_policy,
                          CORBA::Boolean
                          &is_client_propagated,
                          CORBA::Short &server_priority);

  void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                       CORBA::Short priority,
                                       CORBA::Short &min_priority,
                                       CORBA::Short &max_priority,
                                       int &in_range);

  /**
   * Accessor and modifier to the current thread priority, used to
   * implement the RTCORBA::Current interface, but it is faster for
   * some critical components.
   */
  //@{
  int get_thread_CORBA_priority (CORBA::Short &
                                 ACE_ENV_ARG_DECL);

  int get_thread_native_priority (CORBA::Short &
                                  ACE_ENV_ARG_DECL);

  int get_thread_CORBA_and_native_priority (CORBA::Short &,
                                            CORBA::Short &
                                            ACE_ENV_ARG_DECL);

  int set_thread_CORBA_priority (CORBA::Short
                                 ACE_ENV_ARG_DECL);

  int set_thread_native_priority (CORBA::Short
                                  ACE_ENV_ARG_DECL);

  //@}

protected:

  RTCORBA::ProtocolProperties_ptr server_protocol_properties (IOP::ProfileId protocol_tag,
                                                              CORBA::Policy_ptr policy
                                                              ACE_ENV_ARG_DECL);

  RTCORBA::ProtocolProperties_ptr client_protocol_properties (IOP::ProfileId protocol_tag,
                                                              CORBA::Policy_ptr policy
                                                              ACE_ENV_ARG_DECL);

  RTCORBA::ProtocolProperties_ptr server_protocol_properties_at_orb_level (IOP::ProfileId protocol_tag
                                                                           ACE_ENV_ARG_DECL);

  RTCORBA::ProtocolProperties_ptr client_protocol_properties_at_orb_level (IOP::ProfileId protocol_tag
                                                                           ACE_ENV_ARG_DECL);

  RTCORBA::ProtocolProperties_ptr client_protocol_properties_at_object_level (IOP::ProfileId protocol_tag,
                                                                              TAO_Stub *stub
                                                                              ACE_ENV_ARG_DECL);

  void extract_protocol_properties (TAO_IIOP_Protocol_Properties &to,
                                    RTCORBA::ProtocolProperties_ptr from
                                    ACE_ENV_ARG_DECL);

  void extract_protocol_properties (TAO_UIOP_Protocol_Properties &to,
                                    RTCORBA::ProtocolProperties_ptr from
                                    ACE_ENV_ARG_DECL);

  void extract_protocol_properties (TAO_SHMIOP_Protocol_Properties &to,
                                    RTCORBA::ProtocolProperties_ptr from
                                    ACE_ENV_ARG_DECL);

  void extract_protocol_properties (TAO_DIOP_Protocol_Properties &to,
                                    RTCORBA::ProtocolProperties_ptr from
                                    ACE_ENV_ARG_DECL);

  void extract_protocol_properties (TAO_SCIOP_Protocol_Properties &to,
                                    RTCORBA::ProtocolProperties_ptr from
                                    ACE_ENV_ARG_DECL);

  CORBA::Boolean set_network_priority (IOP::ProfileId protocol_tag,
                                       RTCORBA::ProtocolProperties_ptr protocol_properties
                                       ACE_ENV_ARG_DECL);

protected:

  TAO_ORB_Core *orb_core_;

  // Save a reference to the priority mapping manager.
  TAO_Priority_Mapping_Manager_var mapping_manager_;
  TAO_Network_Priority_Mapping_Manager_var network_mapping_manager_;

  RTCORBA::Current_var current_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, TAO_RT_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO_RTCORBA, TAO_RT_Protocols_Hooks)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_RT_PROTOCOLS_HOOKS_H */
