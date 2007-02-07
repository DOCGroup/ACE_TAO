// -*- C++ -*-

// ===================================================================
/**
 *  @file   Default_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
// ===================================================================

#ifndef TAO_DEFAULT_PROTOCOLS_HOOKS_H
#define TAO_DEFAULT_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Protocols_Hooks.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_POA;

class TAO_Export TAO_Default_Protocols_Hooks : public TAO_Protocols_Hooks
{
public:

  /// destructor
  virtual ~TAO_Default_Protocols_Hooks (void);

  void init_hooks (TAO_ORB_Core *orb_core);

  CORBA::Boolean set_client_network_priority (IOP::ProfileId protocol_tag,
                                              TAO_Stub *stub
                                             );

  CORBA::Boolean set_server_network_priority (IOP::ProfileId protocol_tag,
                                              CORBA::Policy *policy
                                             );

  void server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                               );

  void client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                               );

  void server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                               );

  void client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                               );

  void server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                               );

  void client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                               );

  void server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                               );

  void client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                               );

  void server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                               );

  void client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                               );

  CORBA::Long get_dscp_codepoint (void);

  void rt_service_context (TAO_Stub *stub,
                           TAO_Service_Context &service_context,
                           CORBA::Boolean restart
                          );

  void add_rt_service_context_hook (TAO_Service_Context &service_context,
                                    CORBA::Policy *model_policy,
                                    CORBA::Short &client_priority
                                   );

  void get_selector_hook (CORBA::Policy *model_policy,
                          CORBA::Boolean
                          &is_client_propagated,
                          CORBA::Short &server_priority);

  void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                       CORBA::Short p,
                                       CORBA::Short &min_priority,
                                       CORBA::Short &max_priority,
                                       bool &in_range);

  int get_thread_CORBA_priority (CORBA::Short &);

  int get_thread_native_priority (CORBA::Short &);

  int get_thread_CORBA_and_native_priority (CORBA::Short &,
                                            CORBA::Short &);

  int set_thread_CORBA_priority (CORBA::Short);

  int set_thread_native_priority (CORBA::Short);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Protocols_Hooks)

#include /**/ "ace/post.h"
#endif /* TAO_DEFAULT_PROTOCOLS_HOOKS_H */
