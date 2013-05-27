// -*- C++ -*-

// ===================================================================
/**
 *  @file   Network_Priority_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_NETWORK_PRIORITY_PROTOCOLS_HOOKS_H
#define TAO_NETWORK_PRIORITY_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Environment;
  class Object;
}

class TAO_ORB_Core;
class TAO_Service_Context;
class TAO_Connection_Handler;
class TAO_Stub;

class TAO_Export TAO_Network_Priority_Protocols_Hooks
      : public ACE_Service_Object
{
public:
  /// destructor
  virtual ~TAO_Network_Priority_Protocols_Hooks (void);

  virtual void init_hooks (TAO_ORB_Core *orb_core) = 0;

  virtual CORBA::Long get_dscp_codepoint (TAO_Stub *stub,
        CORBA::Object *object) = 0;

  virtual CORBA::Long get_dscp_codepoint (TAO_Service_Context &req) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_NETWORK_PRIORITY_PROTOCOLS_HOOKS_H */
