/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    RT_Object_Adapter_Factory.h
//
// = AUTHOR
//    Frank Hunleth
//
// ============================================================================

#ifndef TAO_RT_OBJECT_ADAPTER_FACTORY_H
#define TAO_RT_OBJECT_ADAPTER_FACTORY_H
#include "ace/pre.h"

#include "rtportableserver_export.h"
#include "tao/Adapter.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTPortableServer_Export TAO_RT_Object_Adapter_Factory : public TAO_Adapter_Factory
{
public:
  TAO_RT_Object_Adapter_Factory (void);
  // Constructor

  // = The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  // details.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};

ACE_STATIC_SVC_DECLARE (TAO_RT_Object_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_RTPortableServer, TAO_RT_Object_Adapter_Factory)

#include "ace/post.h"
#endif /* TAO_RT_OBJECT_ADAPTER_FACTORY_H */
