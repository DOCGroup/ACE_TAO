// -*- C++ -*-

// =================================================================
/**
 * @file ClientRequestInterceptor_Factory_Impl.cpp
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 */
// =================================================================

#include "tao/PI/ClientRequestInterceptor_Factory_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/ClientRequestInterceptor_Adapter_Impl.h"

#include "tao/ORB.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::ClientRequestInterceptor_Adapter*
TAO_ClientRequestInterceptor_Adapter_Factory_Impl::create (void)
{
  TAO::ClientRequestInterceptor_Adapter_Impl* obj = 0;
  ACE_NEW_RETURN (obj,
                  TAO::ClientRequestInterceptor_Adapter_Impl ,//(interceptors, invocation),
                  0);
  return obj;
}


ACE_STATIC_SVC_DEFINE (TAO_ClientRequestInterceptor_Adapter_Factory_Impl,
                       ACE_TEXT ("ClientRequestInterceptor_Adapter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_ClientRequestInterceptor_Adapter_Factory_Impl),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PI, TAO_ClientRequestInterceptor_Adapter_Factory_Impl)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
