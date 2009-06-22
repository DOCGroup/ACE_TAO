// -*- C++ -*-

//=============================================================================
/**
 *  @file    Network_Priority_Hook.h
 *
 *  $Id$
 *
 *  Provides default network priority hook for use by the POA
 *
 *  @author  Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_NETWORK_PRIORITY_HOOK_H
#define TAO_NETWORK_PRIORITY_HOOK_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "tao/orbconf.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_POA_Policy_Set;
class TAO_Root_POA;
class TAO_ServerRequest;

class TAO_PortableServer_Export TAO_Network_Priority_Hook
  : public ACE_Service_Object
{
public:
  virtual ~TAO_Network_Priority_Hook(void);

  virtual void update_network_priority (TAO_Root_POA& poa,
                                        TAO_POA_Policy_Set& poa_policy_set);


  virtual void set_dscp_codepoint (TAO_ServerRequest &req,
                                   TAO_Root_POA& poa);

  /// Static initializer ensures the factory is loaded
  static int initialize (void);

};


static int
TAO_Requires_Network_Priority_Hook =
TAO_Network_Priority_Hook::initialize ();

ACE_STATIC_SVC_DECLARE (TAO_Network_Priority_Hook)
ACE_FACTORY_DECLARE (TAO_PortableServer, TAO_Network_Priority_Hook)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_NETWORK_PRIORITY_HOOK_H */
