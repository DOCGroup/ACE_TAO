/**
 * @file Client_ORBInitializer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "Client_ORBInitializer.h"
#include "Client_Interceptor.h"

ACE_RCSID(Portable_Interceptor_AMI, Client_ORBInitializer, "$Id$")

Client_ORBInitializer::Client_ORBInitializer ()
{
}

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ClientRequestInterceptor_var interceptor(
      new Echo_Client_Request_Interceptor);

  info->add_client_request_interceptor (interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
