// -*- C++ -*-

//=============================================================================
/**
 *  @file    IORInterceptor_Adapter_Factory_Impl.h
 *
 *  $Id$
 *
 *  @author  George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_IORINTERCEPTOR_ADAPTER_FACTORY_IMPL_H
#define TAO_IORINTERCEPTOR_ADAPTER_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#include "iorinterceptor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/Service_Config.h"
#include "tao/IORInterceptor_Adapter_Factory.h"


class TAO_IORInterceptor_Adapter;

/**
 * @class TAO_IORInterceptor_Adapter_Factory_Impl
 *
 * @brief TAO_IORInterceptor_Adapter_Factory_Impl.
 *
 * Class that creates instances of TAO_IORInterceptor_Adapter (one per ORB).
 * This is the derived class that contains the actual implementations.
 */
class TAO_IORInterceptor_Export TAO_IORInterceptor_Adapter_Factory_Impl
  : public TAO_IORInterceptor_Adapter_Factory
{
public:
  virtual ~TAO_IORInterceptor_Adapter_Factory_Impl (void);

  virtual TAO_IORInterceptor_Adapter * create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_IORInterceptor_Adapter_Factory_Impl)
ACE_FACTORY_DECLARE (TAO_IORInterceptor, TAO_IORInterceptor_Adapter_Factory_Impl)

static int
TAO_Requires_IORInterceptor_Initializer =
  TAO_IORInterceptor_Adapter_Factory_Impl::Initializer ();

#define TAO_IORINTERCEPTOR_SAFE_INCLUDE
#include "tao/IORInterceptor/IORInterceptorC.h"
#undef TAO_IORINTERCEPTOR_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif /* TAO_IORINTERCEPTOR_ADAPTER_FACTORY_IMPL_H */
