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

#ifndef TAO_MONITOR_H
#define TAO_MONITOR_H

#include /**/ "ace/pre.h"

#include "tao/Monitor/monitor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Monitor_Init
 *
 */
class TAO_Monitor_Export TAO_Monitor_Init
{
public:

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

#if defined (TAO_AS_STATIC_LIBS)
// only do this for static builds, it causes a circular
// dependency for dynamic builds.
static int
TAO_Requires_Monitor_Initializer =
  TAO_Monitor_Init::Initializer ();
#endif /* TAO_AS_STATIC_LIBS */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#define TAO_MONITOR_SAFE_INCLUDE
#include "tao/Monitor/MonitorC.h"
#undef TAO_PI_SAFE_INCLUDE

#endif  /* TAO_MONITOR_H */
