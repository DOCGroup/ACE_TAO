// -*- C++ -*-

// ===================================================================
/**
 *  @file   Policy_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_POLICY_PROTOCOLS_HOOKS_H
#define TAO_POLICY_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"
#include "tao/IOP_IORC.h"
#include "ace/SString.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Policy;
  class Environment;
}

class TAO_ORB_Core;
class TAO_Service_Context;
class TAO_Connection_Handler;
class TAO_Stub;

class TAO_Export TAO_Policy_Protocols_Hooks 
      : public ACE_Service_Object
{
public:
  /// destructor
  virtual ~TAO_Policy_Protocols_Hooks (void);

  virtual void init_hooks (TAO_ORB_Core *orb_core
                           ACE_ENV_ARG_DECL) = 0;

  virtual CORBA::Boolean set_client_network_priority (
    IOP::ProfileId protocol_tag,
    TAO_Stub *stub
    ACE_ENV_ARG_DECL) = 0;

  virtual CORBA::Boolean set_server_network_priority (
    IOP::ProfileId protocol_tag,
    CORBA::Policy *policy
    ACE_ENV_ARG_DECL) = 0;

  virtual CORBA::Long get_dscp_codepoint (void) = 0;

  virtual void set_dscp_codepoint (CORBA::Long &dscp_codepoint
                                   ACE_ENV_ARG_DECL) = 0;

  virtual void rt_service_context (TAO_Stub *stub,
                                   TAO_Service_Context &service_context,
                                   CORBA::Boolean restart
                                   ACE_ENV_ARG_DECL) = 0;

  virtual void add_rt_service_context_hook (
    TAO_Service_Context &service_context,
    CORBA::Policy *model_policy,
    CORBA::Short &client_priority
    ACE_ENV_ARG_DECL) = 0;

  virtual void get_selector_hook (CORBA::Policy *model_policy,
                                  CORBA::Boolean
                                  &is_client_propagated,
                                  CORBA::Short &server_priority) = 0;

  virtual void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                               CORBA::Short priority,
                                               CORBA::Short &min_priority,
                                               CORBA::Short &max_priority,
                                               int &in_range) = 0;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_POLICY_PROTOCOLS_HOOKS_H */
