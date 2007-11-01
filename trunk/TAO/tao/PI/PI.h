// -*- C++ -*-

// ===================================================================
/**
 *  @file   PI.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_PI_H
#define TAO_PI_H

#include /**/ "ace/pre.h"

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PI_Init
 *
 */
class TAO_PI_Export TAO_PI_Init
{
public:

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

#if defined (TAO_AS_STATIC_LIBS)
// only do this for static builds, it causes a circular
// dependency for dynamic builds.
static int
TAO_Requires_PI_Initializer =
  TAO_PI_Init::Initializer ();
#endif /* TAO_AS_STATIC_LIBS */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#define TAO_PI_SAFE_INCLUDE
#include "tao/PI/ORBInitInfoC.h"
#include "tao/PI/PolicyFactoryC.h"
#include "tao/PI/ORBInitializerC.h"
#include "tao/PI/InterceptorC.h"
#include "tao/PI/ClientRequestInfoC.h"
#include "tao/PI/ClientRequestInterceptorC.h"
#include "tao/PI/PICurrentC.h"
#include "tao/PI/ProcessingModePolicyC.h"
#undef TAO_PI_SAFE_INCLUDE

#endif  /* TAO_PI_H */
