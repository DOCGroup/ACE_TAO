// -*- C++ -*-
//
// $Id$

#include "Echo_Client_ORBInitializer.h"

ACE_RCSID (Dynamic, Echo_Client_ORBInitializer, "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

#include "interceptors.h"

void
Echo_Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the Echo client request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Client_Request_Interceptor,
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
