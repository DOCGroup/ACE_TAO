// $Id$

#include "FTRT_ClientORB_Initializer.h"
#include "FTRT_ClientORB_Interceptor.h"

ACE_RCSID (ClientORB,
           FTRT_ClientORB_Initializer,
           "$Id$")


FTRT_ClientORB_Initializer::FTRT_ClientORB_Initializer (int transaction_depth)
: client_interceptor_ ( new FTRT_ClientORB_Interceptor(transaction_depth) )
{
}

void
FTRT_ClientORB_Initializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FTRT_ClientORB_Initializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  info->add_client_request_interceptor (client_interceptor_.in()
                                        ACE_ENV_ARG_PARAMETER);
}


