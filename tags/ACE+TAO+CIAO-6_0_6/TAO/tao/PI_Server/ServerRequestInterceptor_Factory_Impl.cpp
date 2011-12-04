/* -*- C++ -*- */

// =================================================================
/**
 * @file ServerRequestInterceptor_Factory_Impl.cpp
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 */
// =================================================================

#include "tao/PI_Server/ServerRequestInterceptor_Factory_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI_Server/ServerInterceptorAdapter.h"

#include "tao/ORB.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::ServerRequestInterceptor_Adapter*
TAO_ServerRequestInterceptor_Adapter_Factory_Impl::create (void)
{
  TAO::ServerRequestInterceptor_Adapter_Impl* obj = 0;
  ACE_NEW_RETURN (obj,
                  TAO::ServerRequestInterceptor_Adapter_Impl ,
                  0);
  return obj;
}


ACE_STATIC_SVC_DEFINE (TAO_ServerRequestInterceptor_Adapter_Factory_Impl,
                       ACE_TEXT ("ServerRequestInterceptor_Adapter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_ServerRequestInterceptor_Adapter_Factory_Impl),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PI_Server, TAO_ServerRequestInterceptor_Adapter_Factory_Impl)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS */
