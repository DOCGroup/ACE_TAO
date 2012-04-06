/* -*- C++ -*- */

//=============================================================================
/**
 *  @file PortableServer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_H
#define TAO_PORTABLESERVER_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_PortableServer_Export TAO_POA_Initializer
{
public:
  /// Used to force the initialization of the ORB code.
  static int init (void);
};

static int
TAO_Requires_POA_Initializer = TAO_POA_Initializer::init ();

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "tao/PortableServer/PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE
#include "tao/PortableServer/POAManagerC.h"
#include "tao/PortableServer/POAManagerFactoryC.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "tao/PortableServer/PortableServer_Functions.h"
#include "tao/PortableServer/PortableServer_WFunctions.h"
#include "tao/PortableServer/PS_ForwardA.h"
#include "tao/PortableServer/IdUniquenessPolicyA.h"
#include "tao/PortableServer/ImplicitActivationPolicyA.h"
#include "tao/PortableServer/RequestProcessingPolicyA.h"
#include "tao/PortableServer/ServantRetentionPolicyA.h"
#include "tao/PortableServer/ThreadPolicyA.h"
#include "tao/PortableServer/IdAssignmentPolicyA.h"
#include "tao/PortableServer/LifespanPolicyA.h"
#include "tao/PortableServer/AdapterActivatorA.h"

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_H */
