// -*- C++ -*-

// ===================================================================
/**
 *  @file   PI.h
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_PI_H
#define TAO_PI_H

#include /**/ "ace/pre.h"

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PI_Init
 */
class TAO_PI_Export TAO_PI_Init
{
public:

  /// Used to force the initialization of the ORB code.
  static int Initializer ();
};

#if defined (TAO_AS_STATIC_LIBS) || (defined (ACE_VXWORKS) && !defined (__RTP__))
// only do this for static builds, it causes a circular
// dependency for dynamic builds.
static int
TAO_Requires_PI_Initializer = TAO_PI_Init::Initializer ();
#endif /* TAO_AS_STATIC_LIBS */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#define TAO_PI_SAFE_INCLUDE
#include "ORBInitInfoC.h"
#include "PolicyFactoryC.h"
#include "ORBInitializerC.h"
#include "InterceptorC.h"
#include "ClientRequestInfoC.h"
#include "ClientRequestInterceptorC.h"
#include "PICurrentC.h"
#include "ProcessingModePolicyC.h"
#undef TAO_PI_SAFE_INCLUDE

#endif  /* TAO_PI_H */
