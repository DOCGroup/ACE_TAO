// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    IORInterceptor_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_IORINTERCEPTOR_ADAPTER_FACTORY_H
#define TAO_IORINTERCEPTOR_ADAPTER_FACTORY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORInterceptor_Adapter.h"

/**
 * @class TAO_IORInterceptor_Adapter_Factory
 *
 * @brief TAO_IORInterceptor_Adapter_Factory.
 *
 * Class that creates one instance of TAO_IOR_Interceptor_Adapter per
 * ORB on the ORB's first usage of its ior_interceptor_adapter_. This is a base
 * class for the actual implementation in the TAO_IORInterceptor library.
 */
class TAO_Export TAO_IORInterceptor_Adapter_Factory : public ACE_Service_Object
{
public:
  virtual ~TAO_IORInterceptor_Adapter_Factory (void);

  virtual TAO_IORInterceptor_Adapter * create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
};

#include "ace/post.h"
#endif /* TAO_IORINTERCEPTOR_ADAPTER_FACTORY_H */
