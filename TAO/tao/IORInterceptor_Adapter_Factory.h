// -*- C++ -*-

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

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/TAO_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IORInterceptor_Adapter;

/**
 * @class TAO_IORInterceptor_Adapter_Factory
 *
 * @brief TAO_IORInterceptor_Adapter_Factory.
 *
 * Class that creates one instance of TAO_IOR_Interceptor_Adapter per
 * ORB on the ORB's first usage of its ior_interceptor_adapter_. This is a base
 * class for the actual implementation in the TAO_IORInterceptor library.
 */
class TAO_Export TAO_IORInterceptor_Adapter_Factory
  : public ACE_Service_Object
{
public:
  virtual ~TAO_IORInterceptor_Adapter_Factory (void);

  virtual TAO_IORInterceptor_Adapter * create (void) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_IORINTERCEPTOR_ADAPTER_FACTORY_H */
