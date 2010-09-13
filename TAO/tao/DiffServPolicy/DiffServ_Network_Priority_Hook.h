// -*- C++ -*-

//=============================================================================
/**
 *  @file    DiffServ_Network_Priority_Hook.h
 *
 *  $Id$
 *
 *  @author  Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *           Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_DIFFSERV_NETWORK_PRIORITY_HOOK_H
#define TAO_DIFFSERV_NETWORK_PRIORITY_HOOK_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#include "tao/DiffServPolicy/DiffServPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Network_Priority_Hook.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DiffServPolicy_Export TAO_DiffServ_Network_Priority_Hook
  : public TAO_Network_Priority_Hook
{
public:
  virtual ~TAO_DiffServ_Network_Priority_Hook(void);

  /// This function is a hook, that is called from the Root_POA's
  /// constructor. It allows the POA to cache the server side network
  /// priority policy, so that it can be used for sending replies with the
  /// policy-specified DiffServ codepoint.
  ///
  void update_network_priority (TAO_Root_POA &poa,
                                TAO_POA_Policy_Set &poa_policy_set);

  /// This function is a hook, that is used by the POA's servant dispatcher
  /// when it tries to assign DiffServ codepoints on the replies.
  ///
  void set_dscp_codepoint (TAO_ServerRequest &req,
                           TAO_Root_POA &poa);
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_DiffServPolicy,
                               TAO_DiffServ_Network_Priority_Hook)
ACE_FACTORY_DECLARE (TAO_DiffServPolicy, TAO_DiffServ_Network_Priority_Hook)

TAO_END_VERSIONED_NAMESPACE_DECL


#include /**/ "ace/post.h"

#endif /* TAO_DIFFSERV_NETWORK_PRIORITY_HOOK_H */
