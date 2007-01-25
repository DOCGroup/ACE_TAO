// -*- C++ -*-

// ===================================================================
/**
 *  @file   Default_Policy_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_DEFAULT_POLICY_PROTOCOLS_HOOKS_H
#define TAO_DEFAULT_POLICY_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Policy_Protocols_Hooks.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_POA;

class TAO_Export TAO_Default_Policy_Protocols_Hooks 
      : public TAO_Policy_Protocols_Hooks
{
public:

  /// destructor
  virtual ~TAO_Default_Policy_Protocols_Hooks (void);

  CORBA::Boolean set_client_network_priority (IOP::ProfileId protocol_tag,
                                              TAO_Stub *stub
                                              ACE_ENV_ARG_DECL);

  CORBA::Boolean set_server_network_priority (IOP::ProfileId protocol_tag,
                                              CORBA::Policy *policy
                                              ACE_ENV_ARG_DECL);

  CORBA::Long get_dscp_codepoint (void);

  void set_dscp_codepoint (CORBA::Long &dscp_codepoint
                           ACE_ENV_ARG_DECL);

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
                                       CORBA::Short p,
                                       CORBA::Short &min_priority,
                                       CORBA::Short &max_priority,
                                       int &in_range);

};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Policy_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Policy_Protocols_Hooks)

#include /**/ "ace/post.h"
#endif /* TAO_DEFAULT_POLICY_PROTOCOLS_HOOKS_H */
