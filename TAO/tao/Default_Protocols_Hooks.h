
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

#include "ace/pre.h"

#include "Protocols_Hooks.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

class TAO_POA;

class TAO_Export TAO_Default_Protocols_Hooks : public TAO_Protocols_Hooks
{
public:

  /// destructor
  virtual ~TAO_Default_Protocols_Hooks (void);

  virtual void init_hooks (TAO_ORB_Core *orb_core
                           ACE_ENV_ARG_DECL);

  virtual int call_client_protocols_hook (int &send_buffer_size,
                                          int &recv_buffer_size,
                                          int &no_delay,
                                          int &enable_network_priority,
                                          const char *protocol_type);

  virtual int call_server_protocols_hook (int &send_buffer_size,
                                          int &recv_buffer_size,
                                          int &no_delay,
                                          int &enable_network_priority,
                                          const char *protocol_type);

  virtual int update_client_protocol_properties (TAO_Stub *stub,
                                                 TAO_Transport *t,
                                                 const char *protocol_type);

  virtual int update_server_protocol_properties (CORBA::Policy *policy,
                                                 TAO_Transport *t,
                                                 const char *protocol_type);

  virtual CORBA::Long get_dscp_codepoint (void);

  virtual void rt_service_context (TAO_Stub *stub,
                                   TAO_Service_Context &service_context,
                                   CORBA::Boolean restart
                                   ACE_ENV_ARG_DECL);

  virtual void add_rt_service_context_hook (TAO_Service_Context &service_context,
                                            CORBA::Policy *model_policy,
                                            CORBA::Short &client_priority
                                            ACE_ENV_ARG_DECL);

  virtual void get_selector_hook (CORBA::Policy *model_policy,
                                  CORBA::Boolean
                                  &is_client_propagated,
                                  CORBA::Short &server_priority);

  virtual void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                               CORBA::Short p,
                                               CORBA::Short &min_priority,
                                               CORBA::Short &max_priority,
                                               int &in_range);

  virtual int get_thread_CORBA_priority (CORBA::Short &
                                         ACE_ENV_ARG_DECL_NOT_USED);

  virtual int get_thread_native_priority (CORBA::Short &
                                          ACE_ENV_ARG_DECL_NOT_USED);

  virtual int get_thread_CORBA_and_native_priority (CORBA::Short &,
                                                    CORBA::Short &
                                                    ACE_ENV_ARG_DECL_NOT_USED);

  virtual int set_thread_CORBA_priority (CORBA::Short
                                         ACE_ENV_ARG_DECL_NOT_USED);

  virtual int set_thread_native_priority (CORBA::Short
                                          ACE_ENV_ARG_DECL_NOT_USED);

  virtual int set_default_policies (ACE_ENV_SINGLE_ARG_DECL);

  virtual int set_default_server_protocol_policy (TAO_Acceptor_Registry &acceptor_registry
                                                  ACE_ENV_ARG_DECL);

};

#if defined (__ACE_INLINE__)
#include "tao/Default_Protocols_Hooks.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Protocols_Hooks)

#include "ace/post.h"
#endif /* TAO_DEFAULT_PROTOCOLS_HOOKS_H */
