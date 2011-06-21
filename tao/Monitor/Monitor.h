// -*- C++ -*-

// ===================================================================
/**
 *  @file   Monitor.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_MONITOR_H
#define TAO_MONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Monitor/Monitor_export.h"
#include "tao/Versioned_Namespace.h"
#include "tao/Object_Loader.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Monitor_Init
 *
 */
class TAO_Monitor_Export TAO_Monitor_Init : public TAO_Object_Loader
{
public:
  TAO_Monitor_Init (void);

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

//#if defined (TAO_AS_STATIC_LIBS)
// only do this for static builds, it causes a circular
// dependency for dynamic builds.
static int
TAO_Requires_Monitor_Initializer = TAO_Monitor_Init::Initializer ();
//#endif /* TAO_AS_STATIC_LIBS */


ACE_STATIC_SVC_DECLARE (TAO_Monitor_Init)
ACE_FACTORY_DECLARE (TAO_Monitor, TAO_Monitor_Init)
TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#define TAO_MONITOR_SAFE_INCLUDE
#include "tao/Monitor/MonitorC.h"
#undef TAO_MONITOR_SAFE_INCLUDE

#endif  /* TAO_MONITOR_H */
