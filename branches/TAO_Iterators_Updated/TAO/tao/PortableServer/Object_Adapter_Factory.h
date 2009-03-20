// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_OBJECT_ADAPTER_FACTORY_H
#define TAO_OBJECT_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Adapter_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_PortableServer_Export TAO_Object_Adapter_Factory
  : public TAO_Adapter_Factory
{
public:

  // = The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  // details.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};

ACE_STATIC_SVC_DECLARE (TAO_Object_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_PortableServer, TAO_Object_Adapter_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_ADAPTER_FACTORY_H */
