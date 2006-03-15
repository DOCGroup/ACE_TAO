// -*- C++ -*-

//=============================================================================
/**
 *  @file    ORT_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_ORT_ADAPTER_FACTORY_H
#define TAO_ORT_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "ace/Service_Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class ORT_Adapter;
}

namespace TAO
{
  /**
   * @class ORT_Adapter_Factory
   *
   * @brief ORT_Adapter_Factory
   *
   * Class that creates one instance of TAO::ORT_Adapter per
   * POA on the POA's first usage of its objectreferencetemplate_adapter.
   * This is a base class for the actual implementation in the
   * TAO_ObjectReferenceTemplate library.
   */
  class TAO_PortableServer_Export ORT_Adapter_Factory
    : public ACE_Service_Object
  {
  public:

    /// Destructor.
    /**
     * @note Even though this class only defines an interface, a
     *       destructor is necessary to avoid dynamic_cast<> failures
     *       when using g++ 4.0's -fvisibility-inlines-hidden command
     *       line option.  Apparently the compiler generated
     *       destructor is inlined.
     */
    virtual ~ORT_Adapter_Factory (void);

    /// Create a new adapter, in case not possible to allocate, returns 0
    virtual TAO::ORT_Adapter * create () = 0;

    /// Destroy the passed adapter
    virtual void destroy (TAO::ORT_Adapter * adapter) = 0;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ORT_ADAPTER_FACTORY_H */
