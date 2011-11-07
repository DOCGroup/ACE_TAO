// -*- C++ -*-

//=============================================================================
/**
 *  @file    RT_Object_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Frank Hunleth
 */
// ===================================================================

#ifndef TAO_RT_OBJECT_ADAPTER_FACTORY_H
#define TAO_RT_OBJECT_ADAPTER_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/RTPortableServer/rtportableserver_export.h"

#include "tao/Adapter_Factory.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTPortableServer_Export TAO_RT_Object_Adapter_Factory
  : public TAO_Adapter_Factory
{
public:

  /// Create adapter.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};


ACE_STATIC_SVC_DECLARE (TAO_RT_Object_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_RTPortableServer, TAO_RT_Object_Adapter_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_OBJECT_ADAPTER_FACTORY_H */
