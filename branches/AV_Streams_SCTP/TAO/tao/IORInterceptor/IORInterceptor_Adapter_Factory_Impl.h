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

#include "IORInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "iorinterceptor_export.h"
#include "tao/IORInterceptor_Adapter_Factory.h"
#include "tao/IORInterceptor/IORInterceptor_Adapter_Impl.h"
#include "ace/Service_Config.h"
#include "IORInterceptor_List.h"

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

#if defined (ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_IORInterceptor_Initializer =
  &TAO_IORInterceptor_Adapter_Factory_Impl::Initializer;

#else

static int
TAO_Requires_IORInterceptor_Initializer =
  TAO_IORInterceptor_Adapter_Factory_Impl::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"

#endif /* TAO_IORINTERCEPTOR_ADAPTER_FACTORY_IMPL_H */
