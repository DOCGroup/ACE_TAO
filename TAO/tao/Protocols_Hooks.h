// -*- C++ -*-

// ===================================================================
/**
 *  @file   Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 *
 */
// ===================================================================

#ifndef TAO_PROTOCOLS_HOOKS_H
#define TAO_PROTOCOLS_HOOKS_H
#include "ace/pre.h"

#include "ORB_Core.h"
#include "RTCORBAC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

class TAO_Export TAO_Protocols_Hooks : public ACE_Service_Object
{
public:

  /// constructor
  TAO_Protocols_Hooks (void);

  /// destructor
  virtual ~TAO_Protocols_Hooks (void);

  virtual int call_client_protocols_hook (TAO_ORB_Core *orb_core,
                                          RTCORBA::ProtocolProperties_var &
                                          properties,
                                          const char *protocol_type);

  virtual int call_server_protocols_hook (TAO_ORB_Core *orb_core,
                                          RTCORBA::ProtocolProperties_var &
                                          properties,
                                          const char *protocol_type);


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

  virtual void select_endpoint_hook (TAO_GIOP_Invocation *invocation,
                                     CORBA::Policy *client_protocol_policy,
                                     TAO_Profile *&profile,
                                     CORBA::Environment &ACE_TRY_ENV);

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
};

#include "ace/post.h"
#endif /* TAO_PROTOCOLS_HOOKS_H */
