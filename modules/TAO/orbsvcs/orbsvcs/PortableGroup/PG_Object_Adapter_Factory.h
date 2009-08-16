// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Object_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen
 */
// ===================================================================

#ifndef TAO_PG_OBJECT_ADAPTER_FACTORY_H
#define TAO_PG_OBJECT_ADAPTER_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#include "tao/PortableServer/Object_Adapter_Factory.h"
#include "tao/orbconf.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_PortableGroup_Export TAO_PG_Object_Adapter_Factory : public TAO_Object_Adapter_Factory
{
public:
  /// Constructor
  TAO_PG_Object_Adapter_Factory (void);

  /// Create adapter.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_PG_Object_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_PortableGroup, TAO_PG_Object_Adapter_Factory)

#include /**/ "ace/post.h"

#endif /* TAO_PG_OBJECT_ADAPTER_FACTORY_H */
