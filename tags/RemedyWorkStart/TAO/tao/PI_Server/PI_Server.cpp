// -*- C++ -*-
// $Id$

#include "tao/PI_Server/PI_Server.h"
#include "tao/PI_Server/PI_Server_Loader.h"
#include "tao/PI_Server/ServerRequestInterceptor_Factory_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_PI_Server_Init::Initializer (void)
{
#if TAO_HAS_INTERCEPTORS == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_ServerRequestInterceptor_Adapter_Factory_Impl);
#endif  /* TAO_HAS_INTERCEPTORS */

  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_PI_Server_Loader);
}

TAO_END_VERSIONED_NAMESPACE_DECL
