// -*- C++ -*-

//=============================================================================
/**
 *  @file    IORInterceptor_Adapter_Impl.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IORINTERCEPTOR_ADAPTER_IMPL_H
#define TAO_IORINTERCEPTOR_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/IORInterceptor/iorinterceptor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORInterceptor/IORInterceptor.h"
#include "tao/IORInterceptor_Adapter.h"
#include "tao/PI/Interceptor_List_T.h"
#include "tao/IORInterceptor/IORInterceptor_Details.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  typedef Interceptor_List< ::PortableInterceptor::IORInterceptor,
                            IORInterceptor_Details>
    IORInterceptor_List;
}

class TAO_Root_POA;

/**
 * @class TAO_IORInterceptor_Adapter_Impl
 *
 * @brief TAO_IORInterceptor_Adapter_Impl.
 *
 * Class that adapts various functions involving the PortableInterceptor
 * interfaces IORInfo and IORInterceptor. This is the derived class
 * that contains the actual implementations.
 */
class TAO_IORInterceptor_Adapter_Impl
  : public TAO_IORInterceptor_Adapter
{
public:
  virtual ~TAO_IORInterceptor_Adapter_Impl (void);

  virtual void add_interceptor (
      PortableInterceptor::IORInterceptor_ptr interceptor);

  virtual void add_interceptor (
      PortableInterceptor::IORInterceptor_ptr interceptor,
      const CORBA::PolicyList& policies);

  virtual void destroy_interceptors (void);

  virtual void establish_components (TAO_Root_POA *poa );

  /// Call the IORInterceptor::components_established() method on all
  /// registered IORInterceptors.
  virtual void components_established (PortableInterceptor::IORInfo_ptr info);

  virtual void adapter_state_changed (
      const TAO::ObjectReferenceTemplate_Array &array_obj_ref_template,
      PortableInterceptor::AdapterState state);

  virtual void adapter_manager_state_changed (
      const char * id,
      PortableInterceptor::AdapterState state);

private:
  /// List of IOR interceptors maintained
  TAO::IORInterceptor_List ior_interceptor_list_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_IORINTERCEPTOR_ADAPTER_IMPL_H */
