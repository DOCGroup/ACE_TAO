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

#include "Protocols_Hooks.h"

#if (TAO_HAS_RT_CORBA == 1)

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_RT_Protocols_Hooks : public TAO_Protocols_Hooks
{
public:

  /// constructor
  TAO_RT_Protocols_Hooks (void);

  /// destructor
  virtual ~TAO_RT_Protocols_Hooks (void);

  /// Invoke the client_protocols hook if present.
  /**
   * The timeout hook is used to determine if the client_protocols policy is
   * set and with what value.  If the ORB is compiled without support
   * for RTCORBA this feature does not take effect
   * \param tcp_properties returns the Protocol List set
   */

  virtual int call_client_protocols_hook (
                 TAO_ORB_Core *orb_core,
                 int &send_buffer_size,
                 int &recv_buffer_size,
                 int &no_delay,
                 const char *protocol_type);

  /// Define the Client_Protocols_TCP_Hook signature
  typedef int (*Client_Protocols_Hook) (TAO_ORB_Core *,
                                        int &send_buffer_size,
                                        int &recv_buffer_size,
                                        int &no_delay,
                                        const char *);

  static void set_client_protocols_hook (Client_Protocols_Hook hook);

  /// The hook to be set for the ClientProtocolPolicy.
  static Client_Protocols_Hook client_protocols_hook_;

  /// Invoke the server_protocols hook if present.
  /**
   * The timeout hook is used to determine if the client_protocols policy is
   * set and with what value.  If the ORB is compiled without support
   * for RTCORBA this feature does not take effect
   * \param tcp_properties returns the Protocol List set
   */
  virtual int call_server_protocols_hook (TAO_ORB_Core *orb_core,
                                          int &send_buffer_size,
                                          int &recv_buffer_size,
                                          int &no_delay,
                                          const char *protocol_type);

  /// Define the Server_Protocols_Hook signature
  typedef int (*Server_Protocols_Hook) (TAO_ORB_Core *,
                                        int &,
                                        int &,
                                        int &,
                                        const char *);

  static void set_server_protocols_hook (Server_Protocols_Hook hook);
  //  static void set_server_protocols_uiop_hook (Server_Protocols_UIOP_Hook hook);

  /// The hook to be set for the ServerProtocolPolicy.
  static Server_Protocols_Hook server_protocols_hook_;

  /**
   * to get the policy_type in Long format.
   *
   */
  virtual void call_policy_type_hook (CORBA::PolicyList *&policy_list,
                                      CORBA::ULong &policy_type);

  virtual void validate_policy_type (CORBA::ULong slot,
                                     CORBA::ULong &type_value,
                                     CORBA::Environment &ACE_TRY_ENV);

  virtual void add_rt_service_context_hook (
                  TAO_GIOP_Invocation *invocation,
                  CORBA::Policy *model_policy,
                  CORBA::Short &client_priority,
                  CORBA::Environment &ACE_TRY_ENV);

  virtual void get_selector_hook (CORBA::Policy *model_policy,
                                  CORBA::Boolean
                                  &is_client_propagated,
                                  CORBA::Short &server_priority);

  virtual void get_selector_bands_policy_hook (
                    CORBA::Policy *bands_policy,
                    CORBA::Short &min_priority,
                    CORBA::Short &max_priority,
                    CORBA::Short &p,
                    int &in_range);


  /**
   * Hook to check and override the exposed policies if needed
   *
   */
  virtual CORBA::Policy *effective_priority_banded_connection_hook (CORBA::Policy *override,
                                                                    CORBA::Policy *exposed,
                                                                    CORBA::Environment &);

  virtual CORBA::Policy *effective_client_protocol_hook (CORBA::Policy *override,
                                                         CORBA::Policy *exposed,
                                                         CORBA::Environment &);

  /**
   * Accessor and modifier to the current thread priority, used to
   * implement the RTCORBA::Current interface, but it is faster for
   * some critical components.  If TAO_HAS_RT_CORBA == 0, the
   * operations are no-ops.
   */
  //@{
  virtual int get_thread_priority (TAO_ORB_Core *orb_core,
                                   CORBA::Short &priority,
                                   CORBA::Environment &ACE_TRY_ENV);

  virtual int set_thread_priority (TAO_ORB_Core *orb_core,
                                   CORBA::Short  priority,
                                   CORBA::Environment &ACE_TRY_ENV);
  //@}

  virtual void set_priority_mapping (TAO_ORB_Core *orb_core,
                                     TAO_Resource_Factory *trf,
                                     CORBA::Environment &ACE_TRY_ENV);

  /// 1. Sets ORB-level policy defaults for this ORB.  Currently sets
  /// default RTCORBA policies: ServerProtocolPolicy and
  /// ClientProtocolPolicy.
  virtual int set_default_policies (TAO_ORB_Core *orb_core);
};

#if defined (__ACE_INLINE__)
#include "RT_Protocols_Hooks.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_RT_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO, TAO_RT_Protocols_Hooks)
ACE_STATIC_SVC_REQUIRE(TAO_RT_Protocols_Hooks)

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_RT_PROTOCOLS_HOOKS_H */
