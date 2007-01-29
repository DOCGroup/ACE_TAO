// -*- C++ -*-

// ===================================================================
/**
 *  @file   DiffServ_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_DIFFSERV_PROTOCOLS_HOOKS_H
#define TAO_DIFFSERVT_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#include "tao/Network_Priority_Protocols_Hooks.h"
#include "ace/Service_Config.h"
#include "tao/DiffServPolicy/DiffServPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DiffServPolicy_Export TAO_DS_Network_Priority_Protocols_Hooks 
      : public TAO_Network_Priority_Protocols_Hooks
{
public:

  /// Constructor
  TAO_DS_Network_Priority_Protocols_Hooks (void);

  /// Destructor
  virtual ~TAO_DS_Network_Priority_Protocols_Hooks (void);

  /// Initialize the protocols hooks instance.
  void init_hooks (TAO_ORB_Core *orb_core
                   ACE_ENV_ARG_DECL);

  CORBA::Long get_dscp_codepoint (void);

  void set_dscp_codepoint (CORBA::Long &dscp_codepoint
                           ACE_ENV_ARG_DECL);

  void np_service_context (TAO_Stub *stub,
                           TAO_Service_Context &service_context,
                           CORBA::Boolean restart
                           ACE_ENV_ARG_DECL);

  void add_rep_np_service_context_hook (
    TAO_Service_Context &service_context,
    CORBA::Long &dscp_codepoint
    ACE_ENV_ARG_DECL);

protected:

  TAO_ORB_Core *orb_core_;

  CORBA::Long dscp_codepoint_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_DiffServPolicy, 
                               TAO_DS_Network_Priority_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO_DiffServPolicy, 
                     TAO_DS_Network_Priority_Protocols_Hooks)

#include /**/ "ace/post.h"
#endif /* TAO_DIFFSERV_PROTOCOLS_HOOKS_H */
