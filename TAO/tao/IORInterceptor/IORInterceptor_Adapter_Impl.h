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

#include "ace/pre.h"

#include "IORInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "iorinterceptor_export.h"
#include "tao/IORInterceptor_Adapter.h"
#include "ace/Service_Config.h"
#include "IORInterceptor_List.h"

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
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual TAO_IORInterceptor_List *interceptor_list (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Used to force the initialization of the ORB code.
  static int Initializer (void);

private:

  /// List of IOR interceptors maintained
  TAO_IORInterceptor_List ior_interceptor_list_;
};

ACE_STATIC_SVC_DECLARE (TAO_IORInterceptor_Adapter_Impl)
ACE_FACTORY_DECLARE (TAO_IORInterceptor, TAO_IORInterceptor_Adapter_Impl)

#if defined (ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_IORInterceptor_Initializer =
  &TAO_IORInterceptor_Adapter_Impl::Initializer;

#else

static int
TAO_Requires_IORInterceptor_Initializer =
  TAO_IORInterceptor_Adapter_Impl::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include "ace/post.h"

#endif /* TAO_IORINTERCEPTOR_ADAPTER_IMPL_H */
