
// -*- C++ -*-

// ===================================================================
/**
 *  @file   RT_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 *
 */
// ===================================================================

#ifndef TAO_RT_PROTOCOLS_HOOKS_H
#define TAO_RT_PROTOCOLS_HOOKS_H
#include "ace/pre.h"

#include "tao/Protocols_Hooks.h"
#include "Priority_Mapping_Manager.h"
#include "Network_Priority_Mapping_Manager.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTCORBA_Export TAO_RT_Protocols_Hooks : public TAO_Protocols_Hooks
{
public:
  /// Constructor
  TAO_RT_Protocols_Hooks (void);

  /// Destructor
  virtual ~TAO_RT_Protocols_Hooks (void);

  /// Initialize the protocols hooks instance.
  virtual void init_hooks (TAO_ORB_Core *orb_core
                           ACE_ENV_ARG_DECL);

  /// Invoke the client_protocols hook if present.
  /**
   * The timeout hook is used to determine if the client_protocols policy is
   * set and with what value.  If the ORB is compiled without support
   * for RTCORBA this feature does not take effect
   * @param tcp_properties returns the Protocol List set
   */
  virtual int call_client_protocols_hook (int &send_buffer_size,
					  int &recv_buffer_size,
					  int &no_delay,
					  int &enable_network_priority,
					  const char *protocol_type);

  /// Define the Client_Protocols_TCP_Hook signature
  typedef int (*Client_Protocols_Hook) (TAO_ORB_Core *,
                                        int &send_buffer_size,
                                        int &recv_buffer_size,
                                        int &no_delay,
					int &enable_network_priority,
                                        const char *);

  static void set_client_protocols_hook (Client_Protocols_Hook hook);

  /// The hook to be set for the ClientProtocolPolicy.
  static Client_Protocols_Hook client_protocols_hook_;

  /// Invoke the server_protocols hook if present.
  /**
   * The timeout hook is used to determine if the client_protocols policy is
   * set and with what value.  If the ORB is compiled without support
   * for RTCORBA this feature does not take effect
   * @param tcp_properties returns the Protocol List set
   */
  virtual int call_server_protocols_hook (int &send_buffer_size,
                                          int &recv_buffer_size,
                                          int &no_delay,
					  int &enable_network_priority,
                                          const char *protocol_type);

  /// Define the Server_Protocols_Hook signature
  typedef int (*Server_Protocols_Hook) (TAO_ORB_Core *,
                                        int &,
                                        int &,
                                        int &,
					int &,
                                        const char *);

  static void set_server_protocols_hook (Server_Protocols_Hook hook);
  //  static void set_server_protocols_uiop_hook (Server_Protocols_UIOP_Hook hook);

  /// The hook to be set for the ServerProtocolPolicy.
  static Server_Protocols_Hook server_protocols_hook_;

/*
  virtual int get_effective_client_protocol_properties (TAO_Stub *stub,
							int &send_buffer_size,
							int &recv_buffer_size,
							int &no_delay,
							int &enable_network_priority,
							const char *protocol_type);

  virtual int get_effective_server_protocol_properties (TAO_Adpater *poa,
							int &send_buffer_size,
							int &recv_buffer_size,
							int &no_delay,
							int &enable_network_priority,
							const char *protocol_type);

  */

  virtual int update_client_protocol_properties (TAO_Stub *stub,
							TAO_Connection_Handler * connection_handler,
							const char *protocol_type);


  virtual int update_server_protocol_properties (CORBA::Policy *server_policy,
							TAO_Connection_Handler * connection_handler,
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

  //@}

  /// 1. Sets ORB-level policy defaults for this ORB.  Currently sets
  /// default RTCORBA policies: ClientProtocolPolicy.
  virtual int set_default_policies (ACE_ENV_SINGLE_ARG_DECL);

  int set_default_server_protocol_policy (TAO_Acceptor_Registry &acceptor_registry
                                          ACE_ENV_ARG_DECL);

protected:
  TAO_ORB_Core *orb_core_;

  // Save a reference to the priority mapping manager.
  TAO_Priority_Mapping_Manager_var mapping_manager_;
  TAO_Network_Priority_Mapping_Manager_var network_mapping_manager_;

  RTCORBA::Current_var current_;
};

#if defined (__ACE_INLINE__)
#include "RT_Protocols_Hooks.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, TAO_RT_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO_RTCORBA, TAO_RT_Protocols_Hooks)

#include "ace/post.h"
#endif /* TAO_RT_PROTOCOLS_HOOKS_H */












