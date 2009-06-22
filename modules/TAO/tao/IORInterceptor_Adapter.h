// -*- C++ -*-

//=============================================================================
/**
 *  @file    IORInterceptor_Adapter.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IORINTERCEPTOR_ADAPTER_H
#define TAO_IORINTERCEPTOR_ADAPTER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI_ForwardC.h"
#include "tao/Policy_ForwardC.h"
#include "tao/TAO_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
template <class T> class ACE_Array_Base;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableInterceptor
{
  class IORInterceptor;
  typedef IORInterceptor *IORInterceptor_ptr;
  class IORInfo;
  typedef IORInfo *IORInfo_ptr;
  class ObjectReferenceTemplate;
}

namespace TAO
{
  typedef ACE_Array_Base< ::PortableInterceptor::ObjectReferenceTemplate*>
    ObjectReferenceTemplate_Array;
}

class TAO_Root_POA;

/**
 * @class TAO_IORInterceptor_Adapter
 *
 * @brief TAO_IORInterceptor_Adapter.
 *
 * Class that adapts various functions involving the PortableInterceptor
 * interfaces IORInfo and IORInterceptor. This is a base class for
 * the actual implementation in the TAO_IORInterceptor library.
 */
class TAO_Export TAO_IORInterceptor_Adapter
{
public:
  virtual ~TAO_IORInterceptor_Adapter (void);

  virtual void add_interceptor (
      PortableInterceptor::IORInterceptor_ptr interceptor) = 0;

  virtual void add_interceptor (
      PortableInterceptor::IORInterceptor_ptr interceptor,
      const CORBA::PolicyList& policies) = 0;

  virtual void destroy_interceptors (void) = 0;

  /// Call the IORInterceptor::establish_components() method on all
  /// registered IORInterceptors.
  /**
   * This method calls IORInterceptor::establish_components() method
   * on all registered IORInterceptors, and
   * IORInterceptor::components_established() once the former is
   * completed.
   */
  virtual void establish_components (TAO_Root_POA *poa) = 0;

  /// Call the IORInterceptor::components_established() method on all
  /// registered IORInterceptors.
  virtual void components_established (PortableInterceptor::IORInfo_ptr info)
    = 0;

  virtual void adapter_state_changed (
      const TAO::ObjectReferenceTemplate_Array &array_obj_ref_template,
      PortableInterceptor::AdapterState state) = 0;

  virtual void adapter_manager_state_changed (
      const char * id,
      PortableInterceptor::AdapterState state) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IORINTERCEPTOR_ADAPTER_H */
