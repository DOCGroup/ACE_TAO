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

#include "ace/Array_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/SystemException.h"
#include "tao/PI_ForwardC.h"
#include "tao/Interceptor_List.h"

namespace PortableInterceptor
{
  class Interceptor;
  typedef Interceptor *Interceptor_ptr;
  class IORInterceptor;
  typedef IORInterceptor *IORInterceptor_ptr;
  class IORInfo;
  typedef IORInfo *IORInfo_ptr;
  class ObjectReferenceTemplate;
}

namespace TAO
{
  typedef Interceptor_List< ::PortableInterceptor::IORInterceptor>
    IORInterceptor_List;

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
      PortableInterceptor::IORInterceptor_ptr interceptor
      ACE_ENV_ARG_DECL
    ) = 0;

  virtual void destroy_interceptors (
      ACE_ENV_SINGLE_ARG_DECL
      ) = 0;

  /// Call the IORInterceptor::establish_components() method on all
  /// registered IORInterceptors.
  /**
   * This method calls IORInterceptor::establish_components() method
   * on all registered IORInterceptors, and
   * IORInterceptor::components_established() once the former is
   * completed.
   */
  virtual void establish_components (TAO_Root_POA *poa ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /// Call the IORInterceptor::components_established() method on all
  /// registered IORInterceptors.
  virtual void components_established (PortableInterceptor::IORInfo_ptr info
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  virtual void adapter_state_changed (
      const TAO::ObjectReferenceTemplate_Array &array_obj_ref_template,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  virtual void adapter_manager_state_changed (
      PortableInterceptor::AdapterManagerId id,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
};

#include /**/ "ace/post.h"
#endif /* TAO_IORINTERCEPTOR_ADAPTER_H */
