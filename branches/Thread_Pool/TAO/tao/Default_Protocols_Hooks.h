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

class TAO_Export TAO_Default_Protocols_Hooks : public TAO_Protocols_Hooks
{
public:

  /// destructor
  virtual ~TAO_Default_Protocols_Hooks (void);

  virtual void init_hooks (TAO_ORB_Core *orb_core,
                           CORBA::Environment &ACE_TRY_ENV);

  virtual int call_client_protocols_hook (int &send_buffer_size,
                                          int &recv_buffer_size,
                                          int &no_delay,
                                          const char *protocol_type);

  virtual int call_server_protocols_hook (int &send_buffer_size,
                                          int &recv_buffer_size,
                                          int &no_delay,
                                          const char *protocol_type);

  virtual void rt_service_context (TAO_Stub *stub,
                                   TAO_Service_Context &service_context,
                                   CORBA::Boolean restart,
                                   CORBA::Environment &ACE_TRY_ENV);

  virtual void add_rt_service_context_hook (TAO_Service_Context &service_context,
                                            CORBA::Policy *model_policy,
                                            CORBA::Short &client_priority,
                                            CORBA::Environment &ACE_TRY_ENV);

  virtual void get_selector_hook (CORBA::Policy *model_policy,
                                  CORBA::Boolean
                                  &is_client_propagated,
                                  CORBA::Short &server_priority);

  virtual void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                               CORBA::Short &min_priority,
                                               CORBA::Short &max_priority,
                                               CORBA::Short &p,
                                               int &in_range);

  virtual int get_thread_priority (CORBA::Short &,
                                   CORBA::Environment &);

  virtual int set_thread_priority (CORBA::Short,
                                   CORBA::Environment &);

  virtual int set_default_policies (CORBA::Environment &ACE_TRY_ENV);

  /// Sets the default ServerProtocolPolicy.
  virtual int set_default_server_protocol_policy (TAO_Acceptor_Registry &acceptor_registry,
                                                  CORBA::Environment &ACE_TRY_ENV);

};

#if defined (__ACE_INLINE__)
#include "tao/Default_Protocols_Hooks.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Protocols_Hooks)

#include "ace/post.h"
#endif /* TAO_DEFAULT_PROTOCOLS_HOOKS_H */
