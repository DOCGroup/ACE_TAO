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

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_PI_Initializer =
  &TAO_PI_Init::Initializer;

#else

static int
TAO_Requires_PI_Initializer =
  TAO_PI_Init::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"

#define TAO_PI_SAFE_INCLUDE
#include "ORBInitializerC.h"
#include "PolicyFactoryC.h"
#include "ORBInitInfoC.h"
#include "InterceptorC.h"
#include "ClientRequestInterceptorC.h"
#undef TAO_PI_SAFE_INCLUDE

#endif  /* TAO_PI_H */
