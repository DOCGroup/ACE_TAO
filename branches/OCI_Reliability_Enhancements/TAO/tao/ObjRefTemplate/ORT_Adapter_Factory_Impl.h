// -*- C++ -*-

//=============================================================================
/**
 *  @file    ORT_Adapter_Factory_Impl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ORT_ADAPTER_FACTORY_IMPL_H
#define TAO_ORT_ADAPTER_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ObjRefTemplate/ort_export.h"
#include "tao/PortableServer/ORT_Adapter_Factory.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class ORT_Adapter_Factory_Impl
   *
   * @brief ORT_Adapter_Factory_Impl
   *
   * Class that creates instances of TAO::ORT_Adapter
   * (one per POA). This is the derived class that contains the actual
   * implementation.
   */
  class TAO_ORT_Export ORT_Adapter_Factory_Impl
    : public ORT_Adapter_Factory
  {
  public:
    /// Create an adapter
    virtual TAO::ORT_Adapter * create ();

    /// Destroy an adapter that is created by this factory
    virtual void destroy (TAO::ORT_Adapter * adapter);

    /// Used to force the initialization of the code.
    static int Initializer (void);
  };
}

static int TAO_Requires_ORTFactory_Initializer =
  TAO::ORT_Adapter_Factory_Impl::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (ORT_Adapter_Factory_Impl)
ACE_FACTORY_DECLARE (TAO_ORT, ORT_Adapter_Factory_Impl)

#define TAO_OBJREF_TEMPLATE_SAFE_INCLUDE
#include "tao/ObjRefTemplate/ObjectReferenceTemplateC.h"
#undef TAO_OBJREF_TEMPLATE_SAFE_INCLUDE
#include /**/ "ace/post.h"

#endif /* TAO_ORT_ADAPTER_FACTORY_IMPL_H */
