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

#include "iorinterceptor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/IORInterceptor_Adapter.h"
#include "tao/IORInterceptor/IORInterceptor_List.h"
#include "tao/IORInterceptor/IORInterceptor.h"

class TAO_POA;

/**
 * @class TAO_IORInterceptor_Adapter_Impl
 *
 * @brief TAO_IORInterceptor_Adapter_Impl.
 *
 * Class that adapts various functions involving the PortableInterceptor
 * interfaces IORInfo and IORInterceptor. This is the derived class
 * that contains the actual implementations.
 */
class TAO_IORInterceptor_Export TAO_IORInterceptor_Adapter_Impl
  : public TAO_IORInterceptor_Adapter
{
public:
  virtual ~TAO_IORInterceptor_Adapter_Impl (void);

  virtual void add_interceptor (PortableInterceptor::IORInterceptor_ptr interceptor
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (());

  virtual TAO_IORInterceptor_List *interceptor_list (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void establish_components (TAO_POA *poa ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Call the IORInterceptor::components_established() method on all
  /// registered IORInterceptors.
  virtual void components_established (PortableInterceptor::IORInfo_ptr info
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void adapter_state_changed (
      const TAO_ObjectReferenceTemplate_Array &array_obj_ref_template,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void adapter_manager_state_changed (
      PortableInterceptor::AdapterManagerId id,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// List of IOR interceptors maintained
  TAO_IORInterceptor_List ior_interceptor_list_;
};

#include /**/ "ace/post.h"

#endif /* TAO_IORINTERCEPTOR_ADAPTER_IMPL_H */
